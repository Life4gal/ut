project(
	PrometheusUTTest
	LANGUAGES CXX
)

add_executable(
	${PROJECT_NAME}

	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
) 

target_link_libraries(
	${PROJECT_NAME}
	PRIVATE 
	PrometheusUT
)

add_test(
		NAME ${PROJECT_NAME}
		COMMAND ${PROJECT_NAME}
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
