#include <fmt/format.h>
#include "config/bin.h"
#include "util/algorithm.hpp"


namespace cppm
{
    void Bins::parse(table_ptr table) {
        for(const auto& bin_table : *table->get_table_array("bin")) {
            Bin bin;
            bin.name = *bin_table->get_as<std::string>("name");
            for(const auto& src : *bin_table->get_array_of<std::string>("source"))
                bin.sources.push_back(src);
            list.emplace_back(std::move(bin));
        }
    }

    std::string Bins::generate() {
        using namespace fmt::literals;
        std::string gen;
        for(const auto& bin : list) {
            gen += "(set {0}_source {1})"_format(bin.name, util::accumulate(bin.sources, "\n"));
        }
    }
}
