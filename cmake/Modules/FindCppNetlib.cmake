find_package ( cppnetlib REQUIRED )

include(${CMAKE_MODULE_PATH}/utils.cmake)
make_third_party_static_lib(cpp-netlib CPPNETLIB)
list(APPEND third_party_library cpp-netlib)
