#include "config/bin.h"
#include "util/filesystem.h"
#include "util/algorithm.hpp"
#include "config/config.h"
#include <string>
#include "util/cmake.h"
#include <fmt/format.h>

using namespace fmt::literals;

namespace cppm
{
    void Bins::parse(table_ptr table) {
        auto table_array = table->get_table_array("bin");
        if(!table_array) return;
        for(const auto& bin_table : *table_array) {
            Bin bin;
            bin.name = *bin_table->get_as<std::string>("name");
            bin.install = bin_table->get_as<bool>("install").value_or(true);
            auto source = bin_table->get_array_of<std::string>("source");
            if(source) for(const auto& src : *source) { bin.sources.push_back(src); }
            auto deps = bin_table->get_array_of<std::string>("dependencies");
            if(deps) for(const auto& dep : *deps) { bin.deps.push_back(dep); } 
            list.emplace_back(std::move(bin));
        }
    }

    std::string Bins::generate(Config& config) {
        using namespace util::cmake;

        std::string gen;
        for(const auto& bin : list) {
            std::vector<std::string> sources;
            for(const auto& src : bin.sources) {
                auto result = util::find_files(config.path.root, std::regex(src), false);
                sources.insert(sources.end(), result.begin(), result.end());
            }
            gen += "\n\ncppm_target_install({0} {1} {2}\nSOURCES {3}\n)\n"_format(bin.name, "BINARY", bin.install ? "INSTALL":"", util::accumulate(sources, "\n\t"));
        }
        return gen;
    }

    //std::string Bins::generate_define(Config& config) {
    //    std::string gen;
    //    for(const auto& bin : list) {
    //        std::vector<std::string> sources;
    //        for(const auto& src : bin.sources) {
    //            auto result = util::find_files(config.path.root, std::regex(src), false);
    //            sources.insert(sources.end(), result.begin(), result.end());
    //        }
    //        gen += "\n\ncppm_target_define({0} {1} \nSOURCES {2}\n)\n"_format(
    //               bin.name, "BINARY", util::accumulate(sources, "\n\t"));
    //    }
    //    return gen;
    //}

    //std::string Bins::generate_dependencies(Config& config) {
    //    std::string gen;
    //    for(auto& bin : list) {
    //        if(bin.deps.empty()) {
    //            for(auto& g_dep : config.dependencies.list) {
    //                bin.deps.push_back(g_dep.first);
    //            }
    //        }
    //        std::vector<std::string> pub_deps;
    //        std::vector<std::string> pri_deps;
    //        std::vector<std::string> int_deps;
    //        for(const auto& dep : bin.deps) {
    //            auto g_dep = config.dependencies.list[dep];
    //                 if(g_dep.link_type == "public")    pub_deps.push_back(g_dep.module);
    //            else if(g_dep.link_type == "private")   pri_deps.push_back(g_dep.module);
    //            else if(g_dep.link_type == "interface") int_deps.push_back(g_dep.module);
    //        }
    //        auto trans = [&](std::string title, std::vector<std::string> list){
    //                         return "\n{0}\n\t{1}"_format(title,util::accumulate(list, "\n\t"));
    //                     };
    //        std::string gen_dep; 
    //        if(!pub_deps.empty()) gen_dep += trans("PUBLIC"   , pub_deps);
    //        if(!pri_deps.empty()) gen_dep += trans("PRIVATE"  , pri_deps);
    //        if(!int_deps.empty()) gen_dep += trans("INTERFACE", int_deps);
    //        gen += "\n\ncppm_target_dependencies({0} {1}\n)\n"_format(
    //               bin.name, gen_dep);
    //    }
    //    return gen;
    //}

    //std::string Bins::generate_install(Config& config) {
    //    std::string gen;
    //    return gen;
    //}
    // std::string Bins::generate(Config& config) {
    //     using namespace fmt::literals;
    //     using namespace util::cmake;
    //     std::string gen;
    //     for(const auto& bin : list) {
    //         std::vector<std::string> sources;
    //         for(const auto& src : bin.sources) {
    //             auto result = util::find_files(config.path.root, std::regex(src), false);
    //             sources.insert(sources.end(), result.begin(), result.end());
    //         }
    //         //gen += "target_sources({0}\n\tPRIVATE)"_format(bin.name, util::accumulate(sources, "\n\t"));
    //         gen += "set({0}_source {1}\n)"_format(bin.name, util::accumulate(sources, "\n\t\t"));
    //         gen += "\n\nbuild_binary({0} \"{1}\" \"{2}\")"_format( bin.name
    //                                                             ,var(bin.name+"_source")
    //                                                             ,var("thirdparty"));
    //     }
    //     return gen;
    // }
}
