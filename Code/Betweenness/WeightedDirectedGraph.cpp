#include "WeightedDirectedGraph.h"

WeightedDirectedGraph::WeightedDirectedGraph(int vertices)
{
	this->vertices = vertices;
	adj = new vector<Edge>[vertices];	//array of lists of adj vertices to vertex represented by index
	edges = -1;
	edgeIdCounter = 0;
}

int WeightedDirectedGraph::GetVertices()
{
	return vertices;
}

int WeightedDirectedGraph::GetEdges()
{
	if (edges == -1)
	{
		edges = 0;
		for (int i = 0; i < vertices; i++)
		{
			edges += adj[i].size();
		}
	}

	return edges;
}

int WeightedDirectedGraph::GetEdgeId(int input, int output)
{
	for (auto &edge : adj[input])
	{
		if (edge.GetOutput() == output)
		{
			return edge.GetId();
		}
	}
	return -1;//should not be called, if input is OK
}

void WeightedDirectedGraph::AddEdge(Edge & edge)
{
	edge.SetId(edgeIdCounter);
	edgeIdCounter++;
	adj[edge.GetInput()].push_back(edge);
}

void WeightedDirectedGraph::AddEdge(int input, int output, double length)
{
	Edge edge(input, output, length);
	AddEdge(edge);
}

vector<Edge> & WeightedDirectedGraph::GetAdjacentVertices(int vertex)
{
	return adj[vertex];
}

double WeightedDirectedGraph::NormalizeWeights()
{
	double maxWeight = 0;
	for (size_t i = 0; i < vertices; i++)
	{
		for (auto &adjEdge : adj[i])
		{
			if (adjEdge.GetWeight() > maxWeight)
			{
				maxWeight = adjEdge.GetWeight();
			}
		}
	}

	for (size_t i = 0; i < vertices; i++)
	{
		for (auto &adjEdge : adj[i])
		{
			adjEdge.SetWeight(adjEdge.GetWeight() / maxWeight);
		}
	}

	return maxWeight;
}

void WeightedDirectedGraph::DenormalizeWeights(double denormalizeValue)
{
	double maxWeight = denormalizeValue;

	for (size_t i = 0; i < vertices; i++)
	{
		for (auto &adjEdge : adj[i])
		{
			adjEdge.SetWeight(adjEdge.GetWeight() * maxWeight);
		}
	}
}

//Rewrites vertices ids, inputs and outputs except first 0 vertex
WeightedDirectedGraph * WeightedDirectedGraph::GetSubGraph(int centerVertex, double distance)
{
	priority_queue<KeyValuePair, vector<KeyValuePair>, greater<KeyValuePair>> Q;
	stack<int> S;
	double *dist = new double[vertices];
	bool *isInStack = new bool[vertices];


	int s = centerVertex;
	//Single source shortest-paths problem
	{
		//Initialization
		{
			for (size_t i = 0; i < vertices; i++)
			{
				dist[i] = DBL_MAX;
				isInStack[i] = false;
			}
			
			dist[s] = 0;

			Q.push(KeyValuePair(dist[s], s));
		}

		while (!Q.empty())
		{
			KeyValuePair pair = Q.top();
			int v = pair.Value;
			Q.pop();

			if (isInStack[v] == false)
			{
				isInStack[v] = true;
				S.push(v);

				vector<Edge> adjEdges = this->GetAdjacentVertices(v);
				for (auto &adjEdge : adjEdges)
				{
					double edgeDistance = adjEdge.GetWeight();
					int w = adjEdge.GetOutput();

					if (dist[w] > dist[v] + edgeDistance)
					{
						dist[w] = dist[v] + edgeDistance;
						Q.push(KeyValuePair(dist[w], w));
					}
				}
			}
		}
	}

	set<int> inRangeVertices;
	for (int i = 0; i < vertices; i++)
	{
		if (dist[i] < distance)
		{
			inRangeVertices.insert(i);
		}
	}

	//normalize vertex numbers
	map<int, int> normalizedVertexPairs; //key is vertex id
	int normValue = 0;
	for (const auto& v : inRangeVertices)
	{
		normalizedVertexPairs.insert(make_pair(v, normValue));
		normValue++;
	}

	WeightedDirectedGraph *subGraph = new WeightedDirectedGraph(inRangeVertices.size());	//Id's are same
	for (const auto& v : inRangeVertices)
	{
		for (auto &edge : this->GetAdjacentVertices(v))
		{
			int w = edge.GetOutput();

			if (inRangeVertices.count(w) > 0)//if w is also in range then create edge in subgraph
			{
				int normV = normalizedVertexPairs[v];
				int normW = normalizedVertexPairs[w];
				subGraph->AddEdge(normV, normW, edge.GetWeight());
			}
		}
	}

	return subGraph;
}

WeightedDirectedGraph::~WeightedDirectedGraph()
{
	delete[] adj;	//delete array of lists
}