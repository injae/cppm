#ifndef __CPPM_VERSION_H__
#define __CPPM_VERSION_H__

#include<string>
#include<sstream>

namespace cppm
{
    class Version
    {
    public:
        Version() {};
        Version(std::string version);
        bool operator< (const Version&  other);
        bool operator> (const Version&  other);
        bool operator== (const Version& other);
        Version& operator= (const std::string& str);
        std::string str();
        operator std::string() const;
        friend std::ostream& operator << (std::ostream& stream, const Version& other);

        bool lastest = false;
        int major = 0;
        int minor = 0;
        int revision = 0;
        int build = 0;
    };
}

#endif
