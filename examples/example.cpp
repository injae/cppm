#include "cppm/cmake/script.hpp"
#include "cppm/core/config.hpp"
#include <range/v3/all.hpp>
#include "options/cppm.h"
#include <cpcli/cpcli.hpp>

int main(int argc, char* argv[]) {
    using namespace cppm::cmake;
    using namespace cppm::core;
    using namespace ranges;
    cpcli::parse<cppm::option::Cppm>("cppm").parse(argc, argv);

    //auto path = cppm::util::reverse_find_file("../../", "cppm.toml");
    //auto config = cppm::core::Config::load(path->parent_path());
    //std::string cmake = CMakeScript{
    //    cmake_minimum_required().flag("VERSION", config.cmake.version),
    //    space,
    //    set("CPPM_VERSION").arg(config.package.tool_version),
    //    _include("cmake/cppm_loader.cmake"),
    //    space,
    //    cppm_project().flag("WITH_VCPKG", config.package.with_vcpkg),
    //    // hunter
    //    cppm_setting().flag("UNITY_BUILD", config.package.unity_build),
    //    cppm_cxx_standard(config.package.standard),
    //    space,
    //    config.cmake.include | views::for_each([](auto it){ return yield(_include(it)); }) | to_vector,
    //    // compiler
    //    // include
    //    // load_dependencies
    //    config.dependencies | views::transform([](auto it) {
    //        auto& [name, dep] = it;
    //        auto script = find_cppkg(dep.name).arg(dep.version)
    //            .flag("MODULE",     dep.module)
    //            .flag("COMPONENTS", dep.components )
    //            .flag("LOADPATH",   dep.path)
    //            .flag("HUNTER", dep.repo == repo_type::hunter)
    //            .flag("TYPE", serde::to_str(dep.type))
    //            .flag("OPTIONAL", "OFF", dep.optional);
    //        return script;
    //    }) | to_vector,
    //    space,

    //    
        //}.set_depth(-1);
        //fmt::print("{}",cmake);

    return 0;
}

