#pragma once
#include <iostream>

using namespace std;

class Edge
{
private:
	int input;
	int output;
	double length;
	int id;
public:
	int GetInput();
	int GetOutput();
	void SetInput(int input);
	void SetOutput(int output);
	double GetWeight();
	void SetWeight(double weight);
	int GetId();
	void SetId(int id);

	Edge(int input, int output, double length);
	~Edge();
};
