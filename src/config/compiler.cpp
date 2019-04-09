#include "config/compiler.h"
#include <fmt/format.h>

namespace cppm
{
    void Compiler::parse(table_ptr table) {
        auto add_option = [&](table_ptr p_table, auto name) -> CompilerType {
            CompilerType compiler_t;
            compiler_t.name = name;
            auto compiler = p_table->get_table(compiler_t.name);
            compiler_t.option  = compiler->get_as<std::string>("option").value_or("");
            compiler_t.version = compiler->get_as<std::string>("version").value_or("");
            return compiler_t;
            };

        if(auto compilers = table->get_table("compiler")) {
            for(auto& compiler_table : *compilers) {
                if(compiler_table.first == "debug") {
                    if(auto debug_table = compilers->get_table("debug")) {
                        for(auto& debug_compiler : *debug_table){
                            auto c_name = debug_compiler.first;
                            debug_list[c_name] = add_option(debug_table,c_name);
                        }
                    }
                }
                else if(compiler_table.first == "release") {
                    if(auto release_table = compilers->get_table("release")) {
                        for(auto& release_compiler : *release_table){
                            auto c_name = release_compiler.first;
                            release_list[c_name] = add_option(release_table,c_name);
                        }
                    }
                }
                else {
                    auto c_name = compiler_table.first;
                    debug_list[c_name] = add_option(compilers,c_name);
                    release_list[c_name] = add_option(compilers,c_name);
                }
                //CompilerType compiler_t;
                //compiler_t.name = compiler_table.first;
                //auto compiler = compilers->get_table(compiler_t.name);
                //compiler_t.option  = compiler->get_as<std::string>("option").value_or("");
                //compiler_t.version = compiler->get_as<std::string>("version").value_or("");
                //list[compiler_t.name] = compiler_t;
            }
        }
        else {
        }
    }
    std::string Compiler::generate() {
        using namespace fmt::literals;
        std::string o_str = "";
        if(!debug_list.empty()) {
            o_str = "\tDEBUG\n";
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
            o_str = "\tRELEASE\n";
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
            o_str += "\tDEFAULT\n";
        }


        return "cppm_compiler_option(\n" + o_str +")\n";


        //return "if(\"${CMAKE_BUILD_TYPE}\" STREQUAL \"Release\")\n"
        //     + "    if(\"${{CMAKE_CXX_COMPILER_ID}}\" STREQUAL \"Clang\")\n"_format()
        //     + "        set(CMAKE_CXX_FLAGS \"${{CMAKE_CXX_FLAGS}} {0}\")\n"_format(release_list["clang"].option)
        //     + "    elseif(\"${CMAKE_CXX_COMPILER_ID}\" STREQUAL \"GNU\")\n"
        //     + "        set(CMAKE_CXX_FLAGS \"${{CMAKE_CXX_FLAGS}} {0}\")\n"_format(release_list["gcc"].option)
        //     + "    elseif (\"${CMAKE_CXX_COMPILER_ID}\" STREQUAL \"MSVC\")\n"
        //     + "        set(CMAKE_CXX_FLAGS \"${{CMAKE_CXX_FLAGS}} {0}\")\n"_format(release_list["msvc"].option)
        //     + "    elseif (\"${CMAKE_CXX_COMPILER_ID}\" STREQUAL \"AppleClang\")\n"
        //     + "        set(CMAKE_CXX_FLAGS \"${{CMAKE_CXX_FLAGS}} {0}\")\n"_format(release_list["xcode"].option)
        //     + "    endif()\n"
        //     + "else()\n"
        //     + "    if(\"${CMAKE_CXX_COMPILER_ID}\" STREQUAL \"Clang\")\n"
        //     + "        set(CMAKE_CXX_FLAGS \"${{CMAKE_CXX_FLAGS}} {0}\")\n"_format(debug_list["clang"].option)
        //     + "    elseif(\"${CMAKE_CXX_COMPILER_ID}\" STREQUAL \"GNU\")\n"
        //     + "        set(CMAKE_CXX_FLAGS \"${{CMAKE_CXX_FLAGS}} {0}\")\n"_format(debug_list["gcc"].option)
        //     + "    elseif (\"${CMAKE_CXX_COMPILER_ID}\" STREQUAL \"MSVC\")\n"
        //     + "        set(CMAKE_CXX_FLAGS \"${{CMAKE_CXX_FLAGS}} {0}\")\n"_format(debug_list["msvc"].option)
        //     + "    elseif (\"${CMAKE_CXX_COMPILER_ID}\" STREQUAL \"AppleClang\")\n"
        //     + "        set(CMAKE_CXX_FLAGS \"${{CMAKE_CXX_FLAGS}} {0}\")\n"_format(debug_list["xcode"].option)
        //     + "    endif()\n"
        //     + "endif()\n";
    }
}
