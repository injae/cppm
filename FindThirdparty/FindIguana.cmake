
include(${CMAKE_MODULE_PATH}/utils.cmake)
make_third_party_header_only_lib(iguana "${THIRD_PARTY_DIR}/iguana/")
list(APPEND third_party_library iguana)