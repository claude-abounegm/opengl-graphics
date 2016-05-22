#ifndef MODEL_H
#define MODEL_H

#include "Triangle.h"
#include "Vertex.h"

#include <GL\glut.h>
#include <algorithm>
#include <vector>

using namespace std;

typedef struct Model
{
private:
	// whether this is a normalized model (lies between -1.0 and 1.0 on all axis)
	Model* normalized = NULL;
	char isNormalized = 0;

	// containers of triangles and vertices
	std::vector<Triangle*> _triangles;
	std::vector<Vertex*> _vertices;

	// boundaries of the model
	float minX = FLT_MAX;
	float maxX = FLT_MIN;
	float minY = FLT_MAX;
	float maxY = FLT_MIN;
	float minZ = FLT_MAX;
	float maxZ = FLT_MIN;

	// 
	Model();
	void setProjection();
	void restoreProjection();
	void addVertex(Vertex* v);
	void addTriangle(Triangle* t);
	void drawInternal();

public:
	Model(const std::vector<Triangle*>& triangles, const std::vector<Vertex*>& vertices);
	~Model();
	void normalize(float clipBounds = 0.6);
	void draw();

	static Model* fromObjFile(const char* path);
} Model;

#endif