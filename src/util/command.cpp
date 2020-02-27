#include "util/command.h"
#include <reproc++/sink.hpp>

namespace console
{
    std::vector<std::string> split(const std::string &txt, char ch)
    {
        std::vector<std::string> strs;
        size_t pos = txt.find(ch);
        size_t initialPos = 0;
        strs.clear();

        while(pos != std::string::npos) {
            strs.push_back(txt.substr( initialPos, pos - initialPos));
            initialPos = pos + 1;
            pos = txt.find(ch, initialPos);
        }
        strs.push_back(txt.substr(initialPos, std::min( pos, txt.size()) - initialPos + 1));

        return strs;
    }

    void console::execute(hook&& hook_) {
        reproc::process pc;
        pc.start(split(cmd_, ' '), options);
        reproc::drain(pc, hook_, hook_);
        pc.wait(reproc::infinite);
    }
}
