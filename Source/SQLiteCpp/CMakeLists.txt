project( SQLiteCpp )

add_library( ${PROJECT_NAME} STATIC "" )

target_sources( ${PROJECT_NAME}
	INTERFACE
		"${CMAKE_CURRENT_LIST_DIR}/SQLiteCpp.h"
	PRIVATE
		"${CMAKE_CURRENT_LIST_DIR}/Assertion.h"
		"${CMAKE_CURRENT_LIST_DIR}/Backup.h"
		"${CMAKE_CURRENT_LIST_DIR}/Backup.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/Column.h"
		"${CMAKE_CURRENT_LIST_DIR}/Column.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/Database.h"
		"${CMAKE_CURRENT_LIST_DIR}/Database.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/Exception.h"
		"${CMAKE_CURRENT_LIST_DIR}/Exception.cpp"	
		"${CMAKE_CURRENT_LIST_DIR}/Statement.h"
		"${CMAKE_CURRENT_LIST_DIR}/Statement.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/Transaction.h"
		"${CMAKE_CURRENT_LIST_DIR}/Transaction.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/VariadicBind.h"
)

target_compile_options( ${PROJECT_NAME} 
    PUBLIC 
        -fvisibility=hidden 
        -fvisibility-inlines-hidden 
        -Wl,--no-undefined 
        -fPIC
)

target_link_libraries( ${PROJECT_NAME} 
    ${SQLITE3_LIBRARY} 
)