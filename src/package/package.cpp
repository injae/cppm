#include "package/package.h"
#include <fmt/format.h>
#include <iostream>

namespace cppm::package
{
    void Package::parse(table_ptr table) {
        if(auto package = table->get_table("package")) {
            name    = package->get_as<std::string>("name").value_or("");
            version = package->get_as<std::string>("version").value_or("");
            target  = package->get_as<std::string>("target").value_or("all");

            auto cmake_      = package->get_table("cmake");
            cmake.name       = cmake_->get_as<std::string>("name").value_or("");
            cmake.option     = cmake_->get_as<std::string>("option").value_or("");
            cmake.components = cmake_->get_as<std::string>("components").value_or("");

            auto download_   = package->get_table("download");
            download.url     = download_->get_as<std::string>("url").value_or("");
            download.git.url = download_->get_as<std::string>("git").value_or("");
            download.git.tag = download_->get_as<std::string>("git_tag").value_or("");

            if(auto build_   = package->get_table("build")) {
                build.config  = build_->get_as<std::string>("config").value_or("");
                build.build   = build_->get_as<std::string>("build").value_or("");
                build.install = build_->get_as<std::string>("install").value_or("");
            }
        }
    }

    std::string Package::generate() {
        using namespace fmt::literals;
        auto is_default = [&](std::string str) { return str != "" ? "" : "#"; };
        return "cmake_minimum_required(VERSION 3.10)\n"
             + "project({0}-install NONE)\n\n"_format(name)
             + "#==========================================\n"
             + "find_package({0} QUIET)\n"_format(name)
             + "if(NOT {0}_FOUND)\n"_format(name)
             + "include(ExternalProject)\n"
             + "ExternalProject_Add(\n"
             + "{0}\n"_format(name)
             + "{0}URL {1}\n"_format(is_default(download.url), download.url)
             + "{0}GIT_REPOSITORY {1}\n"_format(is_default(download.git.url),download.git.url)
             + "{0}GIT_TAG {1}\n"_format(is_default(download.git.tag),download.git.tag)
             + "SOURCE_DIR ${{CMAKE_BINARY_DIR}}/repo/{0}\n"_format(name)
           //+ "BINARY_DIR ${{CMAKE_BINARY_DIR}}/repo/{0}/build\n"_format(name)
             + "{0}CMAKE_ARGS {1}\n"_format(is_default(cmake.option), cmake.option)
             + "{0}CONFIGURE_COMMAND {1}\n"_format(is_default(build.config),build.config)
             + "{0}BUILD_COMMAND {1}\n"_format(is_default(build.build), build.build)
             + "{0}INSTALL_COMMAND {1}\n"_format(is_default(build.install), build.install)
             + "BUILD_IN_SOURCE true\n"
             + ")\n"
             //+ "find_package({0})\n\n"_format(name)
             + "endif()\n"
             + "#==========================================\n\n"
             ;
    }
}
