#ifndef __CPPM_UTIL_OPTIONAL_HPP
#define __CPPM_UTIL_OPTIONAL_HPP

#include<optional>
#include<fmt/format.h>
#include<cpptoml.h>
#include<type_traits>
#include<string_view>
#include<iterator>
#include<algorithm>
#include<functional>

#include <memory>

namespace cppm::util
{
    inline void panic(bool opt, const std::string& message) { if(opt) { fmt::print(stderr, "{}", message);  exit(1); } }

    template<typename T>
    T panic(std::optional<T> opt, const std::string& message) {
        if(!opt) { fmt::print(stderr, "{}", message);  exit(1); }
        return *opt;
    }

    template<typename T>
    std::shared_ptr<T> panic(std::shared_ptr<T> opt, const std::string& message) {
        if(!opt) { fmt::print(stderr, "{}", message);  exit(1); }
        return opt;
    }
}


namespace cppm::toml {
    inline std::shared_ptr<cpptoml::table> get_table(std::shared_ptr<cpptoml::table> table,
                                                             const std::string& name) {
        auto tb = table->get_table(name);
        if(!tb) tb = cpptoml::make_table();
        return tb;
    }

    inline std::shared_ptr<cpptoml::table_array> get_table_array(std::shared_ptr<cpptoml::table> table,
                                                                 const std::string& name,
                                                                 std::optional<std::function<void(std::shared_ptr<cpptoml::table>)>> func = std::nullopt) {
        auto tb = table->get_table_array(name);
        if(tb && func) std::for_each(tb->begin(), tb->end(), *func);
        return tb;

    }

    inline int get(std::shared_ptr<cpptoml::table> table, const std::string& name, int def_v) {
        return table->get_as<int>(name).value_or(def_v);
    }

    inline std::string get(std::shared_ptr<cpptoml::table> table, const std::string& name, std::string_view def_v) {
        return table->get_as<std::string>(name).value_or(std::string{def_v});
    }

    template<typename T>
    std::optional<T> get(std::shared_ptr<cpptoml::table> table, std::string_view name) {
        return table->get_as<T>(std::string{name});
    }

    template<typename T>
    std::optional<T> get(std::shared_ptr<cpptoml::table> table, const std::string& name) {
        return table->get_as<T>(std::string{name});
    }

    template<typename T = std::string>
    auto panic(std::shared_ptr<cpptoml::table> table, const std::string& name) {
        using namespace fmt::literals;
        return util::panic(get<T>(table, name), "need {} config"_format(name));
    }

    template<typename Iter>
    auto get_array(std::shared_ptr<cpptoml::table> table, const std::string& name, Iter& dest) {
        auto arr = table->get_array_of<std::string>(name);
        if(arr) { std::copy(arr->begin(), arr->end(), std::back_inserter(dest)); }
        return arr;
    }


}
#endif
