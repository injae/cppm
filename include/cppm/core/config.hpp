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
#include "cppm/core/features.hpp"
#include "cppm/util/optional.hpp"
#include "cppm/util/filesystem.h"
#include <serdepp/utility.hpp>

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

    class Config {
    public:
        derive_serde(Config, ctx
                     .TAG(package)
                     .TAG_OR(hunter, Hunter())
                     .TAG_OR(cmake, CMake())
                     .tag(features, "features", std::map<std::string, Feature>())
                     .TAG(workspace)
                     .TAG(lib)
                     .tag(bins , "bin")
                     .tag(tests, "test")
                     .tag(examples, "example")
                     .TAG(dependencies)
                     .tag(dev_dependencies, "dev-dependencies")
                     .TAG(profile)
                     .TAG(target)
                     .no_remain();)
        Package package;
        Path path;
        std::optional<Hunter> hunter;
        std::optional<CMake> cmake;
        std::optional<Workspace> workspace; 
        std::optional<Cppkg> lib;
        std::optional<std::vector<Cppkg>> bins;
        std::optional<std::vector<Cppkg>> tests;
        std::optional<std::vector<Cppkg>> examples;
        std::optional<std::map<std::string, Feature>> features;
        std::vector<Cppkg*> cppkgs;
        std::optional<std::map<std::string, Dependency>> dependencies;
        std::optional<std::map<std::string, Dependency>> dev_dependencies;
        std::optional<std::map<std::string, Profile>> profile;
        std::optional<std::map<std::string, Target>> target;
    public:
        static std::optional<Config> load(const std::string& path);
    private:
    };
}
#endif
