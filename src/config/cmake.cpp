#include "config/cmake.h"

namespace cppm
{
    void Cmake::parse(table_ptr table)
    {
        auto cmake = table->get_table("cmake");
        name       = *cmake->get_as<std::string>("name");
        version    = *cmake->get_as<std::string>("version");
        option     = *cmake->get_as<std::string>("option");
    }
}
