#include "config/compiler.h"
#include <fmt/format.h>

namespace cppm
{
    void Compiler::parse(table_ptr table) {
        if(auto compilers = table->get_table("compiler")) {
            for(auto& compiler_table : *compilers) {
                CompilerType compiler_t;
                compiler_t.name = compiler_table.first;
                auto compiler = compilers->get_table(compiler_t.name);
                compiler_t.option  = compiler->get_as<std::string>("option").value_or("");
                compiler_t.version = compiler->get_as<std::string>("version").value_or("");
                list[compiler_t.name] = compiler_t;
            }
        }
        else {
            CompilerType compiler_t;
            compiler_t.name = "none";
            list[compiler_t.name] = compiler_t;
        }
    }
    std::string Compiler::generate() {
        using namespace fmt::literals;
        return "if(\"${CMAKE_CXX_COMPILER_ID}\" STREQUAL \"Clang\")\n"
             + "    set(CMAKE_CXX_FLAGS \"${{CMAKE_CXX_FLAGS}} {0}\") \n"_format(list["clang++"].option)
             + "elseif(\"${CMAKE_CXX_COMPILER_ID}\" STREQUAL \"GNU\")  \n"
             + "    set(CMAKE_CXX_FLAGS \"${{CMAKE_CXX_FLAGS}} {0}\") \n"_format(list["g++"].option)
             // + \"elseif (\"${{CMAKE_CXX_COMPILER_ID}}\" STREQUAL \"Intel\")\n\"
             // + \"set(CMAKE_CXX_FLAGS \"${{CMAKE_CXX_FLAGS}} {0}\")\"_format();
             // + \"elseif (\"${{CMAKE_CXX_COMPILER_ID}}\"\" STREQUAL \"MSVC\") \n\"
             // + \"set(CMAKE_CXX_FLAGS \"${{CMAKE_CXX_FLAGS}} {0}\")\"_format();
             + "endif()\n";
    }
}
