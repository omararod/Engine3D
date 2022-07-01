#include "Engine3D.h"

int main()
{
	Engine3D engine;
	engine.IntializeFull();
	

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

	Engine3D::Vertex v;
	v.Set(0.2, 0, 0, 1.0);
	Engine3D::Grid grid(1.0f, 10,v);
	engine.AddPredefinedPrimitive(grid);
	
	v.Set(0.2, 0, 0, 0.0,1.0);
	Engine3D::Plane plane(0.5, 0.8,v);
	engine.AddPredefinedPrimitive(plane);

    engine.SetBackgroundColor(0.8, 0.8, 0.8, 0);
	engine.StartWindow();

	
	
	delete[] v1;
	delete[] v2;
	
}