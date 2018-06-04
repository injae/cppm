find_package(yaml-cpp REQUIRED)

include(${CMAKE_MODULE_PATH}/utils.cmake)
make_third_party_static_lib(yaml-cpp YAML_CPP)
list(APPEND third_party_library yaml-cpp)