#include"option.h"
#include<string>
#include<vector>

po::options_description make_command_desc() {
    po::options_description desc("Command Options");
    desc.add_options()
        ("command"       , po::value<std::string>()             , ""    )
        ("subargs"       , po::value<std::vector<std::string>>(), ""    )
        ;
   return desc; 
}

po::positional_options_description make_command_option() {
    po::positional_options_description option;
    option.add("command" , 1);
    option.add("subargs" ,-1);
    return option; 
}

void Option::regist() {
    po::store(make_parser(), vm_);
    po::notify(vm_);
}

void Option::regist(std::vector<std::string>& options) {
    po::store(make_parser(options), vm_);
    po::notify(vm_);
}

parser_type Option::make_parser() {
    return  po::command_line_parser(argc_, argv_)
           .options(desc_)
           .positional(make_command_option())
           .allow_unregistered()
           .run();
}

parser_type Option::make_parser(std::vector<std::string>& options) {
    return  po::command_line_parser(options)
           .options(desc_)
           .positional(make_command_option())
           .allow_unregistered()
           .run();
}

std::vector<std::string> Option::get_subarg() {
   if(!vm_.count("subargs")) return std::vector<std::string>();
   return vm_["subargs"].as<std::vector<std::string>>();
}