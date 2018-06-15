include(FindProtobuf)
find_package(Protobuf REQUIRED)

include(${CMAKE_MODULE_PATH}/utils.cmake)
make_third_party_static_lib(protobuf Protobuf)
list(APPEND third_party_library protobuf)
