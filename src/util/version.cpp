#include"util/version.h"
#include<cstdio>
#include<iostream>

#include <fmt/format.h>

using namespace fmt::literals;

namespace cppm
{
    Version::Version(std::string version) {
        if(version == "latest") { latest = true; return; }
        if(version == "git")    { git    = true; return; }
        std::sscanf(version.c_str(), "%d.%d.%d", &major, &minor, &revision);
    }

    Version Version::parse(std::string version_include_string) {
        std::regex regex("(\\d+)\\.?(\\d+)\\.?(\\d+)");
        std::smatch match;
        if(std::regex_search(version_include_string,match,regex)) {
            return {match.str()};
        }
        return {"0.0.0.0"};
    }
    
    bool Version::operator < (const Version& other) const {
        if(latest && latest == other.latest) return false;
        if(!latest && other.latest) return true;
        if(git && git == other.git) return false;
        if(!git && other.git)   return false;
        if(major < other.major) return true;
		if(minor < other.minor) return true;
		if(revision < other.revision) return true;
		return false; 
    }
    
    bool Version::operator > (const Version& other) const {
        if(other.latest && latest == other.latest) return false;
        if(latest && !other.latest)                return true;

        if(git && git == other.git) return false;
        if(git && !other.git)       return true;

        if(major > other.major)       return true;
		if(minor > other.minor)       return true;
		if(revision > other.revision) return true;
		return false; 
    }
    
    bool Version::operator == (const Version& other) const {
        if(latest && latest == other.latest) return true;
        if(git && git == other.git)          return true;
        return major    == other.major
			&& minor    == other.minor
			&& revision == other.revision;
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
}
