#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <vector>
#include "geometry.h"

using namespace std;

class Graphics {
private:
	IDXGISwapChain* swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	ID3D11Debug* debugger;
	ID3D11RenderTargetView* backBufferView;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	ID3D11InputLayout* vertexLayout;
	ID3D11Buffer* quadBuffer;

	ID3D11Buffer* boxBuffer;
	ID3D11Buffer* metaballBuffer;

	HRESULT createDeviceContext(HWND wndHandle, bool windowed);
	HRESULT createResources(HWND wndHandle);
	HRESULT createQuadData();
	void createGeometryBuffers();
public:
	HRESULT init(HWND wndHandle, bool windowed);
	HRESULT createShaders();
	void queueMetaballs(vector<Sphere> metaballs);
	void queueBoxes(vector<Box> boxes);
	void swapBuffer();
	~Graphics();
};