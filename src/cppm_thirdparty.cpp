#include"cppm_thirdparty.h"
#include"utils.h"
#include"cmake_generator.h"
#include"cppm.h"
#include<boost/filesystem.hpp>
#include<fstream>

namespace fs = boost::filesystem;


namespace cppm 
{
    Repository classificate_repo(std::string url) {
        //using namespace boost::network;
        //uri::uri uri(uri);
        //std::cout << uri.scheme() << "\n"
        //          << uri.host()   << "\n"
        //          << uri.path()   << "\n"
        //          ;
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
        cmake::make_find_library(library.name, library.build_type);
        auto project = Cppm::instance()->project();
        auto file_name = project.cmake_find_module + "/Find"+library.name+".cmake";
        std::ofstream file(file_name); file.is_open();
    }
    
    void git(std::string& url) {
       std::string cmd("git clone "+url);
       system(cmd.c_str());
    }
    
    void svn(std::string& url) {
        
    }
    
    std::string wget(std::string& url) {
        return ("");
    } 
    
    void tar(std::string file) {
        
    }
    
}