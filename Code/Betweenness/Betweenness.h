#pragma once
#include "KeyValuePair.h"
#include "WeightedDirectedGraph.h"
#include "Utils.h"
#include "Types.h"
#include <stdlib.h>
#include <queue>
#include <stack>
#include <list>
#include <iostream>
#include <omp.h>
#include <chrono>
#include <cfloat>
#include <cmath>
#include <mpi.h>
#include <functional> //greater
#include <unordered_set>
#include "BetweennessResult.h"

//Betweenness algorithm calculates betweenness values for vertices and also edges
class Betweenness
{
private:
	WeightedDirectedGraph *graph;
public:
	Betweenness(WeightedDirectedGraph &graph);
	//Serial version of betweenness
	BetweennessResult Calculate(int startVertex, int endVertex);
	//OpenMP version of betweenness
	BetweennessResult CalculateOpenMP(int startVertex, int endVertex, int threads);
	//btw_num_t* CalculateOpenMP2(int startVertex, int endVertex, int threads);
	BetweennessResult CalculateMpi(int startVertex, int endVertex, int threads, int chunkSize);
	~Betweenness();
};
