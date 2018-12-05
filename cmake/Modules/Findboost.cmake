include(ExternalProject)

find_package(boost)

if(NOT boost_FOUND)
ExternalProject_Add(
boost
URL https://dl.bintray.com/boostorg/release/1.68.0/source/boost_1_68_0.tar.gzSOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/boost
CONFIGURE_COMMAND ./bootstrap.sh && ./b2
)
find_package(boost)

endif()
