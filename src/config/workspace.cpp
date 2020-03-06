#include"config/workspace.h"
#include<fmt/format.h>
#include<cpptoml.h>
#include"config/config.h"
#include"util/filesystem.h"

namespace cppm
{
    void WorkSpace::parse(table_ptr table)
    {
        auto workspace = toml::get_table(table, "workspace");
        toml::get_array(workspace, "member", member);
        if(!member.empty()) {
            auto dep = toml::get_table(table, "dependencies");
            auto root = fs::path(toml::panic(table, "root"));
            std::for_each(member.begin(), member.end()
            , [&](auto path) {
                  auto origin_path = path;
                  path = (root/path).string();
                  auto sub = Config::load(path);
                  if(!fs::exists(path+"/CMakeLists.txt")) system("cd {} && cppm build --tc"_format(path).c_str());
                  auto mem = cpptoml::make_table();
                  if(!sub->libs.list.empty()) {
                      auto lib = sub->libs.list.front();
                      mem->insert("version", sub->package.version);
                      mem->insert("module", lib.name);
                      mem->insert("type", "lib");
                      mem->insert("load-path", origin_path);
                      dep->insert(lib.name, mem);
                  }
                  else {
                      mem->insert("version", sub->package.version);
                      mem->insert("load-path", origin_path);
                      mem->insert("type", "bin");
                      dep->insert(sub->package.name, mem);
                  }
            });
            table->insert("dependencies", dep);
        }
    }

    std::string WorkSpace::generate() {
        using namespace fmt::literals;
        std::string gen = "\n";
        //for(auto mem : member) gen += "add_subdirectory({0})\n"_format(mem);
        return gen; 
    }
}
