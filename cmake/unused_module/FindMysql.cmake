# - Try to find MySQL.
# Once done this will define:
# MYSQL_FOUND			- If false, do not try to use MySQL.
# MYSQL_INCLUDE_DIRS	- Where to find mysql.h, etc.
# MYSQL_LIBRARIES		- The libraries to link against.
# MYSQL_VERSION_STRING	- Version in a string of MySQL.
#
# Created by RenatoUtsch based on eAthena implementation.
# Updated by Andre Bar'yudin
#
# Please note that this module only supports Windows and Linux officially, but
# should work on all UNIX-like operational systems too.
#
 
#=============================================================================
# Copyright 2012 RenatoUtsch
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)
 
if( WIN32 )
	set(MYENVx86 "PROGRAMFILES(X86)")
 
	set(MYSQL_SOURCE_DIRS 
		"$ENV{PROGRAMFILES}/MySQL" 
		"$ENV{${MYENVx86}}/MySQL" 
		"$ENV{SYSTEMDRIVE}/MySQL"
		"$ENV{ProgramW6432}/MySQL"
		)
 
	foreach(stem ${MYSQL_SOURCE_DIRS})
		file(GLOB MYTEMPVAR1 LINK_DIRECTORIES TRUE "${stem}/*/include")
		list(APPEND MYSQL_DIRS_INC
			${MYTEMPVAR1})
		file(GLOB MYTEMPVAR2 LINK_DIRECTORIES TRUE "${stem}/*/lib")
		list(APPEND MYSQL_DIRS_LIB
			${MYTEMPVAR2})
	endforeach(stem)
 
	find_path( MYSQL_INCLUDE_DIR
		NAMES "mysql.h"
		PATHS ${MYSQL_DIRS_INC})
 
	find_library( MYSQL_LIBRARY
		NAMES "mysqlclient" "mysqlclient_r"
		PATHS  ${MYSQL_DIRS_LIB} )
else()
	find_path( MYSQL_INCLUDE_DIR
		NAMES "mysql.h"
		PATHS "/usr/include/mysql"
			  "/usr/local/include/mysql"
			  "/usr/mysql/include/mysql" )
 
	find_library( MYSQL_LIBRARY
		NAMES "mysqlclient" "mysqlclient_r"
		PATHS "/lib/mysql"
			  "/lib64/mysql"
			  "/usr/lib/mysql"
			  "/usr/lib64/mysql"
			  "/usr/local/lib/mysql"
			  "/usr/local/lib64/mysql"
			  "/usr/mysql/lib/mysql"
			  "/usr/mysql/lib64/mysql" )
endif()
 
if( EXISTS "${MYSQL_INCLUDE_DIR}/mysql_version.h" )
	file( STRINGS "${MYSQL_INCLUDE_DIR}/mysql_version.h"
		MYSQL_VERSION_H REGEX "^#define[ \t]+MYSQL_SERVER_VERSION[ \t]+\"[^\"]+\".*$" )
	string( REGEX REPLACE
		"^.*MYSQL_SERVER_VERSION[ \t]+\"([^\"]+)\".*$" "\\1" MYSQL_VERSION_STRING
		"${MYSQL_VERSION_H}" )
endif()
 
# handle the QUIETLY and REQUIRED arguments and set MYSQL_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( MYSQL
    REQUIRED_VARS	MYSQL_LIBRARY MYSQL_INCLUDE_DIR
	VERSION_VAR		MYSQL_VERSION_STRING )
 
set( MYSQL_INCLUDE_DIRS ${MYSQL_INCLUDE_DIR} )
set( MYSQL_LIBRARIES ${MYSQL_LIBRARY} )
 
mark_as_advanced( MYSQL_INCLUDE_DIR MYSQL_LIBRARY )

include(${CMAKE_MODULE_PATH}/utils.cmake)
make_third_party_static_lib(mysql MYSQL)
list(APPEND third_party_library mysql)




