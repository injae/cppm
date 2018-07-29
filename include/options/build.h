#ifndef __CPPM_OPTION_BUILD_H__
#define __CPPM_OPTION_BUILD_H__

#include<nieel/program_option.h>
namespace option
{
    class Build : public nieel::Option
    {
    public:
        void run();
        Build(int argc, char* argv[]);
    private:
        void _update();
        void _default();
        void _help();
    };
}

#endif 