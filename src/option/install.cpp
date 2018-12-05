#include "option/install.h"
#include "package/package.h"

namespace cppm::option
{
    Install::Install(Config& config) {
        package::Package package;
        app_.add_option("help")
            .abbr("h")
            .desc("show cppm commands and options")
            .call_back([&](){ app_.show_help(); });
        //app_.add_option("git")
        //    .abbr("g")
        //    .desc("install to git repo")
        //    .call_back([&](){
        //       
        //    });
        //app_.add_option("url")
        //    .abbr("u")
        //    .desc("install to use url")
        //    .call_back([&](){
        //    });
        //app_.add_command()
        //    .desc("default command")
        //    .call_back([&](){
        //    });
    }
}
