#include "util/algorithm.hpp"

namespace cppm::util
{
    namespace str
    {
        std::string erase(const std::string& target_str, const std::string& erase_str) {
            auto copy_str = target_str; 
            auto pos = copy_str.find(erase_str);
            if(pos != std::string::npos) return copy_str.erase(pos, erase_str.length());
            else                         return copy_str;
        }
    }
}
