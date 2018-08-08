#include "KeyValuePair.h"

KeyValuePair::KeyValuePair(btw_num_t key, int value)
{
	this->Key = key;
	this->Value = value;
}

bool operator>(const KeyValuePair & lhs, const KeyValuePair & rhs)
{
	return lhs.Key > rhs.Key;
}

KeyValuePair::~KeyValuePair()
{
}