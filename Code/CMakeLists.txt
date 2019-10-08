cmake_minimum_required(VERSION 2.8.12)
project(Betweenness CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -gdwarf-2")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -xHost")
  #set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -xHost -qopt-report=5")
else()
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")
endif()

find_package(OpenMP REQUIRED COMPONENTS CXX)
if(OpenMP_CXX_FOUND)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

find_package(MPI REQUIRED COMPONENTS CXX)
if(MPI_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MPI_CXX_COMPILE_OPTIONS}")
    add_definitions(${MPI_CXX_COMPILE_DEFINITIONS})
    include_directories(${MPI_CXX_INCLUDE_DIRS})
endif()

set(BTW_SOURCE Betweenness/Betweenness.cpp
			Betweenness/BetweennessResult.cpp
			Betweenness/Edge.cpp
			Betweenness/KeyValuePair.cpp
			Betweenness/Utils.cpp
			Betweenness/WeightedDirectedGraph.cpp
            Betweenness/CsvReader.cpp
			Betweenness/main.cpp)

add_executable(betweenness-double ${BTW_SOURCE})
target_compile_definitions(betweenness-double PUBLIC -DNUM_DOUBLE)
target_link_libraries(betweenness-double ${MPI_CXX_LIBRARIES} ${OpenMP_CXX_LIBRARIES})

add_executable(betweenness-float ${BTW_SOURCE})
target_compile_definitions(betweenness-float PUBLIC -DNUM_SINGLE)
target_link_libraries(betweenness-float ${MPI_CXX_LIBRARIES} ${OpenMP_CXX_LIBRARIES})

add_executable(betweenness-double-ext ${BTW_SOURCE})
target_compile_definitions(betweenness-double-ext PUBLIC -DNUM_DOUBLE -DBTW_USE_EXTENSION)
target_link_libraries(betweenness-double-ext ${MPI_CXX_LIBRARIES} ${OpenMP_CXX_LIBRARIES})
