#include "Engine3D.h"

Engine3D::CustomPrimitive::CustomPrimitive()
{
	vertices = NULL;
	nVertices = 0;
	topology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}
Engine3D::CustomPrimitive::~CustomPrimitive()
{
	/*if (vertices)
	{
		delete vertices;
		vertices = NULL;
	}*/
}