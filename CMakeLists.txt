cmake_minimum_required(VERSION 3.12)
project(DUNEPlotStyle VERSION 01.02 LANGUAGES CXX)

set(CMAKE_INSTALL_DEFAULT_DIRECTORY_PERMISSIONS
		OWNER_READ
		OWNER_WRITE
		OWNER_EXECUTE
		GROUP_READ
		GROUP_EXECUTE
		WORLD_READ
		WORLD_EXECUTE
)

add_library(DUNEPlotStyle INTERFACE)
target_include_directories(DUNEPlotStyle INTERFACE
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/root/cpp/include>
		$<INSTALL_INTERFACE:include>
		${ROOT_INCLUDE_DIRS}
)
set_target_properties(DUNEPlotStyle PROPERTIES
		PUBLIC_HEADER
		src/root/cpp/include/DUNEStyle.h
)

install(TARGETS DUNEPlotStyle
		EXPORT DUNEPlotStyleTargets
		LIBRARY DESTINATION lib/
		PUBLIC_HEADER DESTINATION include/)

find_package(ROOT)
if (ROOT_FOUND)
	add_executable(DUNEPlotStyleExample examples/root/cpp/example.C)
	target_link_libraries(DUNEPlotStyleExample DUNEPlotStyle ROOT::Core ROOT::Hist ROOT::Gpad ROOT::Postscript)
	install(TARGETS DUNEPlotStyleExample)
endif()

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
		"${PROJECT_BINARY_DIR}/DUNEPlotStyleConfigVersion.cmake"
		VERSION ${CMAKE_PROJECT_VERSION}
		COMPATIBILITY AnyNewerVersion
)
configure_package_config_file(
		"${PROJECT_SOURCE_DIR}/cmake/templates/DUNEPlotStyleConfig.cmake.in"
		"${PROJECT_BINARY_DIR}/DUNEPlotStyleConfig.cmake"
		INSTALL_DESTINATION share/cmake/DUNEPlotStyle
)
install(EXPORT DUNEPlotStyleTargets DESTINATION share/cmake/DUNEPlotStyle)
install(FILES "${PROJECT_BINARY_DIR}/DUNEPlotStyleConfigVersion.cmake"
		"${PROJECT_BINARY_DIR}/DUNEPlotStyleConfig.cmake"
		DESTINATION share/cmake/DUNEPlotStyle)