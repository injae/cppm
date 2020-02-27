#ifndef __CPPM_OPTION_BASE_OPTION_H__
#define __CPPM_OPTION_BASE_OPTION_H__

#include <nlpo/app.h>
#include "config/config.h"

namespace cppm::option
{
    class base_option
    {
    public:
        nlpo::App& app() { return app_; }
        virtual ~base_option() {}
        bool config_load();
        void start_from(const std::string& path) { start_path_ = path; }
    protected:
        nlpo::App app_;
        Config::ptr config_;
        std::string start_path_ = "";
    };
}

#endif
