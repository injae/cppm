#pragma once

#ifndef __CPPM_CORE_CPPKG_HPP__
#define __CPPM_CORE_CPPKG_HPP__

#include "cppm/core/dependency.hpp"
#include "cppm/util/hash.hpp"
#include <functional>
#include <serdepp/utility.hpp>

namespace cppm::core {
    class Cppkg {
    public:
        derive_serde(Cppkg,
            using namespace utilpp::literals;
                      ctx
                     .name(type)
                     .TAG(name)
                     .tag(namespace_, "namespace")
                     .TAG(source)
                     .tag(exclude_var, "flag");
                    switch(utilpp::hash(type))
                    { 
                    case "lib"_h:
                        ctx.tag(cppkg_type, "type", "static") ;
                        ctx.TAG_OR(install, true);
                        break;
                    case "bin"_h:
                        ctx.TAG_OR(install, true);
                        ctx.tag(cppkg_type, "type", "binary") ;
                        break;
                    case "test"_h:
                    case "example"_h:
                        ctx.tag(cppkg_type, "type", "binary") ;
                        ctx.TAG_OR(install, false);
                        break;
                    default:
                        fmt::print(stderr, "unkown target type \"{}\"\n",type); exit(1);
                    })
        std::string name;
        std::string type; // bin | lib | test | example
        std::optional<std::string> cppkg_type;
        std::optional<std::string> namespace_;
        std::optional<bool> install;
        std::optional<std::vector<std::string>> source;
        std::optional<std::string> exclude_var;
        std::optional<bool> exclude;
    };

    class Cppkgs {
    public:
        derive_serde(Cppkgs, ctx
                     .TAG(lib)
                     .tag(bins, "bin")
                     .tag(tests, "test")
                     .tag(examples, "example");
                     if constexpr (std::decay_t<decltype(ctx)>::serialize_step) {
                        if(lib) list.push_back(lib.value());
                        if(bins) {
                            list.reserve( bins->size() + 1 ); // preallocate memory
                        }
                        if(tests) {
                            list.reserve( tests->size() + 1 ); // preallocate memory
                            list.insert( list.end(), tests->begin(), tests->end() );
                        }
                     }
                     )
        std::optional<Cppkg> lib;
        std::optional<std::vector<Cppkg>> bins;
        std::optional<std::vector<Cppkg>> tests;
        std::optional<std::vector<Cppkg>> examples;
        std::vector<Cppkg> list;
    };
}

#endif

