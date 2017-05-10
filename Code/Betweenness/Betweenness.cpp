#include "Betweenness.h"


Betweenness::Betweenness(WeightedDirectedGraph &graph)
{
	this->graph = &graph;
}

BetweennessResult Betweenness::Calculate(int startVertex, int endVertex)
{
	cout << "start " << startVertex << ", end " << endVertex << endl;
	int vertices = graph->GetVertices();
	int edges = graph->GetEdges();
	double *betweenness = new double[vertices];
	double *edgeBetweenness = new double[edges];
	for (size_t i = 0; i < vertices; i++)
	{
		betweenness[i] = 0;
	}
	for (size_t i = 0; i < edges; i++)
	{
		edgeBetweenness[i] = 0;
	}

	priority_queue<KeyValuePair, vector<KeyValuePair>, greater<KeyValuePair>> Q;
	stack<int> S;
	double *dist = new double[vertices];
	list<int> *Pred;	//list of predecessors is faster
	double *sp = new double[vertices];
	double *delta = new double[vertices];
	bool *isInStack = new bool[vertices];

	for (int s = startVertex; s < endVertex; s++)
	{
		//Single source shortest-paths problem
		{
			//Initialization
			{
				Pred = new list<int>[vertices];
				for (int i = 0; i < vertices; i++)
				{
					dist[i] = DBL_MAX;
					sp[i] = 0;
					isInStack[i] = false;
				}
				dist[s] = 0;
				sp[s] = 1;
				Q.push(KeyValuePair(dist[s], s));
			}

			while (!Q.empty())
			{
				KeyValuePair pair = Q.top();
				int v = pair.Value;
				Q.pop();

				//this part of code should replace updating in priority queue which is expensive?
				if (isInStack[v] == false)
				{
					isInStack[v] = true;
					S.push(v);


					vector<Edge> adjEdges = graph->GetAdjacentVertices(v);	//vector of edges is faster than list
					for (auto &adjEdge : adjEdges)
					{
						double edgeDistance = adjEdge.GetWeight();

						int w = adjEdge.GetOutput();

						//path discovery, shorter path found?
						if (dist[w] > dist[v] + edgeDistance)
						{
							dist[w] = dist[v] + edgeDistance;
							Q.push(KeyValuePair(dist[w], w));
							sp[w] = 0;
							Pred[w].clear();
						}

						//path counting, edge(v,w) on a shortest path?
						if (Utils::AreEqual(dist[w], dist[v] + edgeDistance))
						{
							//sp[w] = sp[w] + edgeDistance*sp[v];
							sp[w] = sp[w] + sp[v];
							Pred[w].push_back(v);
						}
					}
				}
			}
		}

		//Accumulation
		{
			double c = 0;

			for (int v = 0; v < vertices; v++)
			{
				delta[v] = 0;
			}

			while (S.size() != 0)
			{
				int w = S.top();
				S.pop();

				for (auto v : Pred[w])
				{
					c = (sp[v] / sp[w] * (1 + delta[w]));
					//save edge betweenness
					int edgeId = graph->GetEdgeId(v, w);
					edgeBetweenness[edgeId] = edgeBetweenness[edgeId] + c;
					delta[v] = delta[v] + c;
				}
				if (w != s)
				{
					betweenness[w] = betweenness[w] + delta[w];
				}
			}
		}

		delete[] Pred;
	}

	delete[] dist;
	delete[] sp;
	delete[] delta;
	delete[] isInStack;
	return BetweennessResult(betweenness, edgeBetweenness);
}

Betweenness::~Betweenness()
{
}
