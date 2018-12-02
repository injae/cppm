#ifndef __CPPM_OPTION_BUILDER_H__
#define __CPPM_OPTION_BUILDER_H__

#include "config/base_config.h"
#include <map>
#include <string>

namespace cppm
{
    struct BuilderType 
    {
        std::string name;
        std::string option;
    };

    class Builder : public base_config
    {
    public:
        void parse(table_ptr table);
    public:
        std::map<std::string, BuilderType> list;
    };
}



#endif
