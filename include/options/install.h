#ifndef __OPTIONS_INSTALL_H__
#define __OPTIONS_INSTALL_H__

#include<nieel/program_option.h>

namespace option
{
    class Install: public nieel::Option
    {
    public:
        void run();
        Install(int argc, char* argv[]);
    private:
        void _help();
        void _config(std::string& name);
        void _url();
        void _git(); 
        void _svn();
        void _file();
        
    };
}



#endif