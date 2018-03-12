#pragma once
#include "KeyValuePair.h"
#include "WeightedDirectedGraph.h"
#include "Utils.h"
#include <stdlib.h>
#include <queue>
#include <stack>
#include <list>
#include <iostream>
#include <omp.h>
#include <chrono>
#include <cfloat>
#include <cmath>
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

	~Betweenness();
};
