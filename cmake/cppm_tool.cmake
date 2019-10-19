string(REPLACE "\\" "/" HOME "$ENV{HOME}")
set(CPPM_VERSION "1.0.9")
set(CPPM_ROOT   ${HOME}/.cppm)
set(CPPM_CORE   ${CPPM_ROOT}/cmake/core)

include(FetchContent)
if(NOT IS_CPPM_LOADED)
    FetchContent_Populate(cmake-tools-${CPPM_VERSION}
        GIT_REPOSITORY https://github.com/injae/cppm_tools.git
        GIT_TAG        ${CPPM_VERSION}
        SOURCE_DIR     ${CPPM_CORE}/${CPPM_VERSION}
        QUIET
    )
endif()
set(IS_CPPM_LOADED TRUE)
include(${CPPM_CORE}/${CPPM_VERSION}/core_load.cmake)

