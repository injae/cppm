#set(Boost_USE_STATIC_LIBS        ON) # only find static libs
find_package(Boost 1.54 COMPONENTS system filesystem program_options regex REQUIRED)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/utils.cmake)
#make_third_party_static_lib(boost Boost)
list(APPEND third_party_library Boost::boost)

#include_directories( ${Boost_INCLUDE_DIR} )
#list(APPEND third_party_library ${Boost_LIBRARIES})
