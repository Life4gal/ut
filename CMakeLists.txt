cmake_minimum_required(VERSION 3.25)

set(
	PROMETHEUS_UT_MAJOR_VERSION 
	1
	CACHE STRING 
	"PROMETHEUS-UT major version"
)
set(
	PROMETHEUS_UT_MINOR_VERSION 
	1 
	CACHE STRING 
	"PROMETHEUS-UT minor version"
)
set(
	PROMETHEUS_UT_PATCH_VERSION 
	0
	CACHE STRING 
	"PROMETHEUS-UT patch version"
)
set(
	PROMETHEUS_UT_VERSION 
	"${PROMETHEUS_UT_MAJOR_VERSION}.${PROMETHEUS_UT_MINOR_VERSION}.${PROMETHEUS_UT_PATCH_VERSION}" 
	CACHE STRING 
	"PROMETHEUS-UT version"
)

project(
		PrometheusUT
		VERSION ${PROMETHEUS_UT_VERSION}
		DESCRIPTION "prometheus-ut"
		HOMEPAGE_URL "https://github.com/Life4gal/ut"
		LANGUAGES CXX
)

# ===================================================================================================
# OPTION

option(PROMETHEUS_UT_TEST "Generate the test target." ON)
option(PROMETHEUS_UT_INSTALL "Install targets and generate package config." ${PROJECT_IS_TOP_LEVEL})

# ===================================================================================================
# DEFINES

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/defines.cmake)

# ===================================================================================================
# DEPENDENCIES

# packageProject
#CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.13.0")

# ===================================================================================================
# PACK

set(PROMETHEUS_UT_VERSION_HEADER_FILE ${CMAKE_CURRENT_BINARY_DIR}/include/prometheus/version-ut.hpp)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.hpp.in
	${PROMETHEUS_UT_VERSION_HEADER_FILE}
	@ONLY
)

# ===================================================================================================
# LIBRARY

#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#set(CMAKE_POSITION_INDEPENDENT_CODE ON)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(${PROJECT_NAME} INTERFACE)

#set_target_properties(
#	${PROJECT_NAME} PROPERTIES
#	VERSION ${PROMETHEUS_UT_VERSION}
#	SOVERSION ${PROMETHEUS_UT_MAJOR_VERSION}
#)

target_include_directories(
	${PROJECT_NAME} 

	INTERFACE 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	# version header file
	$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

target_compile_options(
	${PROJECT_NAME}

	INTERFACE
	${PROMETHEUS_COMPILE_FLAGS}
)

#target_compile_definitions(
#	${PROJECT_NAME}
#	INTERFACE
#	
#)

target_compile_features(
	${PROJECT_NAME}
	INTERFACE
	cxx_std_23
)

target_link_libraries(
	${PROJECT_NAME} 
	INTERFACE 
	PrometheusCore
)

set(
	PROMETHEUS_UT_SOURCE_FILES_PUBLIC

	${PROJECT_SOURCE_DIR}/include/prometheus/ut/def.hpp
	${PROJECT_SOURCE_DIR}/include/prometheus/ut/events.hpp
	${PROJECT_SOURCE_DIR}/include/prometheus/ut/operands.hpp
	${PROJECT_SOURCE_DIR}/include/prometheus/ut/executor.hpp
	${PROJECT_SOURCE_DIR}/include/prometheus/ut/dispatcher.hpp

	${PROJECT_SOURCE_DIR}/include/prometheus/ut/unit_test.hpp
)

set_source_files_properties(
	${PROMETHEUS_UT_SOURCE_FILES_PUBLIC}
	PROPERTIES
	LANGUAGE CXX
)

target_sources(
	${PROJECT_NAME}
	PUBLIC
	FILE_SET public_header_files
	TYPE HEADERS
	BASE_DIRS 
	${CMAKE_CURRENT_SOURCE_DIR}/include
	# version header file
	${CMAKE_CURRENT_BINARY_DIR}/include
	FILES
	${PROMETHEUS_UT_SOURCE_FILES_PUBLIC}
	${PROMETHEUS_UT_VERSION_HEADER_FILE}
)

# ===================================================================================================
# TESTS

if (PROMETHEUS_UT_TEST)
	enable_testing()
	add_subdirectory(test)
endif (PROMETHEUS_UT_TEST)

# ===================================================================================================
# INSTALL

if (PROMETHEUS_UT_INSTALL)
	include(GNUInstallDirs)
	include(CMakePackageConfigHelpers)

	install(
		TARGETS ${PROJECT_NAME}
		EXPORT ${PROJECT_NAME}Targets
		FILE_SET public_header_files DESTINATION include
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		INCLUDES DESTINATION include
	)

	install(
		EXPORT ${PROJECT_NAME}Targets
		FILE ${PROJECT_NAME}Targets.cmake
		NAMESPACE ${PROJECT_NAME}::
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
	)

	configure_package_config_file(
		${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
		${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
		INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
	)

	write_basic_package_version_file(
		${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
		VERSION ${PROMETHEUS_UT_VERSION}
		COMPATIBILITY SameMajorVersion
	)

	install(
		FILES
		${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
		${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
	)

	configure_file(
		${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}.pc.in
		${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc
		@ONLY
	)

	install(
		FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
	)

	set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
	set(CPACK_PACKAGE_VENDOR ${PROJECT_NAME})
	set(CPACK_PACKAGE_VERSION ${PROMETHEUS_UT_VERSION})
	set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Prometheus Unit Test Library")
	set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
	set(CPACK_DEBIAN_PACKAGE_DEPENDS "")
	set(CPACK_RPM_PACKAGE_REQUIRES "")

	include(CPack)
endif (PROMETHEUS_UT_INSTALL)
