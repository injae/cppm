#include"package/build.h"
#include"cppm.h"
#include<nieel/util/hash.hpp>
#include<nieel/string.hpp>

namespace cppm::package
{
    void Build::build() {
        using namespace nieel;
        auto path = Cppm::instance()->project().path.thirdparty; 
        std::string cmd;
        if(!global.empty()) 
            cmd = "cd " + path + str::accumulate(global, {" && "});
        else if(!local.empty())
            cmd = "cd " + path + str::accumulate(local, {" && "});
        else 
            return ;
        system(cmd.c_str());
    }
    
    Build Build::parse(YAML::Node& node) {
        using namespace nieel;
        Build build;
        for(auto it : node["package"]["build"]) {
            auto property = it.first.as<std::string>();
            switch(hash(property))
            {
            case hash("type"):
                build.type = it.second.as<std::string>();
                break;
            case hash("local"):
                for(auto script : node["package"]["build"]["local"])
                    build.local.push_back(script.as<std::string>());
                break;
            case hash("global"):
                for(auto script : node["package"]["build"]["global"])
                    build.global.push_back(script.as<std::string>());
                break;
            }
        }
        return build;
    }
}