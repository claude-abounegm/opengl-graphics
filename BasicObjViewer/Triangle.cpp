#include "Triangle.h"

Triangle::Triangle(uint v1, uint v2, uint v3)
{
	_indexes[0] = v1;
	_indexes[1] = v2;
	_indexes[2] = v3;
}

Triangle::Triangle(const uint v[3])
	: Triangle(v[0], v[1], v[2])
{ }

uint Triangle::v1()
{
	return _indexes[0];
}

uint Triangle::v2()
{
	return _indexes[1];
}

uint Triangle::v3()
{
	return _indexes[2];
}