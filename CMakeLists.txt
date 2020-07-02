cmake_minimum_required(VERSION 3.1.0)

project(BlackBodyColorCalculator C CXX)

add_library(BlackbodyLib STATIC
	${CMAKE_CURRENT_SOURCE_DIR}/src/units.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/blackbody.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/blackbody.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/cie_xyz.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/cie_xyz.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/util.h)
# On GCC etc we need to link against libm for the math functions pow etc.
if(NOT MSVC)
	target_link_libraries(BlackbodyLib PUBLIC m)
endif()

add_executable(BlackBodyCalc
	${CMAKE_CURRENT_SOURCE_DIR}/src/main.c)
set_target_properties(BlackBodyCalc PROPERTIES
	C_STANDARD 99)
target_link_libraries(BlackBodyCalc BlackbodyLib)

# Testing
# Force gtest to use the shared version of the CRT, otherwise there'll be incompatibilities between it and the other targets
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "Enable mocking functionality" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "Install gtest" FORCE)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/deps/googletest")

enable_testing()
add_executable(BlackBodyTest ${CMAKE_CURRENT_SOURCE_DIR}/test/main.cpp
						 ${CMAKE_CURRENT_SOURCE_DIR}/test/blackbody.cpp)
add_executable(CieXyzTest ${CMAKE_CURRENT_SOURCE_DIR}/test/main.cpp
						 ${CMAKE_CURRENT_SOURCE_DIR}/test/cie_xyz.cpp)
target_include_directories(BlackBodyTest PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(CieXyzTest PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(BlackBodyTest gtest gtest_main BlackbodyLib)
target_link_libraries(CieXyzTest gtest gtest_main BlackbodyLib)
add_test(NAME BlackBodyTest COMMAND BlackBodyTest)
add_test(NAME CieXyzTest COMMAND CieXyzTest)