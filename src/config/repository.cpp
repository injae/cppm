#include"config/repository.h"
#include<nieel/util/hash.hpp>
#include<nieel/string.hpp>
#include"url.h"
#include"cppm.h"

namespace cppm
{
    Repository Repository::classificate(std::string url) {
        using namespace nieel;
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
    
    void git(std::string& url) {
        auto project = Cppm::instance()->project();
        auto command = "    cd " + project.path.thirdparty 
                     + " && git clone " + url;
        system(command.c_str());
    }
    
    void svn(std::string& url) {
        auto project = Cppm::instance()->project();
        auto command = "    cd " + project.path.thirdparty 
                     + " && svn co " + url;
        system(command.c_str());
    }
    
    std::string wget(std::string& url) {
        auto project = Cppm::instance()->project();
        auto command = "    cd " + project.path.thirdparty 
                     + " && wget " + url;
        system(command.c_str());
        return command;
    } 
    
    void tar(std::string file) {
        auto project = Cppm::instance()->project();
        auto command = "    cd " + project.path.thirdparty 
                     + " && tar -zxvf " + file;
        system(command.c_str());
    }
}