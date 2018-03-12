#pragma once
#include <cmath>
#include <stdlib.h>

class Utils
{
public:
	Utils();
	static bool AreEqual(double a, double b);
	static double RandBetween(double minIncluseive, double maxInclusive);
	~Utils();
};

