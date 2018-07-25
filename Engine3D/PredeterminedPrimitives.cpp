#include "Engine3D.h"

Engine3D::Grid::Grid(float size, int subdivisions, Vertex centerAndColor )
{
	this->size = size;
	this->subdivisions = subdivisions;

	center_color = centerAndColor;
	
	nVertices = 4 * (subdivisions + 1);
	vertices = new Vertex[nVertices];
	topology = D3D10_PRIMITIVE_TOPOLOGY_LINELIST;

	int i = 0;
	float currentX = center_color.x - size/2;
	float stepSize = size / subdivisions;
	for (; i < nVertices / 2; i += 2)
	{
		vertices[i].x = currentX;
		vertices[i].z = center_color.z - size/2;

		vertices[i + 1].x = currentX;
		vertices[i + 1].z = center_color.z + size/2 ;

		vertices[i].SetColor(center_color);
		vertices[i + 1].SetColor(center_color);
			
		currentX+= stepSize;
	}
	float currentZ = center_color.z - size/2;
	for (; i < nVertices; i += 2)
	{
		vertices[i].z = currentZ;
		vertices[i].x = center_color.x - size/2;

		vertices[i + 1].z = currentZ;
		vertices[i + 1].x = center_color.x + size/2;

		vertices[i].SetColor(center_color);
		vertices[i + 1].SetColor(center_color);
		

		currentZ += stepSize;
	}
}

Engine3D::Grid::~Grid()
{
	if (vertices)
		delete[] vertices;
}

Engine3D::Plane::Plane(float width, float length, Vertex centerAndColor)
{
	this->width = width;
	this->length = length;

	center_color = centerAndColor;
	
	nVertices = 4 ;
	vertices = new Vertex[nVertices];
	topology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	vertices[0].x = center_color.x - this->length / 2;
	vertices[0].y = center_color.y;
	vertices[0].z = center_color.z - this->width / 2;
	vertices[0].SetColor(center_color);

	vertices[1].x = center_color.x - this->length / 2;
	vertices[1].y = center_color.y;
	vertices[1].z = center_color.z + this->width / 2;
	vertices[1].SetColor(center_color);

	vertices[2].x = center_color.x + this->length / 2;
	vertices[2].y = center_color.y;
	vertices[2].z = center_color.z - this->width / 2;
	vertices[2].SetColor(center_color);

	vertices[3].x = center_color.x + this->length / 2;
	vertices[3].y = center_color.y;
	vertices[3].z = center_color.z + this->width / 2;
	vertices[3].SetColor(center_color);

}

Engine3D::Plane::~Plane()
{
	if (vertices)
		delete[] vertices;
}