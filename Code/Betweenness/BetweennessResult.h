#pragma once
class BetweennessResult
{
public:
	double * VertexBetweenness;
	double * EdgeBetweenness;
	BetweennessResult();
	BetweennessResult(double * vertexBetweenness, double * edgeBetweenness);
	~BetweennessResult();
};

