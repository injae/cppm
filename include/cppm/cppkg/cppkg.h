#pragma once

#ifndef __CPPKG_CPPKG_H__
#define __CPPKG_CPPKG_H__

#include <string>
#include "cppm/core/config.hpp"
#include "cppm/cppkg/repo.h"

namespace cppkg {
    using dep_in_repo = std::pair<cppm::core::Dependency,std::string>;
    cppm::core::Dependency parse(const std::string& name, std::string path="");
    //    std::string translate(cppm::core::Dependency& dep);
    void init(const std::string& name);
    void init(cppm::core::Dependency& dep);
    Repos list();
    void build(const std::string& name);
    void regist(const std::string& name);
    void transcompile(cppm::core::Dependency& dep, const std::string& path);
    dep_in_repo search(const std::string& name, const std::string& version); 
    void install(cppm::core::Config& config, const dep_in_repo& depr);
    cppm::core::Dependency cppkg_auto_gen(std::string& path);
}

#endif
