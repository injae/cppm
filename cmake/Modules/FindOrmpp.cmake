
include(${CMAKE_MODULE_PATH}/utils.cmake)
make_third_party_header_only_lib(ormpp "${THIRD_PARTY_DIR}/ormpp/")
list(APPEND third_party_library ormpp)