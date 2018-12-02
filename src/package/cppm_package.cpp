#include "package/cppm_package.h"

namespace cppm
{
    void CppmPackage::parse(table_ptr table) {
        if(auto config = table->get_table("config")) {
            auto source = config->get_array_of<std::string>("tool_chains");
            if(source) for(const auto& src : tool_chains) {tool_chains.push_back(src);} 
        }
    }
}
