#include"utils.h"
#include<algorithm>
#include"boost/algorithm/string/replace.hpp"
#include<iostream>
#include<iterator>

std::string make_include_guard(const std::string& file_name) {
    std::string upper_file_name = boost::replace_all_copy(file_name, "/", "_"); 
    
    std::transform(upper_file_name.begin(), upper_file_name.end(), upper_file_name.begin(), ::toupper);   
        return  "#ifndef __"+upper_file_name+"_H__\n"
               +"#define __"+upper_file_name+"_H__\n"
               +"\n\n"
               +"#endif";
    }
    
    std::string make_include(const std::string& header_path) {
        return "#include\"" + header_path + "\"";
}


optional<std::vector<fs::directory_entry>> file_list(const fs::path& path) {
    std::vector<fs::directory_entry> list;
    std::copy(fs::directory_iterator(path), fs::directory_iterator(), std::back_inserter(list));
    return list;    
}

optional<fs::path> find_file(const fs::path& path, const std::string& file_name) {
    if(!fs::exists(path)) return none; 
    
    auto list = file_list(path);
    if(!list) return none; 
    
    for(const auto& file : list.get()) {
      if(file.path().filename() == file_name) return file.path();
    }
    
    if(path == path.root_directory()) return none;
    return find_file(path.parent_path(), file_name);
}

void create_directory(std::string dir_name) {
    fs::create_directory(dir_name);
}

void recursive_copy(const fs::path& src, const fs::path& dst) {
    if (fs::exists(dst)){
       std::cerr << dst.generic_string() + " exists" << std::endl; return;
    }

    if (fs::is_directory(src)) {
       fs::create_directories(dst);
       for(fs::directory_iterator item(src); item != fs::directory_iterator(); ++item) {
    //         std::cout << dst/item->path().filename() << std::endl;
          recursive_copy(item->path(), dst/item->path().filename());
        }
    } 
    else if (fs::is_regular_file(src)) {
      fs::copy(src, dst); 
    } 
    else {
      std::cerr << (dst.generic_string() + " not dir or file") << std::endl; return ;
    } 
}
    
