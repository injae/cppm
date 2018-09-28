#include <iostream>
#include <string>
#include "config/config.h"
#include "util/filesystem.h"

int main(int argc, char* argv[]) {
    auto path = cppm::util::reverse_find_file(fs::current_path(), "cppm.toml");
    if(!path) std::cout << "can't find cppm.toml" << std::endl;
    auto config = cppm::Config::load(path->string());
   
    return 0;
}
