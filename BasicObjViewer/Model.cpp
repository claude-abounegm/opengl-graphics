#include "Model.h"

Model::Model() { }

void Model::setProjection()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	if (height == 0)
		height = 1;
	double aspect = width / (double)height;
	if (width >= height)
		glOrtho(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0, -1.0, 1.0);
	else
		glOrtho(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect, -1.0, 1.0);
}

void Model::restoreProjection()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void Model::addVertex(Vertex* v)
{
	minX = min(minX, v->x());
	minY = min(minY, v->y());
	minZ = min(minZ, v->z());

	maxX = max(maxX, v->x());
	maxY = max(maxY, v->y());
	maxZ = max(maxZ, v->z());

	_vertices.push_back(v);
}

void Model::addTriangle(Triangle* t)
{
	_triangles.push_back(t);
}

void Model::drawInternal()
{
	// initialize projection matrix
	setProjection();

	// 
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); {
		for (uint i = 0; i < _triangles.size(); ++i)
		{
			glBegin(GL_TRIANGLES); {
				// a triangle has 3 vertices
				for (int j = 0; j < 3; ++j)
				{
					Vertex& v = *_vertices[_triangles[i]->indexes[j] - 1];
					glVertex3f(v.x(), v.y(), v.z());
				}
			} glEnd();
		}
	} glPopMatrix();

	// restore the projection matrix
	restoreProjection();
}

Model::Model(const std::vector<Triangle*>& triangles, const std::vector<Vertex*>& vertices)
{
	for (uint i = 0; i < triangles.size(); ++i)
		addTriangle(triangles[i]);

	for (uint i = 0; i < vertices.size(); ++i)
		addVertex(vertices[i]);
}

Model::~Model()
{
	if (normalized)
		delete normalized;
	normalized = NULL;
	isNormalized = 0;

	for (uint i = 0; i < _triangles.size(); ++i)
		delete _triangles[i];
	_triangles.clear();

	for (uint i = 0; i < _vertices.size(); ++i)
		delete _vertices[i];
	_vertices.clear();
}

void Model::normalize(float clipBounds)
{
	if (isNormalized)
		return;

	clipBounds = abs(clipBounds);
	if (clipBounds > 1.0)
		clipBounds = 1.0;

	// width
	float width = abs(minX) + abs(maxX);
	float centerX = (minX + maxX) / 2.0;

	// height
	float height = abs(minY) + abs(maxY);
	float centerY = (minY + maxY) / 2.0;

	// depth
	float depth = abs(minZ) + abs(maxZ);
	float centerZ = (minZ + maxZ) / 2.0;

	// scaling
	float x = max(abs(minX - centerX), abs(maxX - centerX));
	float y = max(abs(minY - centerY), abs(maxY - centerY));
	float z = max(abs(minZ - centerZ), abs(maxZ - centerZ));
	float scale = clipBounds / max(max(x, y), z);

	// add the triangles to the 
	std::vector<Triangle*> tri;
	for (uint i = 0; i < _triangles.size(); ++i)
		tri.push_back(new Triangle(_triangles[i]->indexes));

	// add the normalized vertices to the new model
	std::vector<Vertex*> vert;
	for (uint i = 0; i < _vertices.size(); ++i)
	{
		Vertex& v = *_vertices[i];
		vert.push_back(new Vertex(
			scale*(v.x() - centerX),
			scale*(v.y() - centerY),
			scale*(v.z() - centerZ))
			);
	}

	if (normalized)	delete normalized;
	normalized = new Model(tri, vert);
	normalized->isNormalized = 1;
}

void Model::draw()
{
	if (isNormalized)
		this->drawInternal();
	else
	{
		if (!normalized)
			normalize();
		normalized->drawInternal();
	}
}

Model* Model::fromObjFile(const char* path)
{
	// read the file
	FILE* stream = NULL;
	if (fopen_s(&stream, path, "r"))
		return NULL;

	// shortcuts
	char buffer[256];
	Model* m = new Model;

	while (fscanf_s(stream, "%s", buffer, sizeof(buffer)) != EOF)
	{
		if (buffer[0] == 'v' && buffer[1] == '\0')
		{
			float x, y, z;
			if (fscanf_s(stream, "%f %f %f", &x, &y, &z) == 3)
				m->addVertex(new Vertex(x, y, z));
		}
		else if (buffer[0] == 'f')
		{
			uint v1, v2, v3;
			if (fscanf_s(stream, "%d %d %d", &v1, &v2, &v3) == 3)
			{
				m->addTriangle(new Triangle(v1, v2, v3));

				while (fscanf_s(stream, "%d", &v2) == 1)
					m->addTriangle(new Triangle(v1, v3, v2));
			}
		}
		else
			fgets(buffer, sizeof(buffer), stream);
	}

	fclose(stream);
	return m;
}