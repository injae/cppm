#include "util/version.h"
#include "util/filesystem.h"

#include <cstdio>
#include <iostream>
#include <fmt/format.h>

using namespace fmt::literals;

namespace cppm
{
    Version::Version(std::string version) {
        if(version == "latest") { latest = true; return; }
        if(version == "git")    { git    = true; return; }
        std::sscanf(version.c_str(), "%d.%d.%d", &major, &minor, &revision);
    }

    int _change_compare_int(const Version& version) {
        if(version.latest)   return 999999999;
        else if(version.git) return 1;
        else                 return 10000 * version.major + 1000 * version.minor + version.revision + 10;
    }

    Version Version::parse(std::string version_include_string) {
        if(version_include_string == "latest") return {"latest"};
        if(version_include_string == "git")    return {"git"}; 
        std::regex regex("(\\d+)\\.?(\\d+)\\.?(\\d+)");
        std::smatch match;
        if(std::regex_search(version_include_string,match,regex)) {
            return { match.str()} ;
        }
        return {"0.0.0.0"};
    }
    
    bool Version::operator < (const Version& other) const {
        return _change_compare_int(*this) < _change_compare_int(other);
    }
    
    bool Version::operator > (const Version& other) const {
        return _change_compare_int(*this) > _change_compare_int(other);
    }
    
    bool Version::operator == (const Version& other) const {
        return _change_compare_int(*this) == _change_compare_int(other);
    }
    
    Version& Version::operator = (const std::string& version) {
             if(version == "latest") { latest = true; }
        else if(version == "git")    { git = true; }
        else std::sscanf(version.c_str(), "%d.%d.%d", &major, &minor, &revision);
        return *this;
    }
    
    std::string Version::str() {
             if(latest) return "latest";
        else if(git)    return "git";
        else            return "{0}.{1}.{2}"_format(major,minor,revision);
    }

    Version::operator std::string() const {
             if(latest) return "latest";
        else if(git)    return "git";
        else            return "{0}.{1}.{2}"_format(major,minor,revision);
    }

    std::ostream& operator << (std::ostream& stream, const Version& ver) {
        if(ver.latest) return (stream << "latest");
        if(ver.git)    return (stream << "git");
        return (stream << ver.major << '.'
                       << ver.minor << '.'
                       << ver.revision);
    }

    std::optional<std::string> Version::get_latest_version_folder(const std::string& path) {
            auto versions = util::file_list(path);
            if(!versions) return std::nullopt;
            std::sort(versions->begin(),versions->end()
                        ,[](auto a, auto b){
                            auto av = Version(a.path().filename().string());
                            auto bv = Version(b.path().filename().string());
                            return av > bv;
                        });
             return versions->begin()->path().string();
    }
}
