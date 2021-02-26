#include "option/cppkg_install.h"
#include "cppm/cppkg/cppkg.h"
#include "cppm/util/system.hpp"
#include "cppm/core/config.hpp"
#include "cppm/util/string.hpp"
#include "cppm/util/filesystem.h"
#include "cppm/core/cppm_tool.hpp"
#include "option/build.h"

#include <fmt/format.h>

using namespace fmt::literals;
using namespace cppm::util::str;

namespace cppm::option
{
    CppkgInstall::CppkgInstall(){
        app_.add_option("version").abbr("v").args("{name}")
            .desc("package version, default is latest")
            .call_back([&](const std::string& arg){ version = arg; });
        app_.add_command()
            .call_back([&](){
                     if(app_.args().size() < 1) { fmt::print(stderr, "need package name");  exit(1); }
                else if(app_.args().size() > 1) { fmt::print(stderr, "too many arguments"); exit(1); }
                auto name = app_.get_arg();
                auto [dep, dep_path] = cppkg::search(name, version);
                auto inst_proj = _base_cppm_project();
                auto path = "{}cache/__install"_format(core::cppm_root());
                if(fs::exists(path)) {
                    fs::remove_all("{}/build"_format(path));
                    fs::remove_all("{}/thirdparty"_format(path));
                }
                fs::create_directories(path);
                fs::create_directories(path + "/cmake");
                fs::create_directories(path + "/src");
                inst_proj += "{} = {}\n"_format(dep.name, quot(*dep.version));
                util::write_file(path + "/cppm.toml" , inst_proj);
                //auto main = "\nint main(int argc, char* argv[]) {\n\n     return 0; \n}";
                //util::write_file(path + "/src/main.cpp" , main);

                auto build = Build();
                build.start_from(path);
                std::vector<std::string> args = {"install"};
                build.app().parse(app_);
                build.app().args().insert(build.app().args().end(), args.begin(), args.end());
                fs::remove_all(path);
            });
    }

    std::string CppkgInstall::_base_cppm_project() {
        std::string gen
            = "[package]\n name={}\n version={}\n"_format("installer"_quot, "0.0.0"_quot)
            //+ "[[bin]]\n name = {}\n source = [{}]\n"_format("installer"_quot, "src/.*"_quot)
            + "[dependencies]\n";
        return gen;
    }        
}
