#include"util/version.h"
#include<cstdio>
#include<iostream>

namespace cppm
{
    Version::Version(std::string version) {
        std::sscanf(version.c_str(), "%d.%d.%d.%d", &major, &minor, &revision, &build);
    }
    
    bool Version::operator < (const Version& other) {
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
    
    bool Version::operator == (const Version& other) {
        return major    == other.major
			&& minor    == other.minor
			&& revision == other.revision
			&& build    == other.build;    
    }
    
    Version& Version::operator = (const std::string& version) {
        std::sscanf(version.c_str(), "%d.%d.%d.%d", &major, &minor, &revision, &build);
        return *this;
    }
    
    std::string Version::str() {
        std::stringstream stream("");
        stream << major << "." << minor << "." << revision << "." << build;
        return stream.str();
    }

    Version::operator std::string() const {
        std::stringstream stream("");
        stream << major << "." << minor << "." << revision << "." << build;
        return stream.str();
    }

    std::ostream& operator << (std::ostream& stream, const Version& ver) {
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
