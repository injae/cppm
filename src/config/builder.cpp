#include "config/builder.h"

namespace cppm
{
    void Builder::parse(table_ptr table) {
        auto builders = table->get_table("builder");
        for(auto& builder_table : *builders) {
            BuilderType builder_t; 
            builder_t.name = builder_table.first;
            auto builder = builders->get_table(builder_t.name);
            builder_t.option     = *builder->get_as<std::string>("option");
            list[builder_t.name] = builder_t;
        }
    }
}
