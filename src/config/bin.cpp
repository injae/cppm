#include "config/bin.h"

namespace cppm
{
    void Bins::parse(table_ptr table) {
        for(const auto& bin_table : *table->get_table_array("bin")) {
            Bin bin;
            bin.name = *bin_table->get_as<std::string>("name");
            bins.emplace_back(std::move(bin));
        }
    }
    
    std::string Bins::generate() {
        std::string gen;

        return gen;
    }
}
