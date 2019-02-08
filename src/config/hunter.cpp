#include "config/hunter.h"
#include "util/filesystem.h"
#include <fmt/format.h>
#include <util/version.h>

using namespace fmt::literals;

namespace cppm
{
    void Hunter::parse(table_ptr table) {
        auto d_url = "https://github.com/ruslo/hunter/archive/v0.23.98.tar.gz";
        auto d_sha1 = "bf539cb70375ac319e386ae2ec5b5a0c4923480a";
        if(auto hunter = table->get_table("hunter")) {
            url = hunter->get_as<std::string>("url").value_or(d_url);
            sha1 = hunter->get_as<std::string>("sha1").value_or(d_sha1);
        }
        else {
            url = d_url;
            sha1 = d_sha1;
        }
    }

    std::string Hunter::package_path() {
        Version version("0.0.0");
        auto hunter_root = "{0}/.hunter/_Base/Download/Hunter/"_format(std::getenv("HOME"));
        if(auto list = util::file_list(hunter_root)) {
            for(auto& file : *list) {
                auto other = Version::parse(file.path().filename().string());
                if(version < other) {
                    version = other;
                }
            }
        }
        auto min_hash = util::file_list(hunter_root+version.str())->front()
                        .path().filename().string().substr(0,7);
            
        auto hunter_package = "{0}{1}/{2}/Unpacked/cmake/projects/"_format(hunter_root,version.str(),min_hash);
        return hunter_package;
    }

    std::string Hunter::generate() {
        using namespace fmt::literals;
        return "include(cmake/HunterGate.cmake)\n"
             + "HunterGate(\n"_format()
             + "   URL \"{}\"\n"_format(url)
             + "   SHA1 \"{}\"\n"_format(sha1)
             + ")\n";
    }
}
