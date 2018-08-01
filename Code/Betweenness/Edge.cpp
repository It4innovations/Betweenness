#include "Edge.h"

int Edge::GetInput()
{
	return input;
}

int Edge::GetOutput()
{
	return output;
}

void Edge::SetInput(int input)
{
	this->input = input;
}

void Edge::SetOutput(int output)
{
	this->output = output;
}

btw_num_t Edge::GetWeight()
{
	return length;
}

void Edge::SetWeight(btw_num_t length)
{
	this->length = length;
}

int Edge::GetId()
{
	return id;
}

Edge::Edge(int id, int input, int output, btw_num_t length)
{
	this->id = id;
	this->input = input;
	this->output = output;
	this->length = length;
}



Edge::~Edge()
{
	//cout << "Edge " << input << ", " << output << ", " << weight << " destroyed\n";
}
