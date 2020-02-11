#include "config/bin.h"
#include "util/filesystem.h"
#include "util/algorithm.hpp"
#include "config/config.h"
#include <string>
#include "cmake/cmake.h"
#include <fmt/format.h>

using namespace fmt::literals;

namespace cppm
{
    void Bins::parse(table_ptr table) {
        toml::get_table_array(table, "bin", [&](auto tb) {
             Bin bin; 
             bin.name    = toml::panic(tb, "name");
             bin.install = toml::get(tb, "install", true);
             toml::get_array(tb, "source"      , bin.sources);
             toml::get_array(tb, "dependencies", bin.deps);
             list.emplace_back(std::move(bin));
        });
    }

    std::string Bins::generate(Config& config) {
        using namespace cmake;

        std::string gen;
        for(const auto& bin : list) {
            std::vector<std::string> sources;
            for(const auto& src : bin.sources) {
                auto result = util::find_files(config.path.root, std::regex(src, std::regex_constants::grep), false);
                sources.insert(sources.end(), result.begin(), result.end());
            }
            gen += "\n\ncppm_target_install({0} {1} {2}\nSOURCES {3}\n)\n"_format(bin.name, "BINARY", bin.install ? "INSTALL":"", util::accumulate(sources, "\n\t"));
        }
        return gen;
    }

}
