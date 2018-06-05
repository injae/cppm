set(Boost_USE_STATIC_LIBS        ON) # only find static libs
find_package(Boost 1.55 COMPONENTS system filesystem program_options REQUIRED)

include(${CMAKE_MODULE_PATH}/utils.cmake)
#make_third_party_static_lib(boost Boost)
#list(APPEND third_party_library boost)

include_directories( ${Boost_INCLUDE_DIR} )
list(APPEND third_party_library ${Boost_LIBRARIES})
