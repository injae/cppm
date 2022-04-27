
#include <regex>

#include "cli/cppkg_init.h"
#include "cli/build.h"
#include "cli/init.h"
#include "cppm/core/cppm_tool.hpp"
#include "cppm/util/filesystem.h"
#include "cppm/cppkg/cppkg.h"
#include <serdepp/adaptor/nlohmann_json.hpp>

namespace cppm::command {
    void CppkgInit::execute(CppkgInit& cmd) {
        core::Dependency pkg;
        pkg.name = cmd.target;
        if (auto ov = magic_enum::enum_cast<core::cppkg_type>(cmd.type.c_str()); ov.has_value()) {
            pkg.type = ov.value();
        } else {
            throw std::runtime_error("Invalid package type");
        }
        if(!pkg.module && pkg.type == core::cppkg_type::lib) {
            fmt::print("[cppkg] can't find module value\n");
            fmt::print("[cppkg] module name find mode on\n");

            auto current_path = fs::current_path();
            auto path ="{}/cache/__install"_format(core::cppm_root());

            fs::create_directory(path);
            util::working_directory(path);

            std::string name = "script";
            auto cppm_toml = Init::make_project(name,false);
            cppm_toml += "[dependencies]\n{} = \"{}\"\n"_format(pkg.name, pkg.version);
            auto project = core::Path::make((fs::current_path()/name).string());
            if(fs::exists(path)) { fs::remove_all(project.thirdparty);}
            fs::create_directory(project.thirdparty);
            std::fstream file; 
            file.open(project.root/"cppm.toml", std::ios::out);
            file << cppm_toml;
            file.close();

            util::working_directory(project.thirdparty.string());
            cppkg::init(pkg);
            cppkg::build(pkg.name);


            CLI::App app;
            Build build;
            std::vector<std::string> args = {"--release"};
            try {
                serde::serialize_to(build, app);
                app.parse(args);
            } catch(const CLI::ParseError &e) {
                //return app.exit(e);
            }

            auto cache = cmake::Cache(project.build.string());
            auto pkg_path = cache["{}_DIR"_format(pkg.name)];
            if(pkg_path.empty()) {
                fmt::print(stderr, "can't find package name {}", pkg.name);
                exit(1);
            }

            std::regex find_target_cmake(".*[Tt][Aa][Rr][Gg][Ee][Tt][Ss].cmake");
            auto target_file= util::find_files(pkg_path, find_target_cmake, true)[0];
            std::regex filter(R"(foreach\(_expectedTarget (.*)\))");
            if(auto match = util::find_pattern(target_file,filter)) {
                pkg.module = (*match)[1].str();
            } else {
                fmt::print(stderr, "this package can't generate cppkg file\n"
                                    "maybe this package is not cmake package or unexportable package\n");
                exit(1);
            }
            util::working_directory(current_path.string());
        }

        cppkg::init(pkg);
        cppkg::build(pkg.name);

        //if(cmd.regist) {cppkg::regist("{}/{}"_format(pkg.name, pkg.version));}


        throw CLI::Success();
    }
    
}
