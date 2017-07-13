target_sources( ${PROJECT_NAME}
	INTERFACE
		"${CMAKE_CURRENT_LIST_DIR}/IParameter.h"
		"${CMAKE_CURRENT_LIST_DIR}/IParameterTagCollection.h"
	PUBLIC
		"${CMAKE_CURRENT_LIST_DIR}/Parameter.h"
		"${CMAKE_CURRENT_LIST_DIR}/ParameterContainer.h"
		"${CMAKE_CURRENT_LIST_DIR}/Exception.h"
	PRIVATE
		"${CMAKE_CURRENT_LIST_DIR}/ParameterIdentificationGenerator.h"
)