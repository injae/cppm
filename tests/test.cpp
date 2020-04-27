#include <cppm/core/config.hpp>
#include <fmt/format.h>
#include <range/v3/all.hpp>
#include <cppm/core/cppm_tool.hpp>
#include "test.h"

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

using namespace ranges;

TEST_CASE("TEST Catch2", "[test]") {
    REQUIRE( test() == 1);
    REQUIRE( test() == 1);
}


//int main() {
//    auto config = cppm::core::Config::load(fs::current_path());
//    fmt::print(cppm_translate(config.value()));
//    test("helo");

    //auto dep_print = [](auto& target, std::string name) {
    //    fmt::print("[{}]\n",name);
    //    if(target) {
    //        ranges::for_each(target.value(), [](auto i) {
    //            auto& [_,com] = i;
    //            fmt::print("{}:{} [-{}-] -> [{}]:{}\n",com.name, *com.version, com.module, *com.repo,*com.path);
    //        });
    //    }
    //};
    //dep_print(config->dependencies, "dependencies");
    //dep_print(config->dev_dependencies, "dev-dependencies");

    //fmt::print("[bins]\n");
    //if(config->bins) {
    //    ranges::for_each(config->bins.value(), [](auto& i) {
    //            fmt::print("{}:{}[install:{}] -> \n{}\n",i.type, i.name, *i.install, fmt::join(i.source.value(), "\n"));
    //    });
    //}
    //fmt::print("[lib]\n");
    //if(config->lib) {
    //    auto& i = *config->lib;
    //    fmt::print("{}:{}[install:{}] -> \n{}\n",i.type, i.name, *i.install, fmt::join(i.source.value(), " "));
    //}

    //ranges::for_each(*config.profile, [](auto it) {
    //    auto& [_, profile] = it;
    //    fmt::print("[profile] {}\n",profile.name);
    //    if(profile.compiler) {
    //        ranges::for_each(*profile.compiler, [](auto it) {
    //            auto [_, com] = it;
    //            fmt::print("[compiler] => {} = {}\n",com.name,*com.option);
    //        });
    //    }
    //    if(profile.package) {
    //        ranges::for_each(*profile.package, [](auto it) {
    //            auto& [_, package] = it;
    //            fmt::print("[package] => {}\n",package.name);
    //            if(package.compiler) {
    //                ranges::for_each(*package.compiler, [](auto it) {
    //                    auto [_, com] = it;
    //                    fmt::print("[compiler] => {} = {}\n",com.name,*com.option);
    //                });
    //            }
    //            fmt::print("==========================\n");
    //        });
    //    }
    //    fmt::print("\n");
    //});


    //    return 0;
    //}
