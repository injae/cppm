#ifndef __UTILS_H__
#define __UTILS_H__

#include<iostream>
#include<string>
#include"boost/filesystem.hpp"
#include"boost/optional.hpp"
#include<boost/regex.hpp>
#include<vector>

#define BOOST_FILESYSTEM_NO_DEPRECATED
#define BOOST_FILESYSTEM_VERSION 3

namespace fs = boost::filesystem;
using boost::optional;
using boost::none;

namespace util 
{
    bool has_str(std::string& target, std::string_view str);
}
#endif