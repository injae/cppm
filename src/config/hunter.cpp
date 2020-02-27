#include "config/hunter.h"
#include "util/filesystem.h"
#include <fmt/format.h>
#include <util/version.h>
#include "util/system.hpp"

using namespace fmt::literals;

namespace cppm
{
    void Hunter::parse(table_ptr table) {
        auto hunter = toml::get_table(table, "hunter");
        url         = toml::get(hunter, "url" , "https://github.com/ruslo/hunter/archive/v0.23");
        sha1        = toml::get(hunter, "sha1", "e14bc153a7f16d6a5eeec845fb0283c8fad8c358");       
        use_hunter  = toml::get_bool(hunter, "use" , false);
    }

    void Hunter::build_lock(table_ptr table, table_ptr lock) {
        auto origin = toml::get_table(table, "hunter");
        auto hunter = cpptoml::make_table();
        hunter->insert("url" , url);
        hunter->insert("sha1", sha1);
        hunter->insert("use" , use_hunter);
        lock->insert("hunter", hunter);
    }

    std::string Hunter::package_path() {
        Version version("0.0.0");
        auto hunter_root = "{0}.hunter/_Base/Download/Hunter/"_format(util::system::home_path());
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
        return "include(HunterGate)\n"
             + "HunterGate(\n"_format()
             + "   URL \"{}\"\n"_format(url)
             + "   SHA1 \"{}\"\n"_format(sha1)
             + ")\n";
    }
}
