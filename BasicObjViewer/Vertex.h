#ifndef VERTEX_H
#define VERTEX_H

struct Vertex
{
private:
	float _coords[3];

public:
	const float* const coords = &_coords[0];

	Vertex(float x, float y, float z);
	Vertex(const float f[3]);

	float x();
	float y();
	float z();
};

#endif