project( Core )

## SOURCES

set( Data_HEADERS
	Data/DesignRulesDBConnector.h
)
set( Data_SOURCES
	Data/DesignRulesDBConnector.cpp
)

set( Equation_HEADERS
	Equation/Equation.h
)
set( Equation_SOURCES
)

set( Exception_HEADERS
	Exception/Exception.h
)
set( Exception_SOURCES
	Exception/Exception.cpp
)

set( Parameter_HEADERS
	Parameter/IParameter.h
	Parameter/IParameterTagCollection.h
	Parameter/Parameter.h
	Parameter/Parameter_IdentificationGenerator.h
	Parameter/ParameterContainer.h
	Parameter/Exception.h
)
set( Parameter_SOURCES

)

set( SQLiteCpp_HEADERS
	SQLiteCpp/Assertion.h
	SQLiteCpp/Backup.h
	SQLiteCpp/Column.h
	SQLiteCpp/Database.h
	SQLiteCpp/Exception.h
	SQLiteCpp/SQLiteCpp.h
	SQLiteCpp/Statement.h
	SQLiteCpp/Transaction.h
	SQLiteCpp/VariadicBind.h
)
set( SQLiteCpp_SOURCES
	SQLiteCpp/Backup.cpp
	SQLiteCpp/Column.cpp
	SQLiteCpp/Database.cpp
	SQLiteCpp/Exception.cpp
	SQLiteCpp/Statement.cpp
	SQLiteCpp/Transaction.cpp
)

set( ThreadPool_HEADERS
	ThreadPool/ThreadPool.h
)
set( ThreadPool_SOURCES
	ThreadPool/ThreadPool.cpp
)

# Set sources and headers
set( Base_SOURCES 
	${Data_SOURCES}
	${Equation_SOURCES}
	${Exception_SOURCES}
	${Parameter_SOURCES}
	${SQLiteCpp_SOURCES}
	${ThreadPool_SOURCES}
)

set( Base_HEADERS 
	${Data_HEADERS}
	${Equation_HEADERS}
	${Exception_HEADERS}
	${Parameter_HEADERS}
	${SQLiteCpp_HEADERS}
	${ThreadPool_HEADERS}
)

# Setup where to place built library
#set( LIBRARY_OUTPUT_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )

add_library( ${PROJECT_NAME} SHARED ${Base_SOURCES} ${Base_HEADERS} )

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
)