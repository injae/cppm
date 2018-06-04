#ifndef __CPPM_OPTIONS_H__
#define __CPPM_OPTIONS_H__

#include"option.h"

class CppmOptions : public Option
{
public:
    void run();
    CppmOptions(int argc, char* argv[]);
    void registe_subcommand(std::pair<std::string, std::string> command);

private:
    void _help();
    void _version();
    void _build();
    void _run();
    
private:
    std::map<std::string, std::string> subcommand_;
};

#endif