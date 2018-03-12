#pragma once
class KeyValuePair
{
public:
	double Key;
	int Value;

	KeyValuePair(double key, int value);
	friend bool operator >(const KeyValuePair& lhs, const KeyValuePair& rhs);
	~KeyValuePair();
};

