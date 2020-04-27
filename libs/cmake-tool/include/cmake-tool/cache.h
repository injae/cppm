
#pragma once
#ifndef __CMAKE_TOOL_H__
#define __CMAKE_TOOL_H__

#include <fmt/format.h>
#include <map>
#include <tuple>
#include <optional>
#include <queue>
#include <functional>

namespace cmake
{
    using namespace fmt::literals;
    inline std::string var(const std::string& str) { return "${{{0}}}"_format(str);}
    namespace func {
        inline std::string arg(const std::string& name, const std::string& value) {
            return name != "" ? "{} {} "_format(name, value) : "";
        }
    }

    class Cache{
    public:
        Cache(std::string path);
        bool exist(const std::string& name, const std::string& value);
        inline std::string&  operator[](const std::string& name) { return variables[name].second; }
        bool exist_file() { return exist_file_; }
        using variable = std::pair<std::string, std::string>;
    private:
        std::map<std::string, variable> variables;
        bool exist_file_=true;
    };

    class Cmake {
    public:
        using opt_str = std::optional<std::string>;
        Cmake() : cache(std::nullopt) {}
        Cmake generator(const std::string& generator);
        Cmake set_target(const std::string& target);
        Cmake generator_options(const std::string& option);
        Cmake define(const std::string name, const std::string value, const std::string option_type="D");
        Cmake set(const std::string name, const std::string value, const std::string option_type="D");
        Cmake build(const std::string& root, const std::string& build_path="build");
    private:
        std::optional<Cache> cache;
        opt_str cmake_option=std::nullopt;
        opt_str generator_option=std::nullopt;
        opt_str target_=std::nullopt;
        std::queue<std::function<void(std::optional<Cache>&, opt_str&)>> after_hooks;
    public:
        bool install=false;
        std::string prefix="";
        std::string build_type="Debug";
        bool no_cache=false;
        bool sudo=false;
        bool detail=false;
    };
}

#endif
