#include"utils.h"
#include<algorithm>
#include<boost/algorithm/string/replace.hpp>
#include<iostream>
#include<iterator>

namespace util
{
    bool has_str(std::string& target, std::string_view str) {
        std::size_t pos = 0;
        if ((pos = target.find(str, 0)) != std::string::npos) {
            return true;
        } 
        else {
           return false;
        }
    }

}