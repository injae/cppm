#ifndef __CPPM_OPTIONS_H__
#define __CPPM_OPTIONS_H__

#include<nieel/program_option.h>
#include<string_view>

class CppmOptions : public nieel::Option
{
public:
    void run();
    CppmOptions(int argc, char* argv[]);
    void registe_subcommand(std::pair<std::string, std::string> command);

private:
    void _run();
    void _help();
    void _init();
    void _build();
    void _version();
    void _install();
    void _show_thirdparties();
    void _get_cmake_lib_hint();
    void _make_cmake_find_lib_file();
    void _user_command(std::string_view cmd);
    
private:
    std::map<std::string, std::string> subcommand_;
};

#endif