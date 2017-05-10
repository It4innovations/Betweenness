#include "Utils.h"

Utils::Utils()
{
}

bool Utils::AreEqual(double a, double b)
{
	double EPSILON = 0.00001f;
	return fabs(a - b) < EPSILON;
}

double Utils::RandBetween(double minIncluseive, double maxInclusive) {
	double val = rand() / RAND_MAX;
	double range = maxInclusive - minIncluseive;	//with +1 to max => exclusive, but take care of overflow
	return (val * range) + minIncluseive;
}

Utils::~Utils()
{
}
