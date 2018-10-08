#ifndef __CPPM_UTIL_ALGORITHM_HPP__
#define __CPPM_UTIL_ALGORITHM_HPP__

#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

namespace cppm::util
{
    template<typename Collection>
    void insert(Collection& src, Collection& des) {
        src.insert(src.end(), des.begin(), des.end());
    }

    template<typename Collection>
    std::string accumulate(Collection stl, std::string token="") {
        return std::accumulate(begin(stl), end(stl), std::string{},
            [&token](auto& str, const auto& piece) { return str += (token + piece);});
    }
}


#endif
