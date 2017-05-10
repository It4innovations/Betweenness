#include "BetweennessResult.h"

BetweennessResult::BetweennessResult()
{
}

BetweennessResult::BetweennessResult(double * vertexBetweenness, double * edgeBetweenness)
{
	this->VertexBetweenness = vertexBetweenness;
	this->EdgeBetweenness = edgeBetweenness;
}


BetweennessResult::~BetweennessResult()
{
}
