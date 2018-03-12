cmake_minimum_required(VERSION 2.8.12)
project(Betweenness CXX)

set(CMAKE_CXX_COMPILER "mpicc")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -std=c++11 -qopenmp")

set(BTW_SOURCE Betweenness/Betweenness.cpp
			Betweenness/BetweennessResult.cpp
			Betweenness/Edge.cpp
			Betweenness/KeyValuePair.cpp
			Betweenness/Utils.cpp
			Betweenness/WeightedDirectedGraph.cpp
            Betweenness/CsvReader.cpp
			Betweenness/main.cpp)

add_executable(betweenness ${BTW_SOURCE})