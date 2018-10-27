#include "option/init.h"
#include <fmt/format.h>


namespace cppm::option
{
    Init::Init(Config& config) {
        using namespace fmt::literals;
        app_.add_option("help")
            .abbr("h")
            .desc("show cppm command and options")
            .call_back([&](){ app_.show_help(); });
        //app_.add_option("bin")
        //    .abbr("b")
        //    .desc("initialize new c++ binary project")
        //    .call_back([&](){ this->make_bin(config); });
        //
        //app_.add_option("lib")
        //    .abbr("l")
        //    .desc("initialize new c++ library project")
        //    .call_back([&](){ this->make_lib(config); });
    }
    void Init::make_bin(Config& config) {

    }
    void Init::make_lib(Config& config){

    }

}
