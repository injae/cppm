#include "config/cmake.h"

namespace cppm
{
    void Cmake::parse(table_ptr table)
    {
        auto cmake = table->get_table("cmake");
        name       = *cmake->get_as<std::string>("name");
        version    = *cmake->get_as<std::string>("version");
        option     = *cmake->get_as<std::string>("option");
        builder    = *cmake->get_as<std::string>("builder");
        compiler   = *cmake->get_as<std::string>("compiler");
        auto includes = cmake->get_array_of<std::string>("include");
        if(includes) for(const auto& inc : *includes) {include.push_back(inc);} 
    }
}
