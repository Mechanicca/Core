target_sources( ${PROJECT_NAME}
	INTERFACE
		"${CMAKE_CURRENT_LIST_DIR}/IComponent.h"
		"${CMAKE_CURRENT_LIST_DIR}/IComponentDesigner.h"
		"${CMAKE_CURRENT_LIST_DIR}/IComponentModel.h"
		"${CMAKE_CURRENT_LIST_DIR}/IComponentPluginAPI.h"
	PUBLIC
		"${CMAKE_CURRENT_LIST_DIR}/Assembly.h"
		"${CMAKE_CURRENT_LIST_DIR}/Component.h"
		"${CMAKE_CURRENT_LIST_DIR}/ComponentDesigner.h"
		"${CMAKE_CURRENT_LIST_DIR}/ComponentModel.h"
		"${CMAKE_CURRENT_LIST_DIR}/ComponentModelModifier.h"
		"${CMAKE_CURRENT_LIST_DIR}/Exception.h"	
		"${CMAKE_CURRENT_LIST_DIR}/Factory.h"			
	PRIVATE
		"${CMAKE_CURRENT_LIST_DIR}/Factory.cpp"
)