#include "config/cmake.h"
#include <fmt/format.h>

namespace cppm
{
    void Cmake::parse(table_ptr table)
    {
        if(auto cmake = table->get_table("cmake")) {
            version    = *cmake->get_as<std::string>("version");
            option     = *cmake->get_as<std::string>("option");
            builder    = *cmake->get_as<std::string>("builder");
            compiler   = *cmake->get_as<std::string>("compiler");
            auto includes = cmake->get_array_of<std::string>("include");
            if(includes) for(const auto& inc : *includes) {include.push_back(inc);} 
        }
    }

    std::string Cmake::generate() {
        using namespace fmt::literals;
        std::string gen = "\n";
        for(auto inc : include) gen += "include({0})\n"_format(inc);
        return gen; 
    }
}
