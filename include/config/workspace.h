#ifndef __CPPM_CONFIG_WORKSPACE_H__
#define __CPPM_CONFIG_WORKSPACE_H__

#include "config/base_config.h"

namespace cppm
{
    class WorkSpace : public base_config
    {
    public:
        void parse(table_ptr table) override;
        std::string generate();
    private:
        std::vector<std::string> member;
    };
}

#endif
