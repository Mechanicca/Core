project( Core )

add_library( ${PROJECT_NAME} SHARED "" )

include( Component/CMakeLists.txt )
include( Data/CMakeLists.txt )
include( Equation/CMakeLists.txt )
include( Exception/CMakeLists.txt )
include( Parameter/CMakeLists.txt )
include( SQLiteCpp/CMakeLists.txt )
include( ThreadPool/CMakeLists.txt )

generate_export_header( ${PROJECT_NAME} EXPORT_FILE_NAME ${PROJECT_NAME}_Export.h )

target_compile_options( ${PROJECT_NAME} PUBLIC -fvisibility=hidden -fvisibility-inlines-hidden )

set_target_properties( ${PROJECT_NAME} 
	PROPERTIES 
#		VERSION ${${PROJECT_NAME}_VERSION} 
#		SOVERSION ${${PROJECT_NAME}_VERSION_MAJOR}
		LIBRARY_OUTPUT_DIRECTORY ${${APPLICATION_NAME}_Common_DIRECTORY}
)

target_link_libraries( ${PROJECT_NAME} 
	${SQLITE3_LIBRARY} 
	${Qt5_LIBRARIES} 
	${Boost_LIBRARIES}
	pthread
	dl
)