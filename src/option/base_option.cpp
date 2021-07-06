#include "option/base_option.h"
#include "cppm/util/filesystem.h"
#include "cppm/util/algorithm.hpp"
#include <fmt/format.h>

namespace cppm::option {
    bool base_option::config_load(bool panic) {
        fs::path start_position = (start_path_ == "") ? fs::current_path() : fs::path{start_path_};
        auto path = cppm::util::reverse_find_file(start_position, "cppm.toml");
        if(!path) {
            if(panic) {
                fmt::print("can't find cppm.toml\n");
                exit(1);
            } else {
                fmt::print("can't find cppm.toml\nbuild with cppm_toolchain\n");
            }
            return false;
        }
        // 나중에 쓸예정 none cppm test
        config_ = core::Config::load(path->parent_path());
        return true;
    }

}
