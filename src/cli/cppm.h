#pragma once

#ifndef __CPPM_CLI_CPPM_H__
#define __CPPM_CLI_CPPM_H__

#include "cli/util.hpp"
#include "cli/build.h"
#include "cli/init.h"
#include "cli/cppkg.h"
//#include "option/cppkg.h"
#include <serdepp/adaptor/cli11.hpp>
#include <serdepp/attribute/cli11.hpp>

namespace cppm::command
{
    using namespace serde::attribute::cli11;

    class Run {
    public:
        DERIVE_SERDE(Run,
                     .attrs(desc{"Run Binary"}, callback{Run::execute})
                     [attrs(option("-t,--target", "target binary name"))]_SF_(target)
                     [attrs(option("args", "binary args"))]_SF_(args)
                     )
    private:
        static void execute(Run& cmd);
        std::vector<std::string> args;
        std::string target;
    };

    class CTest {
    public:
        DERIVE_SERDE(CTest,
                     .attrs(desc{"Run Unittest"}, callback{CTest::execute})
                     [attrs(option("args", "ctest args"))]_SF_(args)
                     )
    private:
        static void execute(CTest& cmd);
        std::vector<std::string> args;
    };

    struct Update : callback_interface {
        static constexpr auto desc ="update";
        static void callback(std::vector<std::string>& args) {
            system("cppm cppkg install cppm");
            throw CLI::Success{};
        }
    };

    class Cppm 
    {
    public:
        DERIVE_SERDE(Cppm,
                     .attrs(desc{"Cppm"}, callback(Cppm::execute))
                     [attrs(cli11::flag("-v,--version", "cppm version"))]_SF_(version)
                     _SF_(init)
                     _SF_(run)
                     _SF_(test)
                     _SF_(update)
                     _SF_(build)
                     _SF_(cppkg)
                     )
    private:
        static void execute(Cppm& cmd);
    private:
        bool version;
        Init init;
        Run run;
        CTest test;
        SingleCommand<Update> update;
        Build build;
        Cppkg cppkg;
    };
}


#endif
