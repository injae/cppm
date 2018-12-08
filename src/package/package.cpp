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
            download.git.tag = download_->get_as<std::string>("git_tag").value_or("master");

            if(auto build_   = package->get_table("build")) {
                build.config  = build_->get_as<std::string>("config").value_or("");
                build.build   = build_->get_as<std::string>("build").value_or("");
                build.install = build_->get_as<std::string>("install").value_or("");
            }
        }
    }

    std::string Package::generate() {
        using namespace fmt::literals;
        auto download_ = [&](){
            if(download.url != "") {
                return "URL {0}"_format(download.url);
            }
            else if(download.git.url != "") {
                return "GIT_REPOSITORY {0}\n"_format(download.git.url)
                     + "GIT_TAG {0}\n"_format(download.git.tag);  
            }
            else {
                std::cerr << "need download argument" << std::endl;
                exit(1);
            }
        };
        auto build_ = [&](){
            std::string gen;
            if(build.config != "") 
                gen += "CONFIGURE_COMMAND {0}\n"_format(build.config);
            if(build.build != "")
                gen += "BUILD_COMMAND {0}\n"_format(build.build);
            if(build.install != "")
                gen += "INSTALL_COMMAND {0}\n"_format(build.install);
            return gen;
        };

        auto cmake_ = [&]() -> std::string {
            if(cmake.option != "") return "CMAKE_ARGS {0}"_format(cmake.option);
            else return "";
        };

        return "cmake_minimum_required(VERSION 3.10)\n"
             + "project({0}-install NONE)\n"_format(name)
             + "include(ExternalProject)\n\n"
             + "find_package({0})\n\n"_format(name)
             + "if(NOT {0}_FOUND)\n"_format(name)
             + "ExternalProject_Add(\n"
             + "{0}\n"_format(name)
             + download_()
             + "SOURCE_DIR ${{CMAKE_BINARY_DIR}}/repo/{0}\n"_format(name)
             + cmake_()
             + build_()
             + ")\n"
             //+ "find_package({0})\n\n"_format(name)
             + "endif()\n"
             ;
    }
}
