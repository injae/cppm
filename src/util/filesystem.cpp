#include "util/filesystem.h"

namespace cppm::util
{
    std::optional<std::vector<fs::directory_entry>> file_list(const fs::path& path) {
        std::vector<fs::directory_entry> list;
        std::copy(fs::directory_iterator(path), fs::directory_iterator(), std::back_inserter(list));
        return list;    
    }
    
    std::optional<fs::path> find_file(const fs::path& path, const std::string& file_name) {
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

    std::optional<fs::path> reverse_find_file(const fs::path& path, const std::string& file_name) {
        if(!fs::exists(path)) return std::nullopt; 
        auto list = file_list(path);
        if(!list) return std::nullopt; 
        for(const auto& file : *list) {
            if(file.path().filename() == file_name) return std::make_optional(file.path());
            if(file.path().filename() == "/")       return std::nullopt;
        }
        return reverse_find_file(path.parent_path(), file_name);
    }
}
