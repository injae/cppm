#include"util/version.h"
#include<cstdio>
#include<iostream>

#include <fmt/format.h>

using namespace fmt::literals;

namespace cppm
{
    Version::Version(std::string version) {
        if(version == "lastest") { lastest = true; return; }
        std::sscanf(version.c_str(), "%d.%d.%d.%d", &major, &minor, &revision, &build);
    }
    
    bool Version::operator < (const Version& other) {
        if(lastest && lastest == other.lastest) return false;
        if(!lastest && other.lastest) return true;
        if(major < other.major)
			return true;
		if(minor < other.minor)
			return true;
		if(revision < other.revision)
			return true;
		if(build < other.build)
			return true;
		return false; 
    }
    
    bool Version::operator > (const Version& other) {
        if(other.lastest && lastest == other.lastest) return false;
        if(lastest && !other.lastest) return true;
        if(major > other.major)
			return true;
		if(minor > other.minor)
			return true;
		if(revision > other.revision)
			return true;
		if(build > other.build)
			return true;
		return false; 
    }
    
    bool Version::operator == (const Version& other) {
        if(lastest && lastest == other.lastest) return true;
        return major    == other.major
			&& minor    == other.minor
			&& revision == other.revision
			&& build    == other.build;    
    }
    
    Version& Version::operator = (const std::string& version) {
        if(version == "lastest") { lastest = true; }
        else std::sscanf(version.c_str(), "%d.%d.%d.%d", &major, &minor, &revision, &build);
        return *this;
    }
    
    std::string Version::str() {
        if(lastest) return "lastest";
        return "{0}.{1}.{2}.{3}"_format(major,minor,revision,build);
    }

    Version::operator std::string() const {
        if(lastest) return "lastest";
        return "{0}.{1}.{2}.{3}"_format(major,minor,revision,build);
    }

    std::ostream& operator << (std::ostream& stream, const Version& ver) {
        if(ver.lastest) return (stream << "lastest");
        stream << ver.major;
        stream << '.';
        stream << ver.minor;
        stream << '.';
        stream << ver.revision;
        stream << '.';
        stream << ver.build;
        return stream; 
    }
}
