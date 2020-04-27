#include <cppm/core/config.hpp>
#include <range/v3/all.hpp>
#include <cppm/util/filesystem.h>
#include <cppm/core/cppm_tool.hpp>
#include <cppm/cppkg/cppkg.h>


namespace cppm::core {
    std::optional<Config> Config::load(const std::string& path) {
        using namespace ranges;
        using namespace fmt::literals;
        opt<Config> config;
        Path p = Path::make(path);
        if(!fs::exists(p.root/"cppm.toml")) { fmt::print("can't find {} file", p.root.string()); }
        toml::orm::parser(config, (p.root/"cppm.toml").string());
        //std::cout << *table << std::endl;
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
            if (*config->lib->cppkg_type == "header-only" &&
                config->lib->source) {
              fmt::print(stderr, "header-only library can't have sources\n");
              exit(1);
            }
        }
        if(config->bins) ranges::for_each(*config->bins, [&parse_source](auto& it) { parse_source(it.source); });
        if(config->tests) ranges::for_each(*config->tests, [&parse_source](auto& it) { parse_source(it.source); });
        if(config->examples) ranges::for_each(*config->examples, [&parse_source](auto& it) {parse_source(it.source);});

        // dependency <- load cppkg package thirdparty/{name}/{version}
        auto table = toml::parse_file((p.root/"cppm.toml").string());
        auto load_cppkg = [&config,table](opt<nested<Dependency>>& target, std::string name) {
            if(target) {
                auto deps = target.value();
                auto& thirdparty = config->path.thirdparty;
                auto paths = deps >>= [thirdparty,&config](auto& it) {
                    auto& [name, dep] = it;
                    auto path = thirdparty/"{}/{}/cppkg.toml"_format(dep.name, dep.version);
                    if(!fs::exists(path)) { cppkg::install(*config, cppkg::search(dep.name, *dep.version)); }
                    return yield_if(fs::exists(path), path.string());
                };
                ranges::for_each(paths, [&target](auto& it) { toml::orm::parser(target, it); });
                toml::orm::parser(target, table->get_table(name));
                ranges::for_each(*target, [&config](auto& it) {
                     auto& [_, dep] = it;
                     if(!*dep.custom) {
                         auto path = config->path.thirdparty/dep.name/(*dep.version);
                         install_cppm_download_package(path,dep);
                     }
                });
            }
        };

        load_cppkg(config->dependencies, "dependencies");
        load_cppkg(config->dev_dependencies, "dev-dependencies");

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
                         dep.path    = it->path.root.lexically_relative(root).string();
                         dep.link    = "public";
                         dep.repo    = "workspace";
                         return yield(std::make_pair(dep.name, dep));})
                | to<std::map>;
             
            if(not config->dependencies) { config->dependencies = opt<nested<Dependency>>(); }
            ranges::insert(config->dependencies.value(),dep);
        }
        return config;
    }

}
