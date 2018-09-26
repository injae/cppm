#include "config/package.h"

namespace cppm
{
    void Package::parse(table_ptr table) {
        auto package = table->get_table("package");
        name         = *package->get_as<std::string>("name");
        version      = *package->get_as<std::string>("version");
        description  = *package->get_as<std::string>("description");
    }
    
    std::string Package::generate() {
        std::string gen;
        
        return gen;
    }
}
