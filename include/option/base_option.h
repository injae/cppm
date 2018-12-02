#ifndef __CPPM_OPTION_BASE_OPTION_H__
#define __CPPM_OPTION_BASE_OPTION_H__

#include <nlpo/app.h>

namespace cppm::option
{
    class base_option
    {
    public:
        nlpo::App& app() { return app_; }
        virtual ~base_option() {}
    protected:
        nlpo::App app_;
    };
}

#endif
