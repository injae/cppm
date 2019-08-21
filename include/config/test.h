#ifndef __CPPM_CONFIG_TEST_H__
#define __CPPM_CONFIG_TEST_H__

#include "config/base_config.h"

namespace cppm
{

    class Test : public base_config
    {
    public:
        void parse(table_ptr table);
        std::string generate();
    public:
    };
}


#endif
