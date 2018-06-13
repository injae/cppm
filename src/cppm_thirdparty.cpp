#include"cppm_thirdparty.h"
#include"utils.h"
#include"cmake/generator.h"
#include"cmake/find_package.h"
#include"cppm.h"
#include"url.h"


namespace cppm 
{
    Repository classificate_repo(std::string url) {
        using namespace util;
        Url parsed = parse_url(url);
        Repository repo; 
        repo.url = url;
        if(   has_str(parsed.protocol, "http") 
           || has_str(parsed.protocol, "https")) 
        {
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
        else {
            if(   has_str(parsed.path, "tar")
               || has_str(parsed.path, "zip")
               || has_str(parsed.path, "gz"))
            {
                repo.type = "file";
            }
            else {
                repo.type = "unknown";
            }
        }
        
        return repo;
    }
     
     
    bool has_find_package(Thirdparty thirdparty) {
        for(auto& package : cmake::get_find_package_list()) {
            if(package.name == thirdparty.name) return true;
        }
        return false;
    }
    
    
    void install_thirdparty(Thirdparty& library) {
        using namespace util;
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