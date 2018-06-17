#include<nieel/util/hash.hpp>
#include<nieel/util/string.h>
#include"cppm_thirdparty.h"
#include"cmake/generator.h"
#include"cmake/find_package.h"
#include"cppm.h"
#include"url.h"


namespace cppm 
{
    Repository classificate_repo(std::string url) {
        using namespace nieel::util;
        auto parsed = *parse_url(url);
        Repository repo; 
        repo.url = url;
        if(has_str(parsed.protocol, "http") || has_str(parsed.protocol, "https")) {
            if(has_str(parsed.path, "git")) {
                repo.type = "git";
            }
            else if(has_str(parsed.path,"svn")) {
                repo.type = "svn";
            }
            else if(   has_str(parsed.path, "tar")
                    || has_str(parsed.path, "zip")
                    || has_str(parsed.path, "gz"))
            {
                repo.type = "link";
            }
            else {
                repo.type = "unknown";
            }
        }
        else if(has_str(parsed.path, "tar") || has_str(parsed.path, "zip") || has_str(parsed.path, "gz")) {
            repo.type = "file";
        }
        else {
            repo.type = "unknown";
        }
        
        return repo;
    }
     
     
    bool has_find_package(Thirdparty thirdparty) {
        for(auto& package : cmake::get_find_package_list()) {
            if(package.name == thirdparty.name) return true;
        }
        return false;
    }
    
    void make_thirdparty_config(Thirdparty& library) {
        auto project = Cppm::instance()->project();
        std::ofstream config_file(project.path + "/cppm.yaml"); config_file.is_open();
        
        auto config = YAML::LoadFile(project.path+"/cppm.yaml");
        config["project"]["thirdparty"][library.name]["build-type"]       = library.build_type;
        config["project"]["thirdparty"][library.name]["install"]          = library.install_type;
        config["project"]["thirdparty"][library.name]["version"]          = library.version;
        config["project"]["thirdparty"][library.name]["url"]              = library.repo.url;
        config["project"]["thirdparty"][library.name]["find-cmake-value"] = library.cmake_var_name;
        
        config_file << config; 
        
        config_file.close();
    }
    
    void install_thirdparty(Thirdparty& library) {
        using namespace nieel::util;
        switch(hash(library.repo.type.c_str()))
        {
        case hash("git"): 
            git(library.repo.url);
            break;
        case hash("file"):
            tar(library.repo.url);
            break;
        case hash("link"):
            tar(wget(library.repo.url));
            break;
        }
        
    }
    
    void git(std::string& url) {
        auto project = Cppm::instance()->project();
        auto command = "    cd " + project.thirdparty 
                     + " && git clone " + url;
        system(command.c_str());
    }
    
    void svn(std::string& url) {
    }
    
    std::string wget(std::string& url) {
        return ("");
    } 
    
    void tar(std::string file) {
        
    }
    
}