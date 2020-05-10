# - Try to find MySQL.
# Once done this will define:
# MYSQL_FOUND			- If false, do not try to use MySQL.
# MYSQL_DYNAMIC		    - Dynamic libraries.
# MYSQL_INCLUDE_DIRS	- Where to find mysql.h, etc.
# MYSQL_LIBRARIES		- The libraries to link against.
# MYSQL_VERSION_STRING	- Version in a string of MySQL.
#
# Created by RenatoUtsch based on eAthena implementation.
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
    find_path( MYSQL_INCLUDE_DIR
            NAMES "mysql.h"
            PATHS
                "${MySQL_DIR}/include"
                "$ENV{PROGRAMFILES}/MySQL/*/include"
                "$ENV{PROGRAMFILES\(x86\)}/MySQL/*/include"
                "$ENV{SYSTEMDRIVE}/MySQL/*/include" )

    find_library( MYSQL_LIBRARY
            NAMES
                "libmysql"
                "mysqlclient"
                "mysqlclient_r"
            PATHS
                "${MySQL_DIR}/lib"
                "$ENV{PROGRAMFILES}/MySQL/*/lib"
                "$ENV{PROGRAMFILES\(x86\)}/MySQL/*/lib"
                "$ENV{SYSTEMDRIVE}/MySQL/*/lib" )

    find_file( MYSQL_DYNAMIC
            NAMES
                "libmysql.dll"
                "mysqlclient.dll"
                "mysqlclient_r.dll"
            PATHS
                "${MySQL_DIR}/lib"
                "$ENV{PROGRAMFILES}/MySQL/*/lib"
                "$ENV{PROGRAMFILES\(x86\)}/MySQL/*/lib"
                "$ENV{SYSTEMDRIVE}/MySQL/*/lib" )
else()
    find_path( MYSQL_INCLUDE_DIR
            NAMES "mysql.h"
            PATHS "${MySQL_DIR}/include"
            PATHS "/usr/include/mysql"
            "/usr/local/include/mysql"
            "/usr/mysql/include/mysql" )

    find_library( MYSQL_LIBRARY
            NAMES "mysqlclient" "mysqlclient_r"
            PATHS "${MySQL_DIR}/lib"
            PATHS "/lib/mysql"
            "/lib64/mysql"
            "/usr/lib/mysql"
            "/usr/lib64/mysql"
            "/usr/local/lib/mysql"
            "/usr/local/lib64/mysql"
            "/usr/mysql/lib/mysql"
            "/usr/mysql/lib64/mysql" )
endif()

if( MYSQL_INCLUDE_DIR AND EXISTS "${MYSQL_INCLUDE_DIR}/mysql_version.h" )
    file( STRINGS "${MYSQL_INCLUDE_DIR}/mysql_version.h"
            MYSQL_VERSION_H REGEX "^#define[ \t]+MYSQL_SERVER_VERSION[ \t]+\"[^\"]+\".*$" )
    string( REGEX REPLACE
            "^.*MYSQL_SERVER_VERSION[ \t]+\"([^\"]+)\".*$" "\\1" MYSQL_VERSION_STRING
            "${MYSQL_VERSION_H}" )
    message("Found MySQL library ${MYSQL_LIBRARY}")
    message("Found MySQL version ${MYSQL_VERSION_STRING}")
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