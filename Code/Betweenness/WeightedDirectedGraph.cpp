#include "WeightedDirectedGraph.h"

WeightedDirectedGraph::WeightedDirectedGraph(int vertices)
{
	this->vertices = vertices;
	adj = new vector<Edge>[vertices];	//array of vectors of adj vertices to vertex represented by index
	edgeCount = -1;
	alpha = new btw_num_t[vertices];
	beta = new btw_num_t[vertices];

	for (size_t i = 0; i < vertices; i++)
	{
		alpha[i] = 1;
		beta[i] = 1;
	}
}

int WeightedDirectedGraph::GetVertices()
{
	return vertices;
}

Edge * WeightedDirectedGraph::GetEdge(int id)
{
	return edges[id];
}

int WeightedDirectedGraph::GetEdges()
{
	if (edgeCount == -1)
	{
		edgeCount = 0;
		for (int i = 0; i < vertices; i++)
		{
			edgeCount += adj[i].size();
		}
	}

	return edgeCount;
}

void WeightedDirectedGraph::AddEdge(Edge & edge)
{
	adj[edge.GetInput()].push_back(edge);//insert edge to adjacency "list" (uses vectors)
	edges[edge.GetId()] = new Edge(edge.GetId(), edge.GetInput(), edge.GetOutput(), edge.GetWeight());//insert edge also to map of edges, for fast access by id
}

void WeightedDirectedGraph::AddEdge(int id, int input, int output, btw_num_t length)
{
	Edge edge(id, input, output, length);
	AddEdge(edge);
}

vector<Edge> & WeightedDirectedGraph::GetAdjacentVertices(int vertex)
{
	return adj[vertex];
}

btw_num_t WeightedDirectedGraph::NormalizeWeights()
{
	btw_num_t maxWeight = 0;
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

void WeightedDirectedGraph::DenormalizeWeights(btw_num_t denormalizeValue)
{
	btw_num_t maxWeight = denormalizeValue;

	for (size_t i = 0; i < vertices; i++)
	{
		for (auto &adjEdge : adj[i])
		{
			adjEdge.SetWeight(adjEdge.GetWeight() * maxWeight);
		}
	}

	for (auto val : edges)
	{
		val.second->SetWeight(val.second->GetWeight() * maxWeight);
	}
}

btw_num_t * WeightedDirectedGraph::GetAlpha()
{
	return alpha;
}

btw_num_t * WeightedDirectedGraph::GetBeta()
{
	return beta;
}

WeightedDirectedGraph::~WeightedDirectedGraph()
{	
	delete[] adj;	//delete array of vectors
	delete[] alpha;
	delete[] beta;

	for (auto val : edges)
	{
		delete val.second;
	}
	edges.clear();
}