#pragma once

#ifndef __CPPM_UTIL_FILESYSTEM_H__
#define __CPPM_UTIL_FILESYSTEM_H__

#include <fmt/format.h>

#include <optional>
#include <regex>
#include <fstream>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;

template<>
struct fmt::formatter<fs::path> {
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const fs::path& p, FormatContext& ctx) {
        return format_to(ctx.out(), "{}", p.string());
    }
};

namespace cppm::util
{
    auto file_list(const fs::path& path) -> std::optional<std::vector<fs::directory_entry>>;
    auto recursive_file_list(const fs::path& path)-> std::optional<std::vector<fs::directory_entry>>;
    auto find_file(const fs::path& path, const std::string& file_name) -> std::optional<fs::path>;
    auto reverse_find_file(const fs::path& path, const std::string& file_name) -> std::optional<fs::path>;
    void recursive_copy(const fs::path& src, const fs::path&dst);
    auto find_files(const std::string& path, std::regex filter, bool is_full_path) -> std::vector<std::string>;
    void over_write_copy_file(const std::string& src, const std::string& des);
    void create(const std::string& path);
    void write_file(const std::string& path, const std::string& data);
    template<typename T> void write(const std::string& path, T content) {
        std::fstream file(path, std::ios::out);
        file << content;
        file.close();
    }
    std::string current_dir();
    std::string read_file_all(const std::string& name);
    std::string file_hash(const std::string& name);
}
#endif
