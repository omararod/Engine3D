#include "Engine3D.h"

Engine3D::Vertex::Vertex():x(0),y(0),z(0),r(0),g(0),b(0),a(0)
{}
Engine3D::Engine3D() : windowed(false),window(nullptr)
{
	
}

Engine3D::~Engine3D()
{
	if (window)
	{
		delete window;
		window = nullptr;
	}
	RELEASE_INTERFACE(swapchain);
	RELEASE_INTERFACE(device);
	RELEASE_INTERFACE(deviceContext);
	RELEASE_INTERFACE(backBuffer);
	RELEASE_INTERFACE(inputLayout);
	RELEASE_INTERFACE(vertexShader);
	RELEASE_INTERFACE(pixelShader);
	RELEASE_INTERFACE(vertexBuffer);
	RELEASE_INTERFACE(constantBuffer1);
}

RET_VAL Engine3D::IntializeFull(int width, int height, LPCSTR title)
{
	
	window = new RenderingWindow(width, height, title);
	windowed = true;
	InitializeGraphics(window->GetWindowHandle());
	return RET_OK;
}

RET_VAL Engine3D::StartWindow()
{
	if (windowed)
	{
		window->Run();
		return RET_OK;
	}
	return RET_ERROR;
}

RET_VAL Engine3D::InitializeGraphics(HWND windowHandle)
{
	DXGI_SWAP_CHAIN_DESC swapChainDescriptor{ 0 };
	RECT windowRect;

	GetClientRect(windowHandle, &windowRect);
	int windowHeight = windowRect.bottom - windowRect.top;
	int windowWidth = windowRect.right - windowRect.left;

	swapChainDescriptor.BufferCount = 1;                                  
	swapChainDescriptor.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    //rgba color
	
	swapChainDescriptor.BufferDesc.Width = windowWidth;
	swapChainDescriptor.BufferDesc.Height = windowHeight;
	swapChainDescriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     
	swapChainDescriptor.OutputWindow = windowHandle;
	swapChainDescriptor.SampleDesc.Count = 4;
	swapChainDescriptor.Windowed = TRUE;
	swapChainDescriptor.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    //enable full-screen

	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&swapChainDescriptor,
		&swapchain,
		&device,
		NULL,
		&deviceContext);

	//Get a pointer to the back buffer on the device
	ID3D11Texture2D *pointerToBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pointerToBackBuffer);

	
	device->CreateRenderTargetView(pointerToBackBuffer, NULL, &backBuffer);
	pointerToBackBuffer->Release();

	
	deviceContext->OMSetRenderTargets(1, &backBuffer, NULL);


	// Set the viewport
	D3D11_VIEWPORT viewport{ 0 };
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)windowWidth;
	viewport.Height = (float)windowHeight;

	deviceContext->RSSetViewports(1, &viewport);

	
	// create the vertex buffer
	D3D11_BUFFER_DESC bd = { 0 };
	

	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(Vertex) * 100;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	device->CreateBuffer(&bd, NULL, &vertexBuffer);

								
	/*D3D11_MAPPED_SUBRESOURCE mappedSubresource{ 0 };
	deviceContext->Map(vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedSubresource);   
	memcpy(mappedSubresource.pData, vertices, sizeof(vertices));
	deviceContext->Unmap(vertexBuffer, NULL);*/

	//Constant Buffers
	D3D11_BUFFER_DESC bufferDescriptor{ 0 };
	bufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
	//struct size must be a multiple of 16 bytes. We add extra bytes for padding
	bufferDescriptor.ByteWidth = (ceilf((float)sizeof(cpuData) / 16.0)) * 16;
	bufferDescriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	device->CreateBuffer(&bufferDescriptor, NULL, &constantBuffer1);
	deviceContext->VSSetConstantBuffers(0, 1, &constantBuffer1);
	

	CompiledShader vertexShaderBinary;
	CompiledShader pixelShaderBinary;
	LoadFile("Debug\\VertexShader.cso", &vertexShaderBinary);
	LoadFile("Debug\\PixelShader.cso", &pixelShaderBinary);

	device->CreateVertexShader(vertexShaderBinary.data, vertexShaderBinary.size, NULL, &vertexShader);
	device->CreatePixelShader(pixelShaderBinary.data, pixelShaderBinary.size, NULL, &pixelShader);
	deviceContext->VSSetShader(vertexShader, 0, 0);
	deviceContext->PSSetShader(pixelShader, 0, 0);

	//Create and set input layout. This describes our vertex struct to the GPU
	// create the input layout object
	D3D11_INPUT_ELEMENT_DESC inputLayoutDescriptor[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	device->CreateInputLayout(inputLayoutDescriptor, 2, vertexShaderBinary.data, vertexShaderBinary.size, &inputLayout);
	deviceContext->IASetInputLayout(inputLayout);

	if (windowed)
		window->SetEngine3DPointer(this);

	return RET_OK;

}

RET_VAL Engine3D::Render()
{
	//Update camera
	camera.Update();
	cpuData.transformationsMatrix = camera.transformationsMatrix;
	deviceContext->UpdateSubresource(constantBuffer1, 0, 0,&cpuData , 0, 0);
	
	float color[4] = { 1, 1, 1, 0 };
	deviceContext->ClearRenderTargetView(backBuffer,color);

	// select which vertex buffer to display
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	for (CustomPrimitive primitive : customPrimitives)
	{
		//Copy vertices to vertex buffer
		D3D11_MAPPED_SUBRESOURCE mappedSubresource{ 0 };
		deviceContext->Map(vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedSubresource);
		memcpy(mappedSubresource.pData, primitive.vertices, sizeof(Vertex)*primitive.nVertices);
		deviceContext->Unmap(vertexBuffer, NULL);
		// select which primtive type we are using
		//deviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		deviceContext->IASetPrimitiveTopology(primitive.topology);

		// draw the vertex buffer to the back buffer
		deviceContext->Draw(primitive.nVertices, 0);
	}

	for (PredefinedPrimitive primitive : predefinedPrimitives)
	{
		//Copy vertices to vertex buffer
		D3D11_MAPPED_SUBRESOURCE mappedSubresource{ 0 };
		deviceContext->Map(vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedSubresource);
		memcpy(mappedSubresource.pData, primitive.vertices, sizeof(Vertex)*primitive.nVertices);
		deviceContext->Unmap(vertexBuffer, NULL);
		// select which primtive type we are using
		deviceContext->IASetPrimitiveTopology(primitive.topology);

		// draw the vertex buffer to the back buffer
		deviceContext->Draw(primitive.nVertices, 0);
	}

	

	// switch the back buffer and the front buffer
	swapchain->Present(0, 0);

	return RET_OK;
}

RET_VAL Engine3D::LoadFile(const char *fileName,CompiledShader *compiledShader)
{
	//open the file for read in binary mode
	std::ifstream file(fileName,std::ifstream::binary|std::ifstream::in);
	//get the size of the file
	file.seekg(0, std::ios::end);
	compiledShader->size = file.tellg();
	//allocate memory for the file
	compiledShader->data = new BYTE[compiledShader->size];
	//rewind and read the file
	file.seekg(0, std::ios::beg);
	file.read((char *)compiledShader->data, compiledShader->size);
	file.close();
	return RET_OK;
}

Engine3D::CompiledShader::CompiledShader() : data(NULL), size(0)
{
}
Engine3D::CompiledShader::~CompiledShader()
{
	if (data)
	{
		delete[] data;
		data = nullptr;
	}
}
RET_VAL Engine3D::AddCustomPrimitive(CustomPrimitive primitive)
{
	customPrimitives.push_back(primitive);

	return RET_OK;
}

RET_VAL Engine3D::AddPredefinedPrimitive(Engine3D::PredefinedPrimitive primitive)
{
	predefinedPrimitives.push_back(primitive);
	return RET_OK;
}

void Engine3D::Vertex::Set(float x , float y , float z , float r , float g , float b , float a)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}