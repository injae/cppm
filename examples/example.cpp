#include "cppm/cmake/script.hpp"
#include "cppm/core/config.hpp"
#include <range/v3/all.hpp>
#include "options/cppm.h"
#include <cpcli/cpcli.hpp>

int main(int argc, char* argv[]) {
    cpcli::parse_with_exec<cppm::option::Cppm>(argc,argv);
    return 0;
}

