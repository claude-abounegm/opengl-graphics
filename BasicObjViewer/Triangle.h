#ifndef TRIANGLE_H
#define TRIANGLE_H

typedef unsigned int uint;

typedef struct Triangle
{
private:
	uint _indexes[3];

public:
	const uint* const indexes = &_indexes[0];

	Triangle(uint v1, uint v2, uint v3);
	Triangle(const uint v[3]);

	uint v1();
	uint v2();
	uint v3();
} Triangle;

#endif