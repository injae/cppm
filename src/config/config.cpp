#include "config/config.h"

namespace cppm
{
    Config Config::load(const std::string &path) {
        auto table = cpptoml::parse_file(path);
        Config config;
        config.parse(table);

        return config; 
    }

    void Config::parse(table_ptr table) {
        package.parse(table);
        bins.parse(table);
    }
    
    std::string Config::generate() {
        std::string gen;

        return gen; 
    }
    
}
