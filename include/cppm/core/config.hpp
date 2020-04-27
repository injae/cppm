#pragma once

#ifndef __CPPM_CORE_CONFIG_HPP__
#define __CPPM_CORE_CONFIG_HPP__

#include <string>
#include "cppm/core/package.hpp"
#include "cppm/core/hunter.hpp"
#include "cppm/core/cmake.hpp"
#include "cppm/core/workspace.hpp"
#include "cppm/core/profile.hpp"
#include "cppm/core/cppkg.hpp"
#include "cppm/core/target.hpp"
#include "cppm/util/optional.hpp"
#include "cppm/util/filesystem.h"

namespace cppm::core {
    struct Path
    {
        inline static Path make(const std::string& root_path)  {
          Path path;
          path.root       = root_path;
          path.build      = path.root/"build";
          path.cmake      = path.root/"cmake";
          path.source     = path.root/"src";
          path.include    = path.root/"include";
          path.thirdparty = path.root/"thirdparty";
          return path;
      }
    public:
        fs::path root;
        fs::path build;
        fs::path cmake;
        fs::path source;
        fs::path include;
        fs::path thirdparty;
    };

    class Config :public toml::orm::table {
    public:
        template<typename Def>
        void parse(Def& defn) {
            defn.element(TOML_D(package))
                .element(TOML_D(hunter))
                .element(TOML_D(cmake))
                .element(TOML_D(workspace))
                .element(TOML_D(lib))
                .element(bins, "bin")
                .element(tests,"test")
                .element(examples, "example")
                .element(TOML_D(dependencies))
                .element(dev_dependencies, "dev-dependencies")
                .element(TOML_D(profile))
                .element(TOML_D(target))
                .no_remains();
        }
        Package package;
        Path path;
        opt<Hunter> hunter;
        opt<CMake> cmake;
        opt<Workspace> workspace; 
        opt<Cppkg> lib;
        opt<arr<Cppkg>> bins;
        opt<arr<Cppkg>> tests;
        opt<arr<Cppkg>> examples;
        arr<Cppkg*> cppkgs;
        opt<nested<Dependency>> dependencies;
        opt<nested<Dependency>> dev_dependencies;
        opt<nested<Profile>> profile;
        opt<nested<Target>> target;
    public:
        static std::optional<Config> load(const std::string& path);
    private:
    };
}

#endif
