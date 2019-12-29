string(REPLACE "\\" "/" HOME "$ENV{HOME}")
set(CPPM_VERSION "1.0.9")

set(CPPM_ROOT   ${HOME}/.cppm)
set(CPPM_SOURCE ${CPPM_ROOT}/src)
set(CPPM_CACHE  ${CPPM_ROOT}/cache)
set(CPPM_CORE   ${CPPM_SOURCE}/cppm_tools/${CPPM_VERSION})

include(FetchContent)
if(NOT IS_CPPM_LOADED)
    FetchContent_Populate(cppm-tools-${CPPM_VERSION}
        GIT_REPOSITORY https://github.com/injae/cppm_tools.git
        GIT_TAG        ${CPPM_VERSION}
        SOURCE_DIR     ${CPPM_CORE}
        QUIET
    )
endif()
set(IS_CPPM_LOADED TRUE)
include(${CPPM_CORE}/core_load.cmake)


