cmake_minimum_required(VERSION 2.8.12)
project(Betweenness CXX)

#set(CMAKE_CXX_COMPILER "icc")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -std=c++11")

set(BTW_SOURCE Betweenness/Betweenness.cpp
			Betweenness/BetweennessResult.cpp
			Betweenness/Edge.cpp
			Betweenness/KeyValuePair.cpp
			Betweenness/Utils.cpp
			Betweenness/WeightedDirectedGraph.cpp
			Betweenness/main.cpp)

add_executable(betweenness ${BTW_SOURCE})

set(BTW_MANIP_SOURCE
	BetweennessGraphManipulator/BetweennessGraphManipulator.cs)


add_custom_command(
	OUTPUT ${PROJECT_BINARY_DIR}/BetweennessGraphManipulator.exe
COMMAND mcs "${PROJECT_SOURCE_DIR}/${BTW_MANIP_SOURCE}" "-out:${PROJECT_BINARY_DIR}/BetweennessGraphManipulator.exe")

add_custom_target(betweenness_graph_manipulator ALL DEPENDS ${PROJECT_BINARY_DIR}/BetweennessGraphManipulator.exe)
