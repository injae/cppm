#include "cppm/core/config.hpp"
#include <range/v3/all.hpp>
#include <serdepp/adaptor/toml11.hpp>
#include "cppm/cppkg/cppkg.h"
#include "cppm/core/cppm_tool.hpp"

#include <serdepp/adaptor/nlohmann_json.hpp>

namespace cppm::core {

    Config Config::load(fs::path config_path) {
        using namespace fmt::literals;
        auto config = serde::deserialize<Config>(toml::parse((config_path/"cppm.toml").string()));
        config.post_processing(config_path.string());
        return config;
    }
    Config& Config::post_processing(const std::string& config_path) {
        using namespace ranges;
        using namespace fmt::literals;
        path = Path::make(config_path);
        if(!fs::exists(path.root/"cppm.toml")) { fmt::print(stderr, "can't find {} file", path.root.string()); }
        auto parse_source = [&](auto& source) {
            source = source
                    | views::transform([&](auto it){
                        auto files = util::find_files(path.root.string(), std::regex(it), false);
                        if(files.empty()) { fmt::print(stderr, "can't find {}\n", it); exit(1); }
                        return files; })
                    | views::cache1
                    | views::join
                    | to_vector;
        };

        if(lib) { parse_source(lib->source); }
        ranges::for_each(bins,       [&](auto& it) { parse_source(it.source); });
        ranges::for_each(examples,   [&](auto& it) { parse_source(it.source); });
        ranges::for_each(tests,      [&](auto& it) { parse_source(it.source); });
        ranges::for_each(benchmarks, [&](auto& it) { parse_source(it.source); });

        //--------

        auto load_cppkg = [&](auto& deps) {
            for(auto& [name, dep] : deps) {
                auto table = serde::serialize<toml::value>(dep);
                if(dep.optional) {
                    if(features.find(name) == features.end()) features[name] = {};
                    features[name].push_back({"$USE_{}_{}"_format(package.name, name)
                            | copy | actions::transform(::toupper)});
                }
                auto _path = path.thirdparty/"{}/{}/cppkg.toml"_format(name, dep.version);
                if(!fs::exists(_path) && dep.repo == repo_type::cppkg) {
                    cppkg::install(*this, cppkg::search(name, dep.version));
                }
                auto default_on = dep.default_features_flag;
                dep = {};
                serde::deserialize_to(toml::parse(_path)[name], dep);
                dep.default_feature = default_on ? dep.default_feature : decltype(dep.default_feature){};
                serde::deserialize_to(table, dep);
                dep.name = name;
                if(!dep.custom) {
                    auto _path = path.thirdparty/name/(dep.version);
                    install_cppm_download_package(_path, dep);
                }
            }
        };
        load_cppkg(dependencies);
        load_cppkg(dev_dependencies);

        for(auto& [name, tar] : target) {
            load_cppkg(tar.dependencies);
            load_cppkg(tar.dev_dependencies);
        }

        if(workspace) {
            std::map<std::string, Dependency> dep = workspace->member
                | views::transform([&](auto& it){ return Config::load(path.root/it); })
                | views::cache1
                | views::filter([](auto&& it){ return it.lib.has_value(); })
                | views::for_each([&](auto&& it) {
                    Dependency dep;
                    dep.name = it.lib->name;
                    dep.version = it.package.version;
                    dep.type = it.lib->type;
                    dep.module = it.lib->name;
                    dep.path.emplace(it.path.root.lexically_relative(path.root).generic_string());
                    dep.link = link_type::PUBLIC;
                    dep.repo = repo_type::workspace;
                    dep.optional = false; 
                    return yield(std::make_pair(it.lib.value().name, dep));
                }) | to<std::map<std::string, Dependency>>;

            ranges::insert(dependencies, dep);
        }


        return *this;
    }
    std::optional<Config> cppm_config_load(bool panic, const std::string& start_path) {
        fs::path start_position = (start_path == "") ? fs::current_path() : fs::path{start_path};
        auto path = cppm::util::reverse_find_file(start_position, "cppm.toml");
        if(!path) {
            if(panic) {
                fmt::print("can't find cppm.toml\n");
                exit(1);
            } else {
                fmt::print("can't find cppm.toml\nbuild with cppm_toolchain\n");
            }
            return std::nullopt;
        }
        return core::Config::load(path->parent_path());
    }
}
