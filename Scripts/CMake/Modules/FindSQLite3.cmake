# Find Sqlite3
# ~~~~~~~~~~~~
# CMake module to search for Sqlite3 library
#
# If it's found it sets SQLITE3_FOUND to TRUE
# and following variables are set:
#    SQLITE3_INCLUDE_DIR
#    SQLITE3_LIBRARY
#    SQLITE3_VERSION
#	 SQLITE3_VERSION_STRING

find_path( SQLITE3_INCLUDE_DIR sqlite3.h
  "$ENV{LIB_DIR}/include"
  "$ENV{LIB_DIR}/include/sqlite"
  "$ENV{INCLUDE}"
)

find_library( SQLITE3_LIBRARY NAMES sqlite3_i sqlite3 PATHS
  "$ENV{LIB_DIR}/lib"
  "$ENV{LIB}/lib"
)

if( SQLITE3_INCLUDE_DIR AND SQLITE3_LIBRARY )
   set( SQLITE3_FOUND TRUE )
endif()

if( SQLITE3_FOUND )

	# Read SQLite version information
	file( READ "${SQLITE3_INCLUDE_DIR}/sqlite3.h" SQLITE3_VERSION_CONTENT )
	string( REGEX MATCH "#define SQLITE_VERSION[ ]*\"[0-9]+\\.[0-9]+\\.[0-9]+\".*\n" SQLITE3_VERSION_STRING_MATCH ${SQLITE3_VERSION_CONTENT} )
	string( REGEX MATCH "#define SQLITE_VERSION_NUMBER[ ]*[0-9]*\n" SQLITE3_VERSION_MATCH ${SQLITE3_VERSION_CONTENT} )
	if( SQLITE3_VERSION_STRING_MATCH AND SQLITE3_VERSION_MATCH )
		string( REGEX REPLACE "#define SQLITE_VERSION[ ]*\"([0-9]+\\.[0-9]+\\.[0-9]+)\".*\n" "\\1" SQLITE3_VERSION_STRING ${SQLITE3_VERSION_STRING_MATCH} )
		string( REGEX REPLACE "#define SQLITE_VERSION_NUMBER[ ]*([0-9]*)\n" "\\1" SQLITE3_VERSION ${SQLITE3_VERSION_MATCH} )
		set( SQLITE3_VERSION ${SQLITE3_VERSION} CACHE STRING "SQLite3 numeric version" )
		set( SQLITE3_VERSION_STRING ${SQLITE3_VERSION_STRING} CACHE STRING "SQLite3 version" )
	endif()

	if( NOT SQLITE3_FIND_QUIETLY )
		message( STATUS "Found Sqlite3 of version: ${SQLITE3_VERSION_STRING}" )
	endif()   
   
else()

	if( SQLITE3_FIND_REQUIRED )
		message( FATAL_ERROR "Could not find Sqlite3" )
	endif()

endif( SQLITE3_FOUND )
