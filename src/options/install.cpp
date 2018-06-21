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
        ("help,h", "produce a help message")
        ("url,u" , po::value<std::string>(), "url base install")
        ;
        visible_option_.add(desc_);
        desc_.add(nieel::make_command_desc());
    }
      
    void Install::run() {
             if(vm_.count("help")) _help();
        else if(vm_.count("url"))  _url_base();
        else if(vm_.count("command")) { auto cmd = vm_["command"].as<std::string>(); 
            std::cout << cmd << std::endl;
            _config_base(cmd);
        }
    }
    
    void Install::_help() {
        
    }
    
    void Install::_config_base(std::string& name) {
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
    
    void Install::_url_base() {
        auto subarg = vm_["url"].as<std::string>();
        if(parse_url(subarg)) {
            cppm::Thirdparty lib;
            lib.repo = cppm::Repository::classificate(subarg);
            cppm::install_thirdparty(lib);
        }
    }


}