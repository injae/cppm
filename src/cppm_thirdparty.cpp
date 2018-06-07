#include"cppm_thirdparty.h"
#include"utils.h"
#include"cmake_generator.h"
#include"cppm.h"
#include<boost/filesystem.hpp>
#include<fstream>
#include"url.h"
#include"boost/tokenizer.hpp"
#include"boost/algorithm/string/replace.hpp"

namespace fs = boost::filesystem;

namespace cppm 
{
    Repository classificate_repo(std::string url) {
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
    
    
    void install_thirdparty(Thirdparty& library) {
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
        
        auto project = Cppm::instance()->project();
        auto file_name = project.cmake_find_module + "/Find"+library.name+".cmake";
        std::ofstream file(file_name); file.is_open();
        file <<  cmake::make_find_library(library.name, library.build_type);
        file.close();
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