#include "option/cppkg_init.h"
#include "option/init.h"
#include <cppm/util/filesystem.h>
#include <cppm/core/cppm_tool.hpp>
#include <option/build.h>
#include <regex>

using namespace fmt::literals;

namespace cppm::option
{
    CppkgInit::CppkgInit() {
        app_.add_option("uri").abbr("U")
            .desc("auto detect uri or git and version")
            .call_back([&](nlpo::arg::One arg){
                std::regex is_url(R"(.*((\d)\.(\d)(\.\d)(\.\d)?).*)");
                std::regex is_git(R"(((git|ssh|http(s)?)|(git@[\w\.]+))(:(\/)?)([\w\.@\:/\-~]+)(\.git)(\/)?)");
                std::smatch match;
                if(std::regex_match(arg, match, is_url)) {
                    pkg.version = match[1];
                    pkg.url = arg;
                    fmt::print("[cppkg] detect url and {}\n", pkg.version);
                }
                else if(std::regex_match(arg, match, is_git)) {
                    pkg.version = "git";
                    pkg.git = arg;
                    fmt::print("[cppkg] detect git and {}\n", pkg.version);
                }
                else {
                    fmt::print(stderr, "can't parse uri\n");
                    exit(1);
                }
            });
        app_.add_option("git").abbr("g").args("{repo}")
            .desc("add git repo")
            .call_back([&](){ pkg.git = app_.get_arg(); pkg.version = "git";});
        app_.add_option("git_tag").abbr("t").args("{tag}")
            .desc("add git tag")
            .call_back([&](){ pkg.branch = app_.get_arg(); });
        app_.add_option("url").abbr("u").args("{url}")
            .desc("add url repo Require --version")
            .call_back([&](){ pkg.url = app_.get_arg(); });
        app_.add_option("module").abbr("m").args("{module}")
            .desc("add module name")
            .call_back([&](){ pkg.module = app_.get_arg(); });
        app_.add_option("des").abbr("d").args("{description}")
            .desc("add description")
            .call_back([&](){ pkg.description = app_.get_arg(); });
        app_.add_option("type").abbr("t")
            .desc("add type default is lib")
            .call_back([&](nlpo::arg::One arg){ pkg.type = arg; });
        app_.add_option("flags").abbr("f")
            .desc("add cmake build flags")
            .call_back([&](nlpo::arg::One arg){ pkg.flags = arg; });
        app_.add_option("version").abbr("v").args("{version}")
            .desc("add library version Require --version")
            .call_back([&](){ pkg.version = app_.get_arg(); });
        app_.add_option("p").abbr("p")
            .desc("cmake base library install to local cppkg repo, Require --url or --git")
            .is_show(false)
            .call_back([&](){ regist = true; });
        app_.add_command().args("{name}")
            .call_back([&](){
                     if(app_.args().size() > 1) { fmt::print(stderr, "too many argument"); exit(1); }
                else if(app_.args().size() < 1) { fmt::print(stderr, "need package name"); exit(1); }
                pkg.name = app_.get_arg();
                if(!pkg.type) pkg.type = "lib";
                if(!pkg.module && *pkg.type == "lib") {
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
                    auto build = Build();
                    build.start_from(project.root.string());
                    std::vector<std::string> args = {"--release"};
                    build.app().args().insert(build.app().args().end(), args.begin(), args.end());
                    build.app().parse(app_);
                    auto cache = cmake::Cache(project.build.string());
                    auto pkg_path = cache["{}_DIR"_format(pkg.name)];
                    std::regex find_target_cmake(".*[Tt][Aa][Rr][Gg][Ee][Tt][Ss].cmake");
                    auto target_file= util::find_files(pkg_path,find_target_cmake,true)[0];
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
                if(regist) {cppkg::regist("{}/{}"_format(pkg.name, pkg.version));}
            });
    }
}

// ((\d)\.(\d)\.(\d)\.?(\d)?) => is url
// ((git|ssh|http(s)?)|(git@[\w\.]+))(:(\/)?)([\w\.@\:/\-~]+)(\.git)(\/)? => is git
