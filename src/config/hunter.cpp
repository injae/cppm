#include "config/hunter.h"
#include <fmt/format.h>

namespace cppm
{
    void Hunter::parse(table_ptr table) {
        auto d_url = "https://github.com/ruslo/hunter/archive/v0.23.89.tar.gz";
        auto d_sha1 = "a370290a8b32987755a36d64807bf8c7f5b61730";
        if(auto hunter = table->get_table("hunter")) {
            url = hunter->get_as<std::string>("url").value_or(d_url);
            sha1 = hunter->get_as<std::string>("sha1").value_or(d_sha1);
        }
        else {
            url = d_url;
            sha1 = d_sha1;
        }
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
