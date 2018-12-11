#ifndef __CPPM_UTIL_FILESYSTEM_H__
#define __CPPM_UTIL_FILESYSTEM_H__

#include <optional>
#include <regex>
#include <boost/filesystem.hpp>
#include <fstream>

namespace fs = boost::filesystem;

namespace cppm::util
{
    auto file_list(const fs::path& path) -> std::optional<std::vector<fs::directory_entry>>;
    auto recursive_file_list(const fs::path& path)-> std::optional<std::vector<fs::directory_entry>>;
    auto find_file(const fs::path& path, const std::string& file_name) -> std::optional<fs::path>;
    auto reverse_find_file(const fs::path& path, const std::string& file_name) -> std::optional<fs::path>;
    auto find_files(const std::string& path, std::regex filter, bool is_full_path) -> std::vector<std::string>;
    void create(const std::string& path);
    template<typename T> void write(const std::string& path, T content) {
        std::fstream file; 
        file.open(path);
        file << content;
        file.close();
    }
}
#endif
