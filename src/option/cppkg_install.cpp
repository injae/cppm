#include "option/cppkg_install.h"
#include "cppkg/cppkg.h"
#include "util/system.hpp"
#include "config/config.h"
#include "util/string.hpp"
#include "util/filesystem.h"
#include "config/cppm_tool.h"
#include "option/build.h"

#include <fmt/format.h>

using namespace fmt::literals;
using namespace cppm::util::str;

namespace cppm::option
{
    CppkgInstall::CppkgInstall(){
        app_.add_option("version").abbr("v").args("{name}")
            .desc("package version, default is latest")
            .call_back([&](){ version = app_.get_arg(); });
        app_.add_command()
            .call_back([&](){
                     if(app_.args().size() < 1) { fmt::print(stderr, "need package name");  exit(1); }
                else if(app_.args().size() > 1) { fmt::print(stderr, "too many arguments"); exit(1); }
                auto name = app_.get_arg();
                cppkg::search(name, version);
                auto inst_proj = _base_cppm_project();
                auto path = "{}install/__install"_format(tool::cppm_root());
                if(fs::exists(path)) {
                    fs::remove_all("{}/build"_format("build"));
                }
                fs::create_directories(path);
                fs::create_directories(path + "/cmake");
                fs::create_directories(path + "/src");
                inst_proj += "{} = {}\n"_format(name, quot(version));
                util::write_file(path + "/cppm.toml" , inst_proj);
                //auto main = "\nint main(int argc, char* argv[]) {\n\n     return 0; \n}";
                //util::write_file(path + "/src/main.cpp" , main);

                auto build = Build();
                build.start_from(path);
                std::vector<std::string> args = {"--local", "--release"};
                build.app().args().insert(build.app().args().end(),args.begin(), args.end());
                build.app().parse(app_);
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
