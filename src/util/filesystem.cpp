#include "util/filesystem.h"
#include "util/algorithm.hpp"
#include <iostream>

#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

namespace cppm::util
{
    auto file_list(const fs::path& path) -> std::optional<std::vector<fs::directory_entry>> {
        std::vector<fs::directory_entry> list;
        std::copy(fs::directory_iterator(path), fs::directory_iterator(), std::back_inserter(list));
        return list;    
    }
 
    auto find_files(const std::string& path, std::regex filter, bool is_full_path) -> std::vector<std::string> {
        auto files = *recursive_file_list(path);
        std::vector<std::string> matching_files;
        fs::directory_iterator end_itr;
        for(auto it : files) {
            std::smatch what;
            auto file = str::erase(it.path().string(), path + "/");
            if(!std::regex_match(file, what, filter)) continue;
            if(!is_full_path)  matching_files.push_back(file);
            else matching_files.push_back(it.path().string());
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
}
