#include<nieel/util/hash.hpp>
#include"options/install.h"
#include"url.h"
#include"utils.h"
#include<string>
#include"cmake/find_package.h"
#include"config/thirdparty.h"
#include"cppm.h"

namespace option {
    Install::Install(int argc, char* argv[]) : Option("Install option", argc, argv) {
        desc_.add_options()
        ("help,h" , "produce a help message")
        ("url,u"  , po::value<std::string>(), "url base install")
        ("git,g"  , po::value<std::string>(), "git base install")
        ("svn,s"  , po::value<std::string>(), "svn base install")
        ("file,f" , po::value<std::string>(), "file base install")
        ;
        visible_option_.add(desc_);
        desc_.add(nieel::make_command_desc());
    }
      
    void Install::run() {
       using namespace nieel::option; 
       using namespace nieel;
       sub_options_(type::option, "help" , opbind(_help)   , "show install command options")
                   (type::option, "url"  , opbind(_git)    , "")
                   (type::option, "git"  , opbind(_git)    , "")
                   (type::option, "svn"  , opbind(_svn)    , "")
                   (type::option, "file" , opbind(_file)   , "")
                   (type::default_command, uopbind(_config))
                   .run();
    }
    
    void Install::_help() {
       std::cout << "Usage: regex [options]\n"
                 << visible_option_
                 << sub_options_.command_description("Install")
                 << std::endl;
    }
    
    void Install::_git() {
        cppm::Thirdparty lib;
        lib.repo.url = vm_["git"].as<std::string>();
        lib.repo.type = "git";
        cppm::install_thirdparty(lib);
    }
    
    void Install::_svn() {
        cppm::Thirdparty lib;
        lib.repo.url = vm_["svn"].as<std::string>();
        lib.repo.type = "svn";
        cppm::install_thirdparty(lib);
    }
    
    void Install::_file() {
        cppm::Thirdparty lib;
        lib.repo.url = vm_["file"].as<std::string>();
        lib.repo.type = "link";
        cppm::install_thirdparty(lib);
    }
    
    void Install::_config(std::string& name) {
        auto thirdparties = Cppm::instance()->project().thirdparties;
        auto subargs = get_subarg();
        std::vector<cppm::Thirdparty> install_list;
        for(auto thirdparty : thirdparties) {
            if(util::has_str(name, "all")) {
                install_list = thirdparties;  break;
            }
            else if(util::has_str(name, thirdparty.name.c_str())) {
               install_list.push_back(thirdparty);
            }
        }
        for(auto& library : install_list) {
            cppm::install_thirdparty(library);
        }
    }
    
    void Install::_url() {
        auto subarg = vm_["url"].as<std::string>();
        if(parse_url(subarg)) {
            cppm::Thirdparty lib;
            lib.repo = cppm::Repository::classificate(subarg);
            cppm::install_thirdparty(lib);
        }
    }


}