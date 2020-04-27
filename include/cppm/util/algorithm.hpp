#pragma once

#ifndef __CPPM_UTIL_ALGORITHM_HPP__
#define __CPPM_UTIL_ALGORITHM_HPP__

#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <range/v3/all.hpp>


using namespace ranges;

namespace cppm::util
{
    template<typename Collection>
    void insert(Collection& src, Collection& des) {
        src.insert(src.end(), des.begin(), des.end());
    }

    template<typename Collection, typename Func>
    void for_each(Collection& col, Func func) {
        std::for_each(col.begin(), col.end(), func);
    }

    template<typename Collection>
    std::string accumulate(Collection stl, std::string token="") {
        return ranges::accumulate(stl, std::string{},
            [&token](auto& str, const auto& piece) { return str += (token + piece);});
    }

    namespace str
    {
        std::string erase(const std::string& target_str, const std::string& erase_str);
    }

    // -------------------------------------------------------------------
    // --- Reversed iterable
    template <typename T>
    struct reversion_wrapper { T& iterable; };

    template <typename T>
    auto begin (reversion_wrapper<T> w) { return rbegin(w.iterable); }

    template <typename T>
    auto end (reversion_wrapper<T> w) { return rend(w.iterable); }

    template <typename T>
    reversion_wrapper<T> reverse (T&& iterable) { return { iterable }; }
}


#endif
