#include "config/test.h"

namespace cppm
{
    void Test::parse(table_ptr table) {
        auto test = table->get_table("test");
        if(test) {
            is_testable = true;
            name = "test";
            install = false;
            auto source = test->get_array_of<std::string>("source");
            if(source) for(const auto& src : *source) { sources.push_back(src); }
            auto _deps = test->get_array_of<std::string>("dependencies");
            if(_deps) for(const auto& dep : *_deps) { deps.push_back(dep); } 
        }
    }

    std::string Test::generate() {
         std::string gen;
         return gen;
    }
}

