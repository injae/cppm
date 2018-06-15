#ifndef __OPTION_H__
#define __OPTION_H__

#include<boost/program_options/options_description.hpp>
#include<boost/program_options/variables_map.hpp>
#include<boost/program_options/parsers.hpp>

namespace po = boost::program_options;
typedef po::basic_parsed_options<char> parser_type;

class Option
{
public:
    virtual void                    run() = 0;
            void                    regist();
            void                    regist(std::vector<std::string>& options);
            parser_type             make_parser();
            parser_type             make_parser(std::vector<std::string>& options); 
            po::options_description description() { return desc_; }
            std::vector<std::string> get_subarg();
             
        
    Option(const std::string descripte, int argc, char* argv[]) 
        : desc_(descripte), visible_option_(desc_), argc_(argc), argv_(argv) {}
    virtual ~Option() {}
    
protected:
    po::variables_map vm_;
    po::options_description desc_;
    po::options_description visible_option_;
    int    argc_;
    char** argv_;
};

po::options_description            make_command_desc();
po::positional_options_description make_command_option();

#endif