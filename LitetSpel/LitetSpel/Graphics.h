#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>

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

	HRESULT createDeviceContext(HWND wndHandle, bool windowed);
	HRESULT createResources(HWND wndHandle);
	HRESULT createQuadData();
public:
	HRESULT init(HWND wndHandle, bool windowed);
	HRESULT createShaders();
	void swapBuffer();
	~Graphics();
};