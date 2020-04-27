#include "option/cppkg.h"
#include "option/cppkg_init.h"
#include "cppm/core/cppm_tool.hpp"
#include "cppm/cppkg/cppkg.h"
#include "cppm/util/filesystem.h"
#include "cppm/util/string.hpp"
#include "option/cppkg_install.h"
#include "option/cppkg_search.h"

#include <fmt/format.h>

using namespace fmt::literals;
using namespace cppm::util::str;

namespace cppm::option
{
    Cppkg::Cppkg() {
        app_.add_command<CppkgInit>("init").args("{option} {name}").desc("init cppkg");
        app_.add_command("build").args("{cppkg name}")
            .desc("build cppm package file")
            .call_back([&](){ _build(); });
        app_.add_command("update")
            .desc("update cppkg repo")
            .call_back([&](){ _update(); });
        app_.add_command<CppkgSearch>("search").args("{name}").desc("search cppkg repo, default is only cppkg");
        app_.add_command("push").args("{name}")
            .desc("push cppkg in local repo")
            .call_back([&](){ _push(); });
        app_.add_command<CppkgInstall>("install").args("{name}").desc("install cppkg package");
    }

    void Cppkg::_update() {
        using namespace fmt::literals;
        auto cppkg_path = "{0}repo/cppkg"_format(core::cppm_root());
        auto command = "cd {0} && git pull"_format(cppkg_path);
        system(command.c_str());
    }

    void Cppkg::_build() {
        if(app_.args().empty())    { fmt::print(stderr,"need argument");     exit(1);}
        if(app_.args().size() > 1) { fmt::print(stderr,"too many argument"); exit(1);}
        cppkg::build(app_.get_arg());
    }

    void Cppkg::_push() {
        if(app_.args().empty())    { fmt::print(stderr,"need argument");}
        if(app_.args().size() > 1) { fmt::print(stderr,"too many argument");}
        cppkg::regist(app_.get_arg());
    }

}
