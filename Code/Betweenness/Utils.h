#pragma once
#include "Types.h"
#include <cmath>
#include <stdlib.h>

class Utils
{
public:
	Utils();
	static bool AreEqual(btw_num_t a, btw_num_t b);
	static btw_num_t RandBetween(btw_num_t minIncluseive, btw_num_t maxInclusive);
	~Utils();
};

