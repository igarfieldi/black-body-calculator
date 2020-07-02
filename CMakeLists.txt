cmake_minimum_required(VERSION 3.1.0)

project(BlackBodyColorCalculator C CXX)

add_library(CieXyz
	${CMAKE_CURRENT_SOURCE_DIR}/src/units.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/blackbody.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/blackbody.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/cie_xyz.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/cie_xyz.c)
add_executable(BlackBodyCalc
	${CMAKE_CURRENT_SOURCE_DIR}/src/main.c)
set_target_properties(BlackBodyCalc PROPERTIES
	C_STANDARD 99)
target_link_libraries(BlackBodyCalc m CieXyz)


# Testing
set(BUILD_GMOCK OFF CACHE BOOL "Enable mocking functionality")
set(INSTALL_GTEST OFF CACHE BOOL "Install gtest")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/deps/googletest")

enable_testing()
add_executable(BlackBodyTest ${CMAKE_CURRENT_SOURCE_DIR}/test/main.cpp
						 ${CMAKE_CURRENT_SOURCE_DIR}/test/blackbody.cpp)
add_executable(CieXyzTest ${CMAKE_CURRENT_SOURCE_DIR}/test/main.cpp
						 ${CMAKE_CURRENT_SOURCE_DIR}/test/cie_xyz.cpp)
target_include_directories(BlackBodyTest PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(CieXyzTest PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(BlackBodyTest gtest gtest_main CieXyz)
target_link_libraries(CieXyzTest gtest gtest_main CieXyz)
add_test(NAME BlackBodyTest COMMAND BlackBodyTest)
add_test(NAME CieXyzTest COMMAND CieXyzTest)