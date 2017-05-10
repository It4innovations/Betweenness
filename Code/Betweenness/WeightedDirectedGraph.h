#pragma once
#include <list>
#include <stdlib.h>
#include "Edge.h"
#include <set>
#include <unordered_map>
#include <map>
#include <queue>
#include <stack>
#include "KeyValuePair.h"
#include <cfloat>
#include <cstring>
#include <functional>

#include <fstream>
#include <iostream>
#include <sstream>

class WeightedDirectedGraph
{
private:
	//Later use some better function
	struct PairHash {
	public:
		template <typename T, typename U>
		std::size_t operator()(const std::pair<T, U> &x) const
		{
			return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
		}
	};

	vector<Edge> *adj;
	int vertices;
	int edges;
	int edgeIdCounter;
	unordered_map<pair<int, int>, int, PairHash> verticesToEdgeMap;
	pair<int, int> myPair;
public:
	WeightedDirectedGraph(int vertices);

	int GetVertices();
	int GetEdges();
	int GetEdgeId(int input, int output);
	void AddEdge(Edge & edge);
	void AddEdge(int input, int output, double weight);

	//Returns list of edges for a given vertex
	vector<Edge> & GetAdjacentVertices(int vertex);

	//Normalizes weights of edges in graph to <0,1> and returns denormalizeValue
	double NormalizeWeights();
	void DenormalizeWeights(double denormalizeValue);

	//This method returns subgraph with vertices around centerVertex that are accessible in path of distance length
	WeightedDirectedGraph * GetSubGraph(int centerVertex, double distance);

	~WeightedDirectedGraph();
};
