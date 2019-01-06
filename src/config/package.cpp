#include "config/package.h"
#include <iostream>
#include <fmt/format.h>

namespace cppm
{
    void Package::parse(table_ptr table) {
        auto package = table->get_table("package");
        if(!package) {
            std::cerr << "cppm.toml need package tables";
            exit(1);
        }
        name         = *package->get_as<std::string>("name");
        version      = package->get_as<std::string>("version").value_or("0.0.1");
        description  = package->get_as<std::string>("description").value_or("");
    }

}
