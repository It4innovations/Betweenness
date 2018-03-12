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
void WriteResult(double * bw, double * ebw, WeightedDirectedGraph * graph, string filename, std::vector<int> * verticesNewToOld, std::vector<int> * edgesNewToOld);
WeightedDirectedGraph * ReadGraph(string fileName, std::vector<int> * verticesNewToOld, std::vector<int> * edgesNewToOld);


int main(int argc, char* argv[])
{
	string file = "graph.csv";
	int startVertex = 0;
	int endVertex = 0;

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
				file = argv[i + 1];
			}
			else if (argument == "-s")//start vertex
			{
				startVertex = atoi(argv[i + 1]);
			}
			else if (argument == "-e")//end vertex
			{
				endVertex = atoi(argv[i + 1]);
			}
			i++;
		}
	}

	std::vector<int> * verticesNewToOld = new std::vector<int>();
	std::vector<int> * edgesNewToOld = new std::vector<int>();

	WeightedDirectedGraph *graph = ReadGraph(file, verticesNewToOld, edgesNewToOld);//if alpha and beta string is "" then default alpha and beta values are used (1, 1, ...)
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
	WriteResult(betweenness, edgeBetweenness, graph, file, verticesNewToOld, edgesNewToOld);
	cout << "Results written ..." << endl;

	delete[] betweenness;
	delete[] edgeBetweenness;

	delete verticesNewToOld;
	delete edgesNewToOld;

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

void WriteResult(double * bw, double * ebw, WeightedDirectedGraph * graph, string filename, std::vector<int> * verticesNewToOld, std::vector<int> * edgesNewToOld)
{
	ofstream file(filename + "_result_edge_betweenness.csv");

	file << "ID;VALUE" << endl;
	file.setf(ios::fixed);
	file.precision(4);

	for (size_t i = 0; i < graph->GetEdges(); i++)
	{
		int id = (*(edgesNewToOld))[i];
		file << id << ";" << ebw[i] << endl;
	}

	file.close();

	ofstream file2(filename + "_result_vertex_betweenness.csv");
	file2 << "ID;VALUE" << endl;
	file2.setf(ios::fixed);
	file2.precision(4);

	for (size_t i = 0; i < graph->GetVertices(); i++)
	{
		int id = (*(verticesNewToOld))[i];
		file2 << id << ";" << bw[i] << endl;
	}

	file2.close();
}

WeightedDirectedGraph * ReadGraph(string fileName, std::vector<int> * verticesNewToOld, std::vector<int> * edgesNewToOld)
{
	//first line is head id1;id2;dist;edge_id
	//other lines are edges 35373027;35405905;101;42060111

	//Load graph
	CsvReader reader(';');
	ifstream inputFileStream(fileName);//open the file
	string line;
	getline(inputFileStream, line);//read first line, which is head

	std::map<int, int> *vertexOldToNewMap = new std::map<int, int>();
	std::vector<Edge> edges;

	int edgeCounter = 0;
	while (getline(inputFileStream, line))
	{
		istringstream iss(line);
		iss >> reader;
		int v = stoi(reader[0]);
		int w = stoi(reader[1]);
		int weight = stoi(reader[2]);
		int edgeId = stoi(reader[3]);

		int newV, newW;

		//Insert first edge
		int count = vertexOldToNewMap->size();
		if (vertexOldToNewMap->count(v))//if map contains key return its value
		{
			newV = (*vertexOldToNewMap)[v];
		}
		else//put the key to map and return its value = count
		{
			(*vertexOldToNewMap).insert(std::pair<int, int>(v, count));
			newV = count;
			verticesNewToOld->push_back(v);
		}

		//Insert second edge
		count = vertexOldToNewMap->size();
		if (vertexOldToNewMap->count(w))//if map contains key return its value
		{
			newW = (*vertexOldToNewMap)[w];
		}
		else//put the key to map and return its value = count
		{
			(*vertexOldToNewMap).insert(std::pair<int, int>(w, count));
			newW = count;
			verticesNewToOld->push_back(w);
		}


		edgesNewToOld->push_back(edgeId);

		edges.push_back(Edge(edgeCounter, newV, newW, weight));
		edgeCounter++;
	}
	inputFileStream.close();


	int vertices = vertexOldToNewMap->size();
	WeightedDirectedGraph *graph = new WeightedDirectedGraph(vertices);

	for (size_t i = 0; i < edges.size(); i++)
	{
		graph->AddEdge(edges[i].GetId(), edges[i].GetInput(), edges[i].GetOutput(), edges[i].GetWeight());//remaping indexes of edges
	}

	delete vertexOldToNewMap;
	return graph;
}