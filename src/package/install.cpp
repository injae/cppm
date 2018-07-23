#include"cppm.h"
#include"package/install.h"
#include<nieel/util/hash.hpp>
#include<nieel/system.hpp>
#include<nieel/string.hpp>
#include<nieel/algorithm.hpp>

namespace cppm::package
{
    Install Install::parse(YAML::Node& node) {
        using namespace nieel;
        Install install;
        for(auto it : node["package"]["install-script"]) {
           auto property = it.first.as<std::string>();
           switch(hash(property))
           {
           case "linux"_h:
               for(auto script : node["package"]["install-script"]["linux"])
                   install.linux.push_back(script.as<std::string>());
               break;
           case "windows"_h:
               for(auto script : node["package"]["install-script"]["windows"])
                   install.windows.push_back(script.as<std::string>());
               break;
           }
        }
        return install;
    }
    
    void Install::install() {
        using namespace nieel;
        auto path = Cppm::instance()->project().path.thirdparty; 
        auto os = nieel::os::what();
        if(hash(os) == "linux"_h) {
            auto cmd = "cd " +path + nieel::str::accumulate(linux, {" && "});
            system(cmd.c_str());
        }
        else if(hash(os) == "windows"_h) {
            auto cmd = "cd " +path + nieel::str::accumulate(windows, {" && "});
            system(cmd.c_str());
        }
    }
}