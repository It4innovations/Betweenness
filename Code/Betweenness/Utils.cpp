#include "Utils.h"

Utils::Utils()
{
}

bool Utils::AreEqual(btw_num_t a, btw_num_t b)
{
	btw_num_t EPSILON = 0.00000001;
	return fabs(a - b) < EPSILON;
}

btw_num_t Utils::RandBetween(btw_num_t minIncluseive, btw_num_t maxInclusive) {
	btw_num_t val = rand() / RAND_MAX;
	btw_num_t range = maxInclusive - minIncluseive;	//with +1 to max => exclusive, but take care of overflow
	return (val * range) + minIncluseive;
}

Utils::~Utils()
{
}
