#ifndef __CPPM_OPTION_CPPKG_SEARCH_H__
#define __CPPM_OPTION_CPPKG_SEARCH_H__

#include "option/base_option.h"
#include "package/package.h"

namespace cppm::option
{
    class CppkgSearch : public base_option
    {
    public:
        CppkgSearch();
    private:
        void _show_hunter_package(const std::string& target);
        void _show_all();
        std::string _make_use_column(const package::Package& pkg, const std::string& repo);
        bool is_all = false;
        std::string repo_name = "cppkg";
    };
}

#endif