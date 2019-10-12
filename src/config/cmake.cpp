#include "config/cmake.h"
#include <fmt/format.h>

namespace cppm
{
    void Cmake::parse(table_ptr table)
    {
        auto cmake = table->get_table("cmake");
        if(!cmake) cmake = cpptoml::make_table();
        version    = cmake->get_as<std::string>("version" ).value_or("3.15");
        option     = cmake->get_as<std::string>("option"  ).value_or("");
        builder    = cmake->get_as<std::string>("builder" ).value_or("make");
        compiler   = cmake->get_as<std::string>("compiler").value_or("none");
        auto tool_chains = cmake->get_array_of<std::string>("toolchain");
        if(tool_chains) for(const auto& tool : *tool_chains) {tool_chain.push_back(tool);} 
        auto includes = cmake->get_array_of<std::string>("include");
        if(includes) for(const auto& inc : *includes) {include.push_back(inc);} 
    }

    std::string Cmake::generate() {
        using namespace fmt::literals;
        std::string gen = "\n";
        for(auto inc : include) gen += "include({0})\n"_format(inc);
        return gen; 
    }
}
