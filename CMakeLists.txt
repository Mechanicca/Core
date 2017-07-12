project( CoreConfig )

# Set CMake modules location
set( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Scripts/CMake/Modules" )

# -- SQLite ----------------------------------------------
find_package( SQLite3 )

# -- Boost library ---------------------------------------
# NOTE: Package Boost::units cannot be detected
find_package( Boost 1.33.0 REQUIRED COMPONENTS filesystem system )

if( Boost_FOUND )
	include_directories( SYSTEM ${Boost_INCLUDE_DIRS} )
	link_directories( ${Boost_LIBRARY_DIRS} )	
endif()

# -- OpenCascade -----------------------------------------
# TODO: Try to remove unnecessary libraries if there are any
find_package( OpenCascade REQUIRED COMPONENTS TKernel TKMath TKBool TKBO TKPrim TKTopAlgo TKV3d TKService TKBin TKBRep TKFillet TKOffset TKOpenGl TKGeomBase TKGeomAlgo TKG3d )
if( OpenCascade_FOUND )
	include_directories( SYSTEM ${OpenCascade_INCLUDE_DIR} )
	link_directories( ${OpenCascade_LIBRARY_DIR} )
endif( OpenCascade_FOUND )

# -- Qt5 -------------------------------------------------
# Qt5Widgets finds its own dependencies (Qt5::QtGui and Qt5::QtCore).
find_package(Qt5Widgets)

find_package(Qt5OpenGL)

if( Qt5Core_FOUND )
	# Qt5 version information
	message( STATUS "Found Qt5 version: ${Qt5Core_VERSION_STRING}" )
endif()

set( Qt5_LIBRARIES Qt5::Widgets Qt5::OpenGL )

# Core
include_directories( Source )
add_subdirectory( Source )