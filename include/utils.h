#ifndef __UTILS_H__
#define __UTILS_H__

#include<iostream>
#include<string>
#include"boost/filesystem.hpp"
#include"boost/optional.hpp"
#include<vector>

namespace fs = boost::filesystem;
using boost::optional;
using boost::none;

uint64_t constexpr mix(char m, uint64_t s) {
    return ((s<<7) + ~(s>>3)) + ~m;
}
 
uint64_t constexpr hash(const char * m) {
    return (*m) ? mix(*m,hash(m+1)) : 0;
}

std::string make_include_guard(const std::string& file_name);
std::string make_include(const std::string& header_path);
optional<std::vector<fs::directory_entry>> file_list(const fs::path& path);
optional<fs::path> find_file(const fs::path& path, const std::string& file_name);
void create_directory(std::string dir_name);
void recursive_copy(const fs::path& src, const fs::path& dst);

#endif