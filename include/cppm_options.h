#ifndef __CPPM_OPTIONS_H__
#define __CPPM_OPTIONS_H__

#include"option.h"
#include<string_view>

class CppmOptions : public Option
{
public:
    void run();
    CppmOptions(int argc, char* argv[]);
    void registe_subcommand(std::pair<std::string, std::string> command);

private:
    void _run();
    void _help();
    void _build();
    void _version();
    void _install();
    void _show_thirdparties();
    void _user_command(std::string_view cmd);
    
private:
    std::map<std::string, std::string> subcommand_;
};

#endif