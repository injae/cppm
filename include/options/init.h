#ifndef __OPTIONS_INIT_H__
#define __OPTIONS_INIT_H__

#include"options/option.h"

namespace option
{
    class Init : public Option
    {
    public:
        void run();
        Init(int argc, char* argv[]);
    private:
        void _bin();
        void _lib(std::string type);
        void _help();
    };
}


#endif