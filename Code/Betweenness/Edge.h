#pragma once
#include "Types.h"
#include <iostream>

using namespace std;

class Edge
{
private:
	int input;
	int output;
	btw_num_t length;
	int id;
public:
	int GetInput();
	int GetOutput();
	void SetInput(int input);
	void SetOutput(int output);
	btw_num_t GetWeight();
	void SetWeight(btw_num_t weight);
	int GetId();

	Edge(int id, int input, int output, btw_num_t length);
	~Edge();
};
