#include "util/filesystem.h"
#include "util/algorithm.hpp"
#include <fmt/format.h>
#include <md5/md5.h>
#include <iostream>

#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

namespace cppm::util
{
    auto file_list(const fs::path& path) -> std::optional<std::vector<fs::directory_entry>> {
        fs::directory_iterator b(path), e;
        std::vector<fs::directory_entry> list{b, e};
        return list;    
    }
 
    auto find_files(const std::string& path, std::regex filter, bool is_full_path) -> std::vector<std::string> {
        auto files = *recursive_file_list(path);
        std::vector<std::string> matching_files;
        fs::path path_(path);
        fs::directory_iterator end_itr;
        for(auto it : files) {
            std::smatch what;
            auto file = str::erase(it.path().generic_string(), path_.generic_string() + "/");
            if(!std::regex_match(file, what, filter)) continue;
            if(!is_full_path) {
                matching_files.push_back(file);
            }
            else matching_files.push_back(it.path().generic_string());
        }
        return matching_files;
    }
    auto recursive_file_list(const fs::path& path) -> std::optional<std::vector<fs::directory_entry>> {
        std::vector<fs::directory_entry> list;
        fs::directory_iterator end_itr;
        for( fs::directory_iterator it(path); it != end_itr; ++it) {
           if(is_directory(it->status())) {
               auto file_list  = recursive_file_list(it->path());
               if(!file_list) return list;
               insert(list, *file_list);
               continue;
           }
           list.push_back(*it);
        }
        return list;
    }
    
    auto find_file(const fs::path& path, const std::string& file_name) -> std::optional<fs::path> {
        if(!fs::exists(path)) return std::nullopt; 
        auto list = file_list(path);
        if(!list) return std::nullopt; 
        for(const auto& file : *list) {
            if(file.path().filename() == file_name) return file.path();
            if(fs::is_directory(file)) 
                if(auto f = find_file(file, file_name)) return f;
        }
        return std::nullopt;
    }

    auto reverse_find_file(const fs::path& path, const std::string& file_name) -> std::optional<fs::path> {
        if(!fs::exists(path)) return std::nullopt; 
        auto list = file_list(path);
        if(!list) return std::nullopt; 
        for(const auto& file : *list) {
            if(file.path().filename() == file_name) return std::make_optional(file.path());
            if(file.path().filename() == "/")       return std::nullopt;
        }
        return reverse_find_file(path.parent_path(), file_name);
    }

    void create(const std::string& path) {
        if(fs::exists(path)) return;
        std::fstream(path, std::ios::out).close();
    }

    void write_file(const std::string& path, const std::string& data) {
         std::ofstream file(path);
         file.is_open();
         file << data;
         file.close();
    }

    std::string current_dir() {
        char buff[FILENAME_MAX];
        GetCurrentDir( buff, FILENAME_MAX );
        std::string current_working_dir(buff);
        return current_working_dir;
    }

    void recursive_copy(const fs::path &src, const fs::path &dst)
    {
        //if (fs::exists(dst)){throw std::runtime_error(dst.generic_string() + " exists");}
        if (fs::is_directory(src)) {
            fs::create_directories(dst);
            for (fs::directory_entry item : fs::directory_iterator(src)) {
            recursive_copy(item.path(), dst/item.path().filename());
            }
        } 
        else if (fs::is_regular_file(src)) {fs::copy(src, dst);} 
        else {throw std::runtime_error(dst.generic_string() + " not dir or file");}
    }

    void over_write_copy_file(const std::string& src, const std::string& des) {
        if(file_hash(src) != file_hash(des)) {
            //   fs::copy_file(src, des, fs::copy_option::overwrite_if_exists);
            fs::copy_file(src, des, fs::copy_options::overwrite_existing);
        }
    }

    std::string read_file_all(const std::string& name) {
        std::ifstream ifs(name);
        auto content = std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
        ifs.close();
        return content;
    }

    std::string file_hash(const std::string& name) {
        return hash::md5(read_file_all(name));
    }
}
