#include "option/cppkg_install.h"
#include "package/package.h"
#include "util/system.hpp"
#include "config/config.h"
#include "util/string.hpp"
#include "util/filesystem.h"
#include "config/cppm_tool.h"

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
                     if(app_.args().size() < 1) { fmt::print(stderr, "need package name"); }
                else if(app_.args().size() > 1) { fmt::print(stderr, "too many arguments"); }
                auto name = app_.get_arg();
                package::cppkg::search(name, version);
                auto inst_proj = _base_cppm_project();
                inst_proj += "{} = {}\n"_format(name, quot(version));
                auto path = "{}install/__install"_format(tool::cppm_root());
                fs::create_directories(path);
                fs::create_directories(path + "/cmake");
                util::write_file(path + "/cppm.toml" , inst_proj);
                util::system::exec(("cd {} && cppm build"_format(path)).c_str());
            });
    }

    std::string CppkgInstall::_base_cppm_project() {
        std::string gen
            = "[package]\n name={}\n version={}\n"_format("installer"_quot, "0.0.0"_quot)
            + "[dependencies]\n";
        return gen;
    }        
}
