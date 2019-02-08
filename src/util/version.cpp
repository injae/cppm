#include"util/version.h"
#include<cstdio>
#include<iostream>

#include <fmt/format.h>

using namespace fmt::literals;

namespace cppm
{
    Version::Version(std::string version) {
        if(version == "lastest") { lastest = true; return; }
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
        if(lastest && lastest == other.lastest) return false;
        if(!lastest && other.lastest) return true;
        if(major < other.major)
			return true;
		if(minor < other.minor)
			return true;
		if(revision < other.revision)
			return true;
		return false; 
    }
    
    bool Version::operator > (const Version& other) const {
        if(other.lastest && lastest == other.lastest) return false;
        if(lastest && !other.lastest) return true;
        if(major > other.major)
			return true;
		if(minor > other.minor)
			return true;
		if(revision > other.revision)
			return true;
		return false; 
    }
    
    bool Version::operator == (const Version& other) const {
        if(lastest && lastest == other.lastest) return true;
        return major    == other.major
			&& minor    == other.minor
			&& revision == other.revision;
    }
    
    Version& Version::operator = (const std::string& version) {
        if(version == "lastest") { lastest = true; }
        else std::sscanf(version.c_str(), "%d.%d.%d", &major, &minor, &revision);
        return *this;
    }
    
    std::string Version::str() {
        if(lastest) return "lastest";
        return "{0}.{1}.{2}"_format(major,minor,revision);
    }

    Version::operator std::string() const {
        if(lastest) return "lastest";
        return "{0}.{1}.{2}"_format(major,minor,revision);
    }

    std::ostream& operator << (std::ostream& stream, const Version& ver) {
        if(ver.lastest) return (stream << "lastest");
        stream << ver.major;
        stream << '.';
        stream << ver.minor;
        stream << '.';
        stream << ver.revision;
        return stream; 
    }
}
