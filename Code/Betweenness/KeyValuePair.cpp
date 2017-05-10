#include "KeyValuePair.h"

KeyValuePair::KeyValuePair(double key, int value)
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