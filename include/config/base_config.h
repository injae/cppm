#ifndef __CPPM_CONFIG_BASE_CONFIG_H__
#define __CPPM_CONFIG_BASE_CONFIG_H__

#include <cpptoml.h>
#include <memory.h>
#include <string>

namespace cppm
{
    class base_config
    {
    protected:
        using table_ptr = std::shared_ptr<cpptoml::table>;
    public:
        virtual void parse(table_ptr table) = 0;
        virtual std::string generate() = 0;
    };
}


#endif
