#include "config/compiler.h"
#include <fmt/format.h>

namespace cppm
{
    void Compiler::parse(table_ptr table) {
        auto add_option = [&](table_ptr p_table, auto name, bool is_table) -> CompilerType {
            CompilerType compiler_t;
            compiler_t.name = name;
            if(is_table) {
                auto compiler = p_table->get_table(compiler_t.name);
                compiler_t.option  = compiler->get_as<std::string>("option").value_or("");
                compiler_t.version = compiler->get_as<std::string>("version").value_or("");
            }
            else {
                compiler_t.option = p_table->get_as<std::string>(compiler_t.name).value_or("");
                compiler_t.version = "";
            }
            return compiler_t;
        };
        auto compilers = table->get_table("compiler");
        if(!compilers) return;
        for(auto& compiler_table : *compilers) {
            if(compiler_table.first == "debug") {
                if(auto debug_table = compilers->get_table("debug")) {
                    for(auto& debug_compiler : *debug_table){
                        auto c_name = debug_compiler.first;
                        auto is_table = debug_compiler.second->is_table();
                        debug_list[c_name] = add_option(debug_compiler.second,c_name, is_table);
                    }
                }
            }
            else if(compiler_table.first == "release") {
                if(auto release_table = compilers->get_table("release")) {
                    for(auto& release_compiler : *release_table){
                        auto c_name = release_compiler.first;
                        auto is_table = release_compiler.second->is_table();
                        release_list[c_name] = add_option(release_compiler.second,c_name, is_table);
                    }
                }
            }
            else {
                auto c_name = compiler_table.first;
                auto is_table = compiler_table.second->is_table();
                debug_list[c_name] = add_option(compilers,c_name, is_table);
                release_list[c_name] = add_option(compilers,c_name, is_table);
            }
        }
    }
    std::string Compiler::generate() {
        using namespace fmt::literals;
        std::string o_str = "";
        if(!debug_list.empty()) {
            o_str += "\n\tDEBUG\n";
            if(debug_list.find("clang") != debug_list.end()) {
                o_str += "\t\tCLANG \"{}\"\n"_format(debug_list["clang"].option);
            }
            if(debug_list.find("gcc") != debug_list.end()) {
                o_str += "\t\tGCC   \"{}\"\n"_format(debug_list["gcc"].option);
            }
            if(debug_list.find("msvc") != debug_list.end()) {
                o_str += "\t\tMSVC  \"{}\"\n"_format(debug_list["msvc"].option);
            }
        }
        std::string r_str = "";
        if(!release_list.empty()) {
            if(debug_list.empty()) o_str += "\n";
            o_str += "\tRELEASE\n";
            if(release_list.find("clang") != release_list.end()) {
                o_str += "\t\tCLANG \"{}\"\n"_format(release_list["clang"].option);
            }
            if(release_list.find("gcc") != release_list.end()) {
                o_str += "\t\tGCC   \"{}\"\n"_format(release_list["gcc"].option);
            }
            if(release_list.find("msvc") != release_list.end()) {
                o_str += "\t\tMSVC  \"{}\"\n"_format(release_list["msvc"].option);
            }
        }
        if(release_list.empty() && debug_list.empty()) {
            o_str += "DEFAULT";
        }


        return "cppm_compiler_option(" + o_str +")\n";
    }
}
