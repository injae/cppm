#ifndef __CPPKG_CPPKG_H__
#define __CPPKG_CPPKG_H__

#include<string>
#include"config/dependency.h"
#include"config/config.h"
#include"cppkg/repo.h"

namespace cppkg {
    using dep_in_repo = std::pair<cppm::Dependency,std::string>;
    cppm::Dependency parse(const std::string& name, std::string path="");
    std::string tanslate(cppm::Dependency& dep);
    void init(const std::string& name);
    void init(cppm::Dependency& dep);
    Repos list();
    void build(const std::string& name);
    void regist(const std::string& name);
    dep_in_repo search(const std::string& name, const std::string& version); 
    void install(cppm::Config& config, const dep_in_repo& depr);
}

#endif
