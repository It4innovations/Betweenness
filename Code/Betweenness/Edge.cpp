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

double Edge::GetWeight()
{
	return length;
}

void Edge::SetWeight(double length)
{
	this->length = length;
}

int Edge::GetId()
{
	return id;
}

void Edge::SetId(int id)
{
	this->id = id;
}

Edge::Edge(int input, int output, double length)
{
	this->input = input;
	this->output = output;
	this->length = length;
}



Edge::~Edge()
{
	//cout << "Edge " << input << ", " << output << ", " << weight << " destroyed\n";
}
