#include "Engine3D.h"

Engine3D::Grid::Grid(float size, int subdivisions)
{
	this->size = size;
	this->subdivisions = subdivisions;

	nVertices = 4 * (subdivisions + 1);
	vertices = new Vertex[nVertices];
	topology = D3D10_PRIMITIVE_TOPOLOGY_LINELIST;

	int i = 0;
	float currentX = 0;
	float stepSize = size / subdivisions;
	for (; i < nVertices / 2; i += 2)
	{
		vertices[i].x = currentX;

		vertices[i + 1].x = currentX;
		vertices[i + 1].z = size;

		currentX+= stepSize;
	}
	float currentZ = 0;
	for (; i < nVertices; i += 2)
	{
		vertices[i].z = currentZ;

		vertices[i + 1].z = currentZ;
		vertices[i + 1].x = size;

		currentZ += stepSize;
	}
}

Engine3D::Grid::~Grid()
{
	if (vertices)
		delete[] vertices;
}