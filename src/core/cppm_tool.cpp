#include "cppm/core/cppm_tool.hpp"
#include "cppm_version.h"
#include "cppm/util/filesystem.h"
#include "cppm/util/version.h"
#include <range/v3/all.hpp>
#include <hashpp/md5.h>

using namespace ranges;


namespace cppm::core {
    namespace cmake {
        template<typename T>
        std::string arg (std::string name, T& var) { return var ? " {} {}"_format(name, *var) : ""; }
        std::string arg_flag(bool flag, std::string var) {return flag ? var : "";}
        template<typename T>
        std::string qarg(std::string name, T& var) { return var ? " {} \"{}\""_format(name, *var) : ""; }
        std::string quote(std::string var) { return "\"{}\""_format(var); }
        std::string set(std::string name, std::string var) { return "set({} {})\n"_format(name, var); }
        std::string get(std::string name) { return "${{{}}}"_format(name); }
        std::string getf(std::string name) { return "${{{}}}"_format(name); }
        std::string append(std::string name, std::string vars) { return "list(APPEND {} {})"_format(name, vars); }
        std::string if_else(std::string flag, std::string code) {
            return "if({})\n"
                   "   {}\n"
                   "endif()\n"_format(flag, code);
        }
        std::string if_else(std::string flag, std::string code, std::string else_code) {
            return "if({})\n"
                   "   {}\n"
                   "else()\n"
                   "   {}\n"
                   "endif()\n"_format(flag, code, else_code);
        }
    }

    std::string cppm_translate(Config& config) {
        std::string gen = fmt::format(
             "cmake_minimum_required(VERSION {cmake_version})\n"
              "\n"
              "set(CPPM_VERSION {cppm_version})\n"
              "include(cmake/cppm_loader.cmake)\n"
              "cppm_project({with_vcpkg})\n"
              "{hunter}\n" // dependencies hunter
              "project({pkg_name} VERSION {pkg_ver} LANGUAGES C CXX)\n"
              "cppm_setting({unity_build})\n"
              "cppm_cxx_standard({cpp_ver})\n"
              "{compiler}\n"
              "{include}\n"
              "{load_dependencies}\n\n"
              "{cppkg_define}\n"
              "{cppkg_dependencies}\n"
              "{cppkg_install}\n"
               ,"cppm_version"_a=std::string(CPPM_VERSION)
               ,"unity_build"_a=cmake::arg_flag(config.package.unity_build.value(), "UNITY_BUILD")
               ,"with_vcpkg"_a=cmake::arg_flag(config.package.with_vcpkg.value(), "WITH_VCPKG")
               ,"cmake_version"_a=config.cmake->version.value()
               ,"hunter"_a=hunter_load(config)
               ,"pkg_name"_a=config.package.name
               ,"pkg_ver"_a=config.package.version.value()
               ,"cpp_ver"_a=config.package.standard.value()
               ,"compiler"_a=cppm_compiler_option(config)
               ,"include"_a=include_cmake_files(config)
               ,"load_dependencies"_a=find_cppkg(config)
               ,"cppkg_define"_a=cppm_target_define(config)
               ,"cppkg_dependencies"_a=cppm_target_dependencies(config)
               ,"cppkg_install"_a=cppm_target_install(config)
        );
        

        return gen;
    }
    std::string cppm_header(Config& config) {
        std::string gen;
        return gen;
    }

    std::string hunter_load(Config& config) {
        std::string gen;
        auto& is_hunter = config.hunter->use.value();
        if(!is_hunter) {
            auto find_hunter = [&is_hunter](auto& target){
                    if(target) {
                        auto result = *target | views::filter([](auto it) { return *it.second.repo == "hunter"; });
                        if(!result.empty()) is_hunter = true;
                    }
                };
            find_hunter(config.dependencies);
            find_hunter(config.dev_dependencies);
        }
        if(is_hunter) {
            util::over_write_copy_file("{0}cmake/HunterGate.cmake"_format(cppm_root())
                                      ,"{0}/HunterGate.cmake"_format(config.path.cmake.string()));
            gen = fmt::format("include(HunterGate)\n"
                  "HunterGate(\n"
                  "   URL {url}\n"
                  "   SHA1 {sha1}\n"
                 ")"
                 ,"url"_a=*config.hunter->url
                 ,"sha1"_a=*config.hunter->sha1);
        }
        return gen;
    }
    std::string cppm_compiler_option(Config& config) {
        std::string gen;
        if(config.profile) {
            for(auto& [name, type] : *config.profile){
                auto rtype = name == "dev" ? "DEBUG" : name | copy | actions::transform(::toupper);
                if(type.compiler) {
                    auto script = *type.compiler
                        | views::filter([](auto it) { return it.second.option.has_value();})
                        | views::transform([](auto it) { auto& [name, comp] = it;
                            auto cname = name | copy | actions::transform(::toupper);
                            return "      {} \"{}\"\n"_format(cname, *comp.option);
                        }) | to_vector;
                    if(!script.empty()) {
                        gen += "\n   {}\n{}"_format(rtype, fmt::join(script, ""));
                    }
                }
            }
        }else {
            gen = "DEFAULT";
        }
        return "cppm_compiler_option({})"_format(gen);
    }
    std::string include_cmake_files(Config& config) {
        std::string gen;
        if(config.cmake->include) {
            auto script = *config.cmake->include
                        | views::transform([](auto it) { return "include({})"_format(it); });
            gen = ranges::accumulate(script, std::string{}, [](auto it, auto acc) { return it + "\n" + acc; });
        }
        return gen;
    }

    std::string find_cppkg(Config& config) {
        using namespace cmake;
        std::string gen;
        auto make_find_cppkg = [&gen](auto& deps){
            if(deps) {
                auto scripts = *deps | views::transform([](auto it) {
                    auto& [name, dep] = it;
                    auto hunter = dep.repo == "hunter" ? " HUNTER" : "";
                    return fmt::format("find_cppkg({name} {ver} {module}{components}{path}{hunter}{type})\n"
                                            ,"name"_a=dep.name
                                            ,"ver"_a=dep.version
                                            ,"module"_a=arg("MODULE",dep.module)
                                            ,"components"_a=qarg("COMPONENTS",dep.components)
                                            ,"path"_a=arg("LOADPATH",dep.path)
                                            ,"hunter"_a=hunter
                                            ,"type"_a=arg("TYPE",dep.type));
                }) | to_vector;
                gen += fmt::format("{}",fmt::join(scripts, ""));
            }
        };
        make_find_cppkg(config.dependencies);
        if(config.dev_dependencies) {
            gen += "\nif(CMAKE_BUILD_TYPE STREQUAL \"Debug\")\n";
            make_find_cppkg(config.dev_dependencies);
            gen += "endif()\n";
        }

        if(config.target) {
            ranges::for_each(*config.target, [&gen,&make_find_cppkg](auto& it) {
                auto& [name, target] = it;
                gen += "\ntriplet_check({})\nif(_result)\n"_format(quote(name));
                make_find_cppkg(target.dependencies);
                if (target.dev_dependencies) {
                    gen += "\nif(CMAKE_BUILD_TYPE STREQUAL \"Debug\")\n";
                    make_find_cppkg(target.dev_dependencies);
                    gen += "endif()\n";
                }
                gen += "endif()\n";
            });
        }
        return gen;
    }


    std::string cppm_target_define(Config& config) {
        using namespace cmake;
        std::string gen;
        auto make_script = [&gen](auto& pkg) {
                auto src = pkg.source ? fmt::format("\nSOURCES\n    {}\n",fmt::join(*pkg.source, "\n    ")) : "";
                std::string cppkg_type = *pkg.cppkg_type == "header-only" ? "INTERFACE"
                                                                          : *pkg.cppkg_type
                                                                          | copy
                                                                          | actions::transform(::toupper);
                gen += fmt::format("cppm_target_define({name} {type}{namespace}{sources})\n\n"
                                  ,"name"_a=pkg.name
                                  ,"type"_a=cppkg_type
                                  ,"namespace"_a=arg("NAMESPACE",pkg.namespace_)
                                  ,"sources"_a=src);
        };
        if(config.lib)  make_script(*config.lib);
        if(config.bins) ranges::for_each(*config.bins, make_script);
        auto upkg_name = config.package.name | copy | actions::transform(::toupper);
        if(config.examples) {
            gen += "cppm_examples_area()\n"
                   "if({}_BUILD_EXAMPLES)\n\n"_format(upkg_name);
            ranges::for_each(*config.examples, make_script);
            gen += "endif()\n\n"_format(upkg_name);
        } 
        if(config.tests) {
            gen += "cppm_unit_test_area()\n"
                   "if({}_BUILD_TESTING)\n\n"_format(upkg_name);
            ranges::for_each(*config.tests, make_script);
            gen += "endif()\n"_format(upkg_name);
            gen += "end_cppm_unit_test_area()\n\n"_format(upkg_name);
        } 

        return gen;
    }
    std::string cppm_target_dependencies(Config& config) {
        std::string gen;
        auto grouped_deps = [](auto& deps) {
            if(deps) {
                auto libs = *deps
                    | views::for_each([](auto it) { return yield_if(it.second.type == "lib", it.second); })
                    | to_vector;
                libs |= actions::sort([](auto a, auto b){ return *a.link > *b.link; });
                auto group = libs | views::group_by([](auto a, auto b) { return *a.link == *b.link; });
                auto link_join = [](auto& i) {
                    auto names =  i | views::transform([](auto i){ return i.name; }) | to_vector;
                    return "{} {}"_format(*i[0].link | copy | actions::transform(::toupper)
                                             ,names | views::join(views::c_str(" ")) | to<std::string>());
                };
                auto cmake_dep = group
                    | views::transform([&link_join](auto i){ return link_join(i); })
                    | views::cache1
                    | views::join(views::c_str("\n"))
                    | to<std::string>();
                return cmake_dep; 
            }
            return std::string{""};
        };
        gen += cmake::set("global_deps", grouped_deps(config.dependencies));
        if(config.dev_dependencies) {
            gen += cmake::if_else("CMAKE_BUILD_TYPE STREQUAL \"Debug\""
                                 , cmake::append("global_deps",grouped_deps(config.dev_dependencies)));
        }

        if(config.target) {
            ranges::for_each(*config.target, [&gen,&grouped_deps](auto& it) {
                auto& [name, target] = it;
                gen += "\ntriplet_check({})\nif(_result)\n"_format(cmake::quote(name));
                gen += cmake::append("global_deps",grouped_deps(target.dependencies));
                if (target.dev_dependencies) {
                    gen += "\nif(CMAKE_BUILD_TYPE STREQUAL \"Debug\")\n";
                    gen += cmake::append("global_deps",grouped_deps(target.dev_dependencies));
                    gen += "endif()\n";
                }
                gen += "\nendif()\n";
            });
        }

        auto set_dependencies = [&config](auto& target) {
            std::vector<std::string> deps{cmake::getf("global_deps")};
            if(target.type != "lib" && config.lib) deps.emplace_back(config.lib->name);
            return fmt::format("cppm_target_dependencies({name}\n   {deps})\n\n"
                               ,"name"_a=target.name
                               ,"deps"_a=fmt::join(deps,"\n   "));
        };
        if(config.lib)  gen += set_dependencies(*config.lib);
        if(config.bins) {
            ranges::for_each(*config.bins, [&set_dependencies, &gen](auto& it) { gen += set_dependencies(it); });
        }
        if(config.examples) {
            ranges::for_each(*config.examples, [&set_dependencies, &gen](auto& it) { gen += set_dependencies(it); });
        }
        if(config.tests) {
            ranges::for_each(*config.tests, [&set_dependencies, &gen](auto& it) { gen += set_dependencies(it); });
        }

        return gen;
    }
    std::string cppm_target_install(Config& config) {
        std::string gen;
        auto make_install = [&gen](auto& target) {
            if(*target.install) {
                gen += "cppm_target_install({})\n"_format(target.name);
            }
        };
        if(config.lib) make_install(*config.lib);
        if(config.bins) for(auto& bin : *config.bins) { make_install(bin); }
        if(config.tests) for(auto& test : *config.tests) { make_install(test); }
        if(config.examples) for(auto& exam : *config.examples) { make_install(exam); }
        return gen;
    }
    std::string cppm_download_package(Dependency& dep) {
        auto download = cmake::arg("GIT", dep.git);
        download += cmake::arg("URL", dep.url);
        download += cmake::arg("GIT_TAG", dep.branch);
        download += cmake::arg("SHA256", dep.sha256);
        return fmt::format(
            "# Cppkg Base Dependency Downloader\n"
            "cmake_minimum_required(VERSION 3.6)\n"
            "project({name}-{version}-install)\n\n"
            "set(CPPM_VERSION {cppm_version})\n"
            "include(${{CMAKE_CURRENT_SOURCE_DIR}}/cmake/cppm_loader.cmake)\n"
            "download_package({name} {version} {url} {type} CMAKE_ARGS "
            "${{CMAKE_ARGS}} {flags})\n\n",
            "name"_a = dep.name, "version"_a = (*dep.version), "cppm_version"_a=std::string(CPPM_VERSION),
            "url"_a = download, "type"_a=cmake::arg("TYPE", dep.type), "flags"_a = dep.flags.value_or(""));
    }

    std::string hunter_root() {
        Version version("0.0.0");
        auto hunter_root = "{0}.hunter/_Base/Download/Hunter/"_format(util::system::home_path());
        if(auto list = util::file_list(hunter_root)) {
            for(auto& file : *list) {
                auto other = Version::parse(file.path().filename().string());
                if(version < other) {
                    version = other;
                }
            }
        }
        auto min_hash = util::file_list(hunter_root+version.str())->front().path().filename().string().substr(0,7);
        return "{}{}/{}/Unpacked/cmake/projects/"_format(hunter_root,version.str(),min_hash);
    }

    void install_cppm_download_package(fs::path path, Dependency& dep) {
        auto script = cppm_download_package(dep);
        auto filename = (path/("{}.cmake.in"_format(dep.name))).string();
        if(util::file_hash(filename) != hashpp::md5(script)) { util::write_file(filename, script); }
    }

    Dependency cppm_auto_generate_cppkg(fs::path& path) {
        Dependency dep;


        return dep;
    }
}
