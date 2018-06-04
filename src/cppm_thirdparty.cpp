#include"cppm_thirdparty.h"
#include"utils.h"

namespace cppm 
{
    Repository classificate_repo(std::string url) {
        
    }
    
    void install_thirdparty(Thirdparty& library) {
        switch(hash(library.repo.type.c_str()))
        {
        case hash("git"):
            break;
        case hash("file"):
            break;
        case hash("link"):
            break;
        }
    }
    
    void git_clone() {
        
    }
    
    void wget() {
        
    } 
    
    void tar() {
        
    }
    
}