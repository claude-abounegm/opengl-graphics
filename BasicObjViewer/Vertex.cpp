#include "Vertex.h"

Vertex::Vertex(float x, float y, float z)
{
	_coords[0] = x;
	_coords[1] = y;
	_coords[2] = z;
}

Vertex::Vertex(const float f[3])
	: Vertex(f[0], f[1], f[2])
{ }

float Vertex::x()
{
	return _coords[0];
}

float Vertex::y()
{
	return _coords[1];
}

float Vertex::z()
{
	return _coords[2];
}