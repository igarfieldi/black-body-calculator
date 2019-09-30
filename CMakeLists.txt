cmake_minimum_required(VERSION 3.1.0)

project(BlackBodyColorCalculator)

add_executable(BlackBodyCalc
	${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/units.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/cie_xyz.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/cie_xyz.c)

set_target_properties(BlackBodyCalc PROPERTIES
	C_STANDARD 99)