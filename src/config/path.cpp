#include "config/path.h"
#include <fmt/format.h>

namespace cppm
{
     Path Path::make(const std::string& root_path) {
         Path path;
         path.root       = root_path;
         path.build      = path.root + "/build";
         path.cmake      = path.root + "/cmake";
         path.source     = path.root + "/src";
         path.include    = path.root + "/include";
         path.thirdparty = path.root + "/thirdparty";
         return path;
     }
}
