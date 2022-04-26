#include "cli/cppm.h"

int main(int argc, char* argv[]) {
    CLI::App app;
    cppm::command::Cppm cppm;
    serde::serialize_to(cppm, app);
    CLI11_PARSE(app, argc, argv);

    return 0;
}

