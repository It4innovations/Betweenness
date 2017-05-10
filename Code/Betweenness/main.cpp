#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <queue>
#include "KeyValuePair.h"
#include "WeightedDirectedGraph.h"
#include "Betweenness.h"
#include <chrono>
#include <cfloat>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <string>

void PrintUsage();
WeightedDirectedGraph * ReadGraph(string fileName);
void WriteGraph(string fileName, WeightedDirectedGraph * graph);
void WriteResult(double * bw, double * ebw, WeightedDirectedGraph * graph, string filename);


int main(int argc, char* argv[])
{
	string fileName = "graph.csv";
	int startVertex = 0;
	int endVertex = 0;
	int distance = 1000;

	for (size_t i = 1; i < argc; i++)
	{
		string argument = string(argv[i]);
		if (argument == "-h" || argument == "-help" || argument == "--help")
		{
			PrintUsage();
			exit(0);
		}
		else
		{
			if (i + 1 == argc)
			{
				PrintUsage();
				exit(0);
			}

			if (argument == "-f")//file path with graph
			{
				fileName = argv[i + 1];
			}
			else if (argument == "-s")//start vertex
			{
				startVertex = atoi(argv[i + 1]);
			}
			else if (argument == "-e")//end vertex
			{
				endVertex = atoi(argv[i + 1]);
			}
			else if (argument == "-d")//distance for subgraph generation
			{
				distance = atoi(argv[i + 1]);
			}
			i++;
		}
	}

	//cout << "Reading graph ..." << endl;
	WeightedDirectedGraph *graph = ReadGraph(fileName);
	//cout << "Graph loaded ..." << endl;
	//cout << "Weights normalized to interval <0,1> ..." << endl;

	//****** PREPARE SUBGRAPHS **********************************************************//
	///////////////////////////////////////////////////////////////////////////////////////
	//WeightedDirectedGraph *subGraph = graph->GetSubGraph(0, distance);
	//cout << "Subgraph created ..." << endl;
	//WriteGraph("CR" + to_string(distance) + ".csv", subGraph);
	//cout << "Subraph written ..." << endl;
	//delete subGraph;
	//delete graph;
	//exit(0);
	///////////////////////////////////////////////////////////////////////////////////////
	//****** PREPARE SUBGRAPHS **********************************************************//

	graph->NormalizeWeights();

	if (startVertex < 0 || startVertex > graph->GetVertices())
	{
		startVertex = 0;
	}

	if (endVertex <= 0 || endVertex > graph->GetVertices())
	{
		endVertex = graph->GetVertices();
	}

	Betweenness *bb = new Betweenness(*graph);
	auto start_time = chrono::high_resolution_clock::now();

	BetweennessResult result = bb->Calculate(startVertex, endVertex);

	auto end_time = chrono::high_resolution_clock::now();
	auto time = end_time - start_time;
	cout << "Betweenness took " <<
		chrono::duration_cast<chrono::milliseconds>(time).count() << " ms to run.\n";

	double *betweenness = result.VertexBetweenness;
	double *edgeBetweenness = result.EdgeBetweenness;
	WriteResult(betweenness, edgeBetweenness, graph, fileName);
	cout << "Results written ..." << endl;

	delete[] betweenness;
	delete[] edgeBetweenness;

	delete graph;
	delete bb;

	return 0;
}

void PrintUsage()
{
	cout << endl;
	cout << "Help for betweenness algorithm parameters" << endl;
	cout << "Usage: ./betweenness.exe -f <file> -s <start> -e <end>" << endl;

	cout << "Where:" << endl;
	cout << " <file>: \tFile with graph in .csv format" << endl;
	cout << " <start>: \tFrom which vertex id calculate betweenness [default=0]" << endl;
	cout << " <end>: \tTo which vertex id calculate [default=last vertex of graph]" << endl;
	cout << endl;
}

WeightedDirectedGraph * ReadGraph(string fileName)
{
	//first line contains number of vertices
	//other lines contains 3 values input output weight delimited by space
	WeightedDirectedGraph *graph = nullptr;
	ifstream file(fileName);
	string line;
	int c = 0;

	getline(file, line);
	int vertices;
	istringstream iss(line);
	iss >> vertices;
	graph = new WeightedDirectedGraph(vertices);

	while (getline(file, line))
	{
		istringstream iss(line);
		int v, w, weight;
		iss >> v >> w >> weight;
		graph->AddEdge(v, w, weight);
	}
	file.close();
	return graph;
}

void WriteGraph(string fileName, WeightedDirectedGraph * graph)
{
	ofstream file(fileName);
	file << graph->GetVertices() << "\n";

	for (int v = 0; v < graph->GetVertices(); v++)
	{
		for (auto &edge : graph->GetAdjacentVertices(v))
		{
			int w = edge.GetOutput();
			file << v << " " << w << " " << edge.GetWeight() << "\n";
		}
	}

	file.close();
}

void WriteResult(double * bw, double * ebw, WeightedDirectedGraph * graph, string filename)
{
	ofstream file(filename + "_result_edge_betweenness.csv");

	file << "ID;VALUE" << endl;
	file.setf(ios::fixed);
	file.precision(4);
	int counter = 0;
	for (size_t v = 0; v < graph->GetVertices(); v++)
	{
		for (auto &edge : graph->GetAdjacentVertices(v))
		{
			file << counter << ";" << ebw[counter] << endl;
			counter++;
		}
	}

	file.close();

	ofstream file2(filename + "_result_vertex_betweenness.csv");
	file2 << "ID;VALUE" << endl;
	file2.setf(ios::fixed);
	file2.precision(4);
	counter = 0;

	for (size_t v = 0; v < graph->GetVertices(); v++)
	{
		auto &edge = graph->GetAdjacentVertices(v)[0];
		if (&edge != nullptr)
			file2 << counter << ";" << bw[v] << endl;
		counter++;
	}

	file2.close();
}