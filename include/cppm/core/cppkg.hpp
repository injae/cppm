#pragma once

#ifndef __CPPM_CORE_CPPKG_HPP__
#define __CPPM_CORE_CPPKG_HPP__

#include "cppm/core/dependency.hpp"
#include "cppm/util/hash.hpp"
#include <functional>
#include <tomlpp/orm.hpp>

namespace cppm::core {
    class Cppkg : public toml::orm::table {
    public:
        template<typename Def>
        void parse(Def& defn) {
            using namespace utilpp::literals;
            type = defn.name();
            defn.element(TOML_D(name))
                .element(namespace_, "namespace")
                .element(TOML_D(source))
                .element(exclude_var, "flag");

            switch(utilpp::hash(type))
            {
            case "lib"_h:
                defn.element(cppkg_type, "type", "static") ;
                defn.element(TOML_D(install), true);
                break;
            case "bin"_h:
                defn.element(TOML_D(install), true);
                defn.element(cppkg_type, "type", "binary") ;
                break;
            case "test"_h:
            case "example"_h:
                defn.element(cppkg_type, "type", "binary") ;
                defn.element(TOML_D(install), false);
                break;
            default:
                fmt::print(stderr, "unkown target type \"{}\"\n",type); exit(1);
            }
        }
        std::string name;
        std::string type; // bin | lib | test | example
        opt<std::string> cppkg_type;
        opt<std::string> namespace_;
        opt<bool> install;
        opt<arr<std::string>> source;
        opt<std::string> exclude_var;
        opt<bool> exclude;
    };

    class Cppkgs : public toml::orm::inline_define {
    public:
        template<typename Def>
        void parse(Def& defn) {
            opt<Cppkg> lib;
            opt<arr<Cppkg>> bins;
            opt<arr<Cppkg>> tests;
            opt<arr<Cppkg>> examples;
            defn.element(bins, "bin");
            defn.element(bins, "test");
            defn.element(bins, "example");
            defn.element(TOML_D(lib));
            if(lib) list.push_back(lib.value());
            if(bins) {
                list.reserve( bins->size() + 1 ); // preallocate memory

            }
            if(tests) {
                list.reserve( tests->size() + 1 ); // preallocate memory
                list.insert( list.end(), tests->begin(), tests->end() );
            }
        }
        arr<Cppkg> list;
    };

}

#endif

