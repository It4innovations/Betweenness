#pragma once
#include "Types.h"

class BetweennessResult
{
public:
	btw_num_t * VertexBetweenness;
	btw_num_t * EdgeBetweenness;
	BetweennessResult();
	BetweennessResult(btw_num_t * vertexBetweenness, btw_num_t * edgeBetweenness);
	~BetweennessResult();
};

