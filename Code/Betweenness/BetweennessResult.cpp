#include "BetweennessResult.h"

BetweennessResult::BetweennessResult()
{
}

BetweennessResult::BetweennessResult(btw_num_t * vertexBetweenness, btw_num_t * edgeBetweenness)
{
	this->VertexBetweenness = vertexBetweenness;
	this->EdgeBetweenness = edgeBetweenness;
}


BetweennessResult::~BetweennessResult()
{
}
