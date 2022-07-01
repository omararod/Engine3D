#pragma once
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <vector>

//DirectX includes
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

enum RET_VAL { RET_OK, RET_ERROR };
enum MOVE_DIRECTION {MOVE_FRONT,MOVE_BACK,MOVE_RIGHT, MOVE_LEFT};
enum PrimitiveType {PRIMITIVE_CUSTOM};
#define RELEASE_INTERFACE(X) if(X){X->Release();X=nullptr;}

//TODO This is temporary
typedef D3D10_PRIMITIVE_TOPOLOGY TopologyType;

class RenderingWindow;
class Engine3D;
class CompiledShader;



class RenderingWindow
{
	static LRESULT CALLBACK MsgRouter(HWND hwnd, UINT cmd, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK WndProc(HWND hwnd, UINT cmd, WPARAM wParam, LPARAM lParam);
public:
	RenderingWindow(int width = 640, int height = 480, LPCSTR title = "3D Visualization");
	~RenderingWindow();
	RET_VAL Run();
	HWND GetWindowHandle();
	RET_VAL SetEngine3DPointer(Engine3D* enginePointer);
	std::string windowTitle;
private:
	static bool CtrlIsPressed(short lowWord);
	HWND windowHandle;
	int width;
	int height;
	Engine3D *engine3D;
	
};

class Camera
{
	
public:
	float speed;
	float rotationSpeed;
	float pitch;
	float yaw;
	float fieldOfView;
	float aspectRatio;
	DirectX::XMVECTOR up;
	DirectX::XMVECTOR right;
	DirectX::XMVECTOR forward;
	DirectX::XMVECTOR cameraPosition;
	DirectX::XMVECTOR originalLookAtPoint;
	DirectX::XMVECTOR lookAtPoint;

	DirectX::XMMATRIX transformationsMatrix;
	


	Camera();

	
	void Update();
	void Move(MOVE_DIRECTION direction);
};

class Engine3D
{

public:
	
	class Vertex {
	public:
		float x;
		float y;
		float z;
		float r;
		float g;
		float b;
		float a;
        

		Vertex();
		void Set(float x =0.0f, float y =0.0f, float z = 0.0f, float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 0.0f);
		void SetColor(float r, float g, float b, float a);
		void SetColor(Vertex v);
	};

	class CompiledShader
	{
	public:
		int size;
		void* data;

		CompiledShader();
		~CompiledShader();
	};

	class CPUData
	{
	public:
		DirectX::XMMATRIX transformationsMatrix;
	};
	class GeometricPrimitive
	{
	public:
		PrimitiveType type;
		Vertex* vertices;
		int nVertices;
		TopologyType topology;
	};
	class CustomPrimitive : public GeometricPrimitive
	{
	public:
		CustomPrimitive();
		~CustomPrimitive();
		
		
	};

	class PredefinedPrimitive : public GeometricPrimitive
	{
	public:
		float pitch;
		float yaw;
		float roll;
		float scale;
		Vertex center_color;
	};

	class Grid : public PredefinedPrimitive
	{
	public:
		Grid(float size, int subdivisions, Vertex centerAndColor = Vertex());
		~Grid();
	private:
		float size;
		float subdivisions;


	};

	class Plane : public PredefinedPrimitive
	{
	public:
		Plane(float width, float length,Vertex centerAndColor = Vertex());
		~Plane();
	private:
		float width;
		float length;


	};


	CPUData cpuData;
	Camera camera;

	Engine3D();
	~Engine3D();

	RET_VAL IntializeFull(int width = 640, int height = 480, LPCSTR title = "3D Visualization");
	RET_VAL InitializeGraphics(HWND windowHandle);
	RET_VAL StartWindow();
	RET_VAL Render();
	RET_VAL AddCustomPrimitive(CustomPrimitive primitive);
	RET_VAL AddPredefinedPrimitive(PredefinedPrimitive primitive);
    void    SetBackgroundColor(float r, float g, float b, float a);

private:
    float backgroundColor[4];
	bool windowed;
	RenderingWindow *window;
	IDXGISwapChain *swapchain;
	ID3D11Device *device;
	ID3D11DeviceContext *deviceContext; 
	ID3D11RenderTargetView *backBuffer;
	ID3D11InputLayout *inputLayout;
	ID3D11VertexShader *vertexShader;
	ID3D11PixelShader *pixelShader;
	ID3D11Buffer *vertexBuffer;
	ID3D11Buffer *constantBuffer1;
	ID3D11DepthStencilView *zBuffer;
	ID3D11RasterizerState *rasterizer;
	std::vector<CustomPrimitive> customPrimitives;
	std::vector<PredefinedPrimitive> predefinedPrimitives;
	

	RET_VAL LoadFile(const char *fileName,CompiledShader *compiledShader);
};