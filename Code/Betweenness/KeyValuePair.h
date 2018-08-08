#pragma once
#include "Types.h"

class KeyValuePair
{
public:
	btw_num_t Key;
	int Value;

	KeyValuePair(btw_num_t key, int value);
	friend bool operator >(const KeyValuePair& lhs, const KeyValuePair& rhs);
	~KeyValuePair();
};

