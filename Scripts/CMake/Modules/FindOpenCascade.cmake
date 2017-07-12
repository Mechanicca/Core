# Try to find OpenCascade Community Edition (OCE)
#
# Use this module by invoking find_package with the form::
#
#   find_package( OpenCascade 
#     [REQUIRED]             # Fail with error if OpenCascade is not found [UNTESTED!]
#     [COMPONENTS <libs>...] # OpenCascade libraries by their canonical name 
#     )                      # e.g. TKernel TMath 
#
# Once done this will define:
#
# OpenCascade_FOUND	- System has OpenCascade installed
# OpenCascade_INCLUDE_DIR - Where the OCE include directory can be found
# OpenCascade_LIBRARY_DIR - Where the OCE library directory can be found
# OpenCascade_LIBRARIES   - OCE shared libraries to link with
# 
# Author: Martin Kopecky <martin.monster696@gmail.com>

find_package( OCE QUIET )

if( OCE_FOUND )
	set( OpenCascade_FOUND ${OCE_FOUND} )
	
	set( OpenCascade_INCLUDE_DIR ${OCE_INCLUDE_DIRS} )
	
	find_library( OCC_LIBRARY TKernel /usr/lib )
	
	if( OCC_LIBRARY )
		get_filename_component( OpenCascade_LIBRARY_DIR ${OCC_LIBRARY} PATH )
	endif( OCC_LIBRARY )
	
	# Required OCE libraries
	#set( OpenCascade_LIBRARIES ${OCE_LIBRARIES} )
	if( OpenCascade_FIND_COMPONENTS )
		foreach( _libname ${OpenCascade_FIND_COMPONENTS} )
			find_library( ${_libname}_OCCLIB ${_libname} ${OpenCascade_LIBRARY_DIR} NO_DEFAULT_PATH )
			set( _foundlib ${${_libname}_OCCLIB} )
			if( _foundlib STREQUAL ${_libname}_OCCLIB-NOTFOUND )
				message( FATAL_ERROR "Cannot find ${_libname} in ${OpenCascade_LIBRARY_DIR}" )
			endif()
			set( OpenCascade_LIBRARIES ${OpenCascade_LIBRARIES} ${_foundlib} )
		endforeach()
	else()
		message( FATAL_ERROR "Required OpenCascade libraries must be specified i.e 'find_package( OpenCascade REQUIRED COMPONENTS TKernel TKMath )'" )
	endif()
	
	# OpenCascade version information
	if( OpenCascade_INCLUDE_DIR )
	  file(STRINGS ${OpenCascade_INCLUDE_DIR}/Standard_Version.hxx OCC_MAJOR
	    REGEX "#define OCC_VERSION_MAJOR.*"
	  )
	  string(REGEX MATCH "[0-9]+" OCC_MAJOR ${OCC_MAJOR})
	  file(STRINGS ${OpenCascade_INCLUDE_DIR}/Standard_Version.hxx OCC_MINOR
	    REGEX "#define OCC_VERSION_MINOR.*"
	  )
	  string(REGEX MATCH "[0-9]+" OCC_MINOR ${OCC_MINOR})
	  file(STRINGS ${OpenCascade_INCLUDE_DIR}/Standard_Version.hxx OCC_MAINT
	    REGEX "#define OCC_VERSION_MAINTENANCE.*"
	  )
	  string(REGEX MATCH "[0-9]+" OCC_MAINT ${OCC_MAINT})
	
	  set(OpenCascade_VERSION_STRING "${OCC_MAJOR}.${OCC_MINOR}.${OCC_MAINT}")
	endif()
	
endif( OCE_FOUND )

# Summary
if( OpenCascade_FOUND )
	message( STATUS "Found OpenCascade Community Edition (OCE) version: ${OpenCascade_VERSION_STRING}" )
	#message( STATUS "OpenCascade include directory: ${OpenCascade_INCLUDE_DIR}" )
	#message( STATUS "OpenCascade shared libraries directory: ${OpenCascade_LIBRARY_DIR}" )
	#message( STATUS "OpenCascade shared libraries: ${OpenCascade_LIBRARIES}" )
else()
	set( OpenCascade_FOUND FALSE )
	message( SEND_ERROR "OpenCascade NOT found." )
endif()
