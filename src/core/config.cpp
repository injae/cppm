#include <cppm/core/config.hpp>
#include <range/v3/all.hpp>
#include <cppm/util/filesystem.h>
#include <cppm/core/cppm_tool.hpp>
#include <cppm/cppkg/cppkg.h>
#include <serdepp/serializer.hpp>
#include <toml.hpp>
#include <serdepp/adaptor/toml11.hpp>
#include "cppm/util/toml.hpp"

namespace cppm::core {
    using dep_map = std::optional<std::map<std::string, Dependency>>;


    std::optional<Config> Config::load(const std::string& path) {
        using namespace ranges;
        using namespace fmt::literals;
        using namespace serde;
        std::optional<Config> config;
        Path p = Path::make(path);
        if(!fs::exists(p.root/"cppm.toml")) { fmt::print("can't find {} file", p.root.string()); }
        config = parse_file_and_serde<toml::value, Config>((p.root/"cppm.toml").string());
        config->path = p;
        // cppkg parse sources
        auto parse_source = [&config](auto& source) {
             if(source) {
                source = *source
                       | views::transform([&config](auto it){
                              auto files = util::find_files(config->path.root.string(), std::regex(it), false);
                              if(files.empty()) { fmt::print(stderr, "can't find {}\n", it); exit(1); }
                              return files; })
                       | views::cache1
                       | views::join
                       | to_vector;
             }
        };

        if(config->lib) {
            parse_source(config->lib->source);

            if (*config->lib->cppkg_type == "header-only" && config->lib->source) {
              fmt::print(stderr, "header-only library can't have sources\n");
              exit(1);
            }
        }
        if(config->bins)     ranges::for_each(*config->bins, [&parse_source](auto& it) { parse_source(it.source); });
        if(config->tests)    ranges::for_each(*config->tests, [&parse_source](auto& it) { parse_source(it.source); });
        if(config->examples) ranges::for_each(*config->examples, [&parse_source](auto& it) {parse_source(it.source);});

        // dependency <- load cppkg package thirdparty/{name}/{version}
        auto table = parse_file<toml::value>((p.root/"cppm.toml").string());
        auto load_cppkg = [&config,table](auto& target) {
            if(target) {
                auto deps = target.value();
                auto& thirdparty = config->path.thirdparty;
                auto paths = deps >>= [thirdparty,&config](auto& it) {
                    auto& [name, dep] = it;
                    if(*dep.optional) {
                        (*config->features)[name].member.emplace_back("$USE_{}_{}"_format(config->package.name, name)   
                                                               | copy | actions::transform(::toupper));
                    }
                                                                  
                    auto path = thirdparty/"{}/{}/cppkg.toml"_format(dep.name, dep.version);
                    if(!fs::exists(path) && *dep.repo == "cppkg") {
                        cppkg::install(*config, cppkg::search(dep.name, dep.version.value()));
                    }
                    return yield_if(fs::exists(path), path.string());
                };
                ranges::for_each(paths, [&target](auto& it) {
                    auto parsed = parse_file<toml::value>(it);
                     serialize_to(parsed , target.value()); 
                });
                ranges::for_each(target.value(), [&config](auto& it) {
                     auto& [_, dep] = it;
                     if(!*dep.custom) {
                         auto path = config->path.thirdparty/dep.name/(*dep.version);
                         install_cppm_download_package(path,dep);
                     }
                });
            }
        };

        load_cppkg(config->dependencies);
        load_cppkg(config->dev_dependencies);
        if(config->target) {
            ranges::for_each(config->target.value(), [&load_cppkg](auto& it) {
                auto& [name, target] = it;
                load_cppkg(target.dependencies);
                load_cppkg(target.dev_dependencies);
            });
        }
        // workspace -> dependencies
        if(config->workspace) {
            auto members = config->workspace->member.value();
            auto& root = config->path.root;
            auto confs  = members
                        | views::transform([root](auto it) { return Config::load((root/it).string()); })
                        | to_vector;
            auto dep = confs
                     | views::filter([](auto it) { return it->lib.has_value(); })
                     | views::for_each([&root](auto it) {
                         Dependency dep;
                         dep.name    = it->lib->name;
                         dep.version = it->package.version;
                         dep.type    = it->lib->type;
                         dep.module  = it->lib->name;
                         dep.path    = it->path.root.lexically_relative(root).generic_string();
                         dep.link    = "public";
                         dep.repo    = "workspace";
                         return yield(std::make_pair(dep.name, dep));})
                | to<std::map>;
             
            if(not config->dependencies) { config->dependencies = dep_map(); }
            ranges::insert(config->dependencies.value(),dep);
        }

        // features
        //for(auto& [key, feature] : *(config->features)) {
        //    //            fmt::print("--{}",key);
        //    if(key == "default") continue;
        //    for(auto& value : feature.member) {
        //        if(value[0] == '$') { // cmake value
        //            //feature.key = value; feature.value="OFF"; 
        //            //fmt::print("= {}\n", value);
        //        }
        //        else {
        //            if(auto idx = value.find_first_of('/'); idx != std::string::npos) {
        //                auto src = value.substr(0,idx);
        //                auto feature_key = value.substr(idx+1);
        //                if(config->dependencies->find(src) != config->dependencies->end()) {
        //                    if(auto& target = config->dependencies.value()[src].features) {
        //                         feature.key = (*target)[feature_key].member[0];
        //                         feature.value = "OFF";
        //                    } else {
        //                        fmt::print(stderr, "undefiend features: {}\n", feature); exit(1);
        //                    }
        //                } else {
        //                    fmt::print(stderr, "undefiend dependencies: {}\n", src); exit(1);
        //                }
        //            } 
        //                fmt::print("= {}\n", value);
        //            }
        //        }
        //    }
        //    //fmt::print("\n");
        //}

        if(config->features->find("default") != config->features->end()) {
            
        }

        return config;
    }

}
