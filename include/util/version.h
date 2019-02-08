#ifndef __CPPM_VERSION_H__
#define __CPPM_VERSION_H__

#include<string>
#include<sstream>
#include<regex>

namespace cppm
{
    class Version
    {
    public:
        Version() {};
        Version(std::string version);
        static Version parse(std::string version_include_string);
        bool operator< (const Version&  other) const;
        bool operator> (const Version&  other) const;
        bool operator== (const Version& other) const;
        Version& operator= (const std::string& str);
        std::string str();
        operator std::string() const;
        friend std::ostream& operator << (std::ostream& stream, const Version& other);

        bool lastest = false;
        int major = 0;
        int minor = 0;
        int revision = 0;
    };
}

#endif
