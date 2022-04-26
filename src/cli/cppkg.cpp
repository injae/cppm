
#include "cli/cppkg.h"
#include "cppm/core/cppm_tool.hpp"
#include "cppm/cppkg/cppkg.h"
#include "cppm/util/filesystem.h"
#include "cppm/util/string.hpp"

#include <serdepp/adaptor/fmt.hpp>

namespace cppm::command {
    void Cppkg::execute(Cppkg& cmd) {

        throw CLI::Success{};
    }

    void CppkgGenerate::callback(std::vector<std::string>& args) {
        for (auto& arg : args) {
            auto dep = cppkg::parse(arg);
            cppkg::transcompile(dep, ".");
        }
        
        throw CLI::Success{};
    }

    void CppkgUpdate::callback(std::vector<std::string>& args) {
        using namespace fmt::literals;
        auto cppkg_path = "{0}repo/cppkg"_format(core::cppm_root());
        if(!fs::exists(cppkg_path)) {
            fmt::print("clone cppkg path:{}\n",cppkg_path);
            system("git clone https://github.com/injae/cppkg.git {}"_format(cppkg_path).c_str());
        }

        auto command = "cd {0} &&  git fetch origin && git pull origin master"_format(cppkg_path);
        system(command.c_str());
        
        throw CLI::Success{};
    }

    void CppkgBuild::callback(std::vector<std::string>& args) {
        for (auto& arg : args) { cppkg::build(arg); }
        
        throw CLI::Success{};
    }
}
