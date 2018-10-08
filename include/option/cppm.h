#ifndef __CPPM_OPTION_CPPM_H__
#define __CPPM_OPTION_CPPM_H__

#include "option/base_option.h"
#include "config/config.h"

namespace cppm::option
{
    class Cppm : public base_option
    {
    public:
        Cppm();
        void run(int argc, char* argv[]);
    private:
        void config_load();
        void generate_cmakelists();
    private:
        Config config_;
    };
}


#endif 
