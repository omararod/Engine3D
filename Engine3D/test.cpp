#include "Engine3D.h"

int main()
{
	Engine3D engine;
	engine.IntializeFull();
	/*Engine3D::Vertex *vertices = new Engine3D::Vertex[3];
	vertices[0] = { 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f };
	vertices[1] = { 0.45f, -0.5f,0.0f,0.0f,1.0f,0.0f,1.0f };
	vertices[2] = { -0.45f, -0.5f,0.0f,0.0f,0.0f,1.0f,1.0f };

	Engine3D::Vertex *vertices2 = new Engine3D::Vertex[3];
	vertices2[0] = { 0.5f, 0.5f,0.0f,1.0f,0.0f,0.0f,1.0f };
	vertices2[1] = { 0.95f, -0.5f,0.0f,0.0f,1.0f,0.0f,1.0f };
	vertices2[3] = { -0.95f, -0.5f,0.0f,0.0f,0.0f,1.0f,1.0f };*/

	Engine3D::Vertex *v1 = new  Engine3D::Vertex[3];
	v1[0].Set(0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v1[1].Set(0.45f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v1[2].Set(-0.45f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	Engine3D::CustomPrimitive p;
	p.nVertices = 3;
	p.vertices = v1;
	engine.AddCustomPrimitive(p);

	Engine3D::Vertex *v2 = new  Engine3D::Vertex[3];
	v2[0].Set(0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v2[1].Set(0.95f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v2[2].Set(-0.45f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	p.vertices = v2;
	p.topology = D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP;
	engine.AddCustomPrimitive(p);

	Engine3D::Grid grid(1.0f, 10);
	engine.AddPredefinedPrimitive(grid);
	engine.StartWindow();

	
	
	delete[] v1;
	delete[] v2;
	
}