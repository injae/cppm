#ifndef __CPPM_UTIL_FILESYSTEM_H__
#define __CPPM_UTIL_FILESYSTEM_H__

#include <optional>
#include <regex>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

namespace cppm::util
{
    std::optional<std::vector<fs::directory_entry>> file_list(const fs::path& path);
    std::optional<fs::path> find_file(const fs::path& path, const std::string& file_name);
    std::optional<fs::path> reverse_find_file(const fs::path& path, const std::string& file_name);
}

#endif
