#include "config/base_target.h"
#include "util/filesystem.h"
#include "config/config.h"

namespace cppm
{
    std::vector<std::string> base_target::get_sources(Config::ptr config) {
        std::vector<std::string> srcs;
        for(const auto& src : sources) {
            auto result = util::find_files(config->path.root, std::regex(src), false);
            srcs.insert(srcs.end(), result.begin(), result.end());
        }
        return srcs;
    }
}
