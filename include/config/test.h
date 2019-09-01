#ifndef __CPPM_CONFIG_TEST_H__
#define __CPPM_CONFIG_TEST_H__

#include "config/base_config.h"
#include "config/base_target.h"

namespace cppm
{
    class Test : public base_config, base_target
    {
    public:
        void parse(table_ptr table);
        std::string generate();
    public:
        bool is_testable = false;
    };
}

#endif
