#ifndef __CPPM_CONFIG_PATH_H__
#define __CPPM_CONFIG_PATH_H__

#include <string>


namespace cppm
{
    class Path
    {
    public:
        static Path make(const std::string& root_path);
    public:
        std::string root;
        std::string build;
        std::string cmake;
        std::string source;
        std::string include;
        std::string thirdparty;
    };
}

#endif
