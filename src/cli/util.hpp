#pragma once

#ifndef __CPPM_CLI_UTIL_HPP__
#define __CPPM_CLI_UTIL_HPP__

#include <serdepp/serde.hpp>
#include <serdepp/attribute/cli11.hpp>

namespace cppm::command
{
    struct callback_interface {
        static constexpr std::string_view desc = "";
        static void callback(std::vector<std::string>& args){ throw serde::unimplemented_error("need callback function"); };
    };

    using namespace serde::attribute::cli11;
    template<typename T=callback_interface>
    struct SingleCommand {
        SingleCommand()=default;
        DERIVE_SERDE(SingleCommand<T>, .attributes(desc{std::string{T::desc}},
                                                   cli11::callback{SingleCommand<T>::callback})
                     [attrs(cli11::option("args", "args"))]_SF_(args))
        std::vector<std::string> args;

        static void callback(SingleCommand<T>& cmd) { T::callback(cmd.args); }
    };
} // namespace cppm::option

#endif
