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

#include "CsvReader.h"
#include <map>

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
	map<int, Edge*> edges;

	int vertices;
	int edgeCount;

	void AddEdge(Edge & edge);
public:
	WeightedDirectedGraph(int vertices);

	int GetVertices();
	Edge * GetEdge(int id);
	int GetEdges();

	void AddEdge(int id, int input, int output, double length);

	//Returns list of edges for a given vertex
	vector<Edge> & GetAdjacentVertices(int vertex);

	//Normalizes weights of edges in graph to <0,1> and returns denormalizeValue
	double NormalizeWeights();
	void DenormalizeWeights(double denormalizeValue);

	~WeightedDirectedGraph();
};
