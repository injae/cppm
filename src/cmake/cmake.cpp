#include "cmake/cmake.h"
#include "util/filesystem.h"
#include "util/optional.hpp"
#include <sstream>
#include <regex>

namespace cppm::cmake
{
    Cache::Cache(std::string path) {
        path += "/CMackeCache.txt";
        if(fs::exists(path)) return;
        auto ss = std::stringstream{util::read_file_all(path)};
        std::string to;
        std::regex filter("^(?!#)(.*):(.*)=(.*)$");
        std::smatch what;
        while (std::getline(ss, to, '\n')) {
            if(std::regex_match(to, what, filter)) {
              variables.insert(std::make_pair(what[0], std::make_pair(what[1], what[2])));}
        }
    }

    bool Cache::exist(const std::string& name, const std::string& value) {
        if(auto data = variables.find(name); data != variables.end()) {
            if(data->second.second == value) return true;
            return false;
        }
        else {
            return false;
        }
    }

    Cmake Cmake::generator(const std::string& generator) {
        auto hook = [this, generator](){
            if(!cache->exist("CMAKE_GENERATOR", generator)) {
                if(!cmake_option) cmake_option = "";
                *cmake_option += "-G {} "_format(generator);
            }
        };
        after_hooks.push(hook);
        return *this;
    }

    Cmake Cmake::define(const std::string& name, const std::string& value, const std::string& option_type) {
        auto hook = [this, name, value, option_type](){
            if(!cache->exist(name, value)) {
                if(!cmake_option) cmake_option = "";
                *cmake_option += "-{}{}={} "_format(option_type, name, value);
            }
        };
        after_hooks.push(hook);
        return *this;
    }

    Cmake Cmake::generator_options(const std::string& option) {
        if(!generator_option) generator_option = "";
        *generator_option += option;
        return *this;
    }

    Cmake Cmake::build(const std::string& root, opt_str target, const std::string& build_path) {
        cache = Cache("{}/{}"_format(root, build_path));
        while(!after_hooks.empty()) { after_hooks.front()(); after_hooks.pop(); }
        auto script = "cd {}/{} &&"_format(root, build_path);
        script += cmake_option ? "cmake {}.. && "_format(*cmake_option) : "";
        auto target_script = target ? "--target {} "_format(*target) : "";
        script += "cmake --build . {}-- {}"_format(target_script, *generator_option);
        fmt::print(script);
        //system(script.c_str());
        return *this;
    }
}
