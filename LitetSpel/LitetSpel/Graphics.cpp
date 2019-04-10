#include "Graphics.h"

#include "../../INCLUDE/glm/glm/glm.hpp"
#include "../../INCLUDE/glm/glm/gtc/type_ptr.hpp"
#include "../../INCLUDE/glm/glm/gtc/matrix_transform.hpp"

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"d3d11.lib")
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) \
   if(x != NULL)        \
   {                    \
      x->Release();     \
      x = NULL;         \
   }
#endif

HRESULT Graphics::init(HWND wndHandle, bool windowed)
{
	HRESULT hr = 0;
	hr = createDeviceContext(wndHandle, windowed);
	if (FAILED(hr)) 
		return hr;
	hr = createResources(wndHandle);
	if (FAILED(hr))
		return hr;
	hr = createShaders();
	if (FAILED(hr))
		return hr;
	createQuadData();
	return hr;
}

HRESULT Graphics::createDeviceContext(HWND wndHandle, bool windowed)
{
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = wndHandle;
	scd.SampleDesc.Count = 1;
	scd.Windowed = windowed;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swapChain,
		&device,
		NULL,
		&deviceContext);

	if (SUCCEEDED(hr))
	{
		hr = device->QueryInterface(__uuidof(ID3D11Debug), (void**)&debugger);
		if (FAILED(hr)) return hr;
	}

	if (SUCCEEDED(hr))
	{
		RECT rect;
		GetClientRect(wndHandle, &rect);
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		D3D11_VIEWPORT viewport;
		viewport.Width = (float)width;
		viewport.Height = (float)height;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		deviceContext->RSSetViewports(1, &viewport);
	}

	return hr;
}

HRESULT Graphics::createResources(HWND wndHandle)
{
	RECT rect;
	GetClientRect(wndHandle, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	ID3DBlob* errorBlob = nullptr;
	HRESULT hr;
	/* Depth Buffer */
	{
		ID3D11Texture2D* depthTexture = nullptr;
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.Width = (UINT)width;
		desc.Height = (UINT)height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_D32_FLOAT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		hr = device->CreateTexture2D(&desc, 0, &depthTexture);

		if (FAILED(hr))
		{
			if (errorBlob)
			{
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
				errorBlob->Release();
			}
			return hr;
		}

		D3D11_DEPTH_STENCIL_DESC dsDesc;

		// Depth test parameters
		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

		// Stencil test parameters
		dsDesc.StencilEnable = true;
		dsDesc.StencilReadMask = 0xFF;
		dsDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing
		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing
		dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		ID3D11DepthStencilState * depthStencilState;
		hr = device->CreateDepthStencilState(&dsDesc, &depthStencilState);
		if (FAILED(hr))
		{
			if (errorBlob)
			{
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
				// release "reference" to errorBlob interface object
				errorBlob->Release();
			}
			return hr;
		}
		deviceContext->OMSetDepthStencilState(depthStencilState, 1);
		depthStencilState->Release();

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		ZeroMemory(&dsvDesc, sizeof(dsvDesc));
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;

		hr = device->CreateDepthStencilView(depthTexture, &dsvDesc, &depthStencilView);
		if (FAILED(hr)) {
			if (errorBlob)
			{
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
				errorBlob->Release();
			}
			return hr;
		}
		depthTexture->Release();
	}

	/* Back Buffer */
	{
		ID3D11Texture2D* backBuffer = nullptr;
		hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
		if (FAILED(hr)) {
			if (errorBlob)
			{
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
				errorBlob->Release();
			}
			return hr;
		}
		hr = device->CreateRenderTargetView(backBuffer, nullptr, &backBufferView);
		if (FAILED(hr)) {
			if (errorBlob)
			{
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
				errorBlob->Release();
			}
			return hr;
		}
		backBuffer->Release();
		deviceContext->OMSetRenderTargets(1, &backBufferView, nullptr);
	}

	createGeometryBuffers();

	return hr;
}

void Graphics::createGeometryBuffers() {
	D3D11_BUFFER_DESC constBufferDesc;
	memset(&constBufferDesc, 0, sizeof(constBufferDesc));
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.ByteWidth = sizeof(Box) * 100;
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	device->CreateBuffer(&constBufferDesc, NULL, &boxBuffer);



	memset(&constBufferDesc, 0, sizeof(constBufferDesc));
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.ByteWidth = sizeof(metaballBuffer) * 100;
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	device->CreateBuffer(&constBufferDesc, NULL, &metaballBuffer);
}

HRESULT Graphics::createShaders()
{
	SAFE_RELEASE(vertexShader);
	SAFE_RELEASE(pixelShader);

	ID3DBlob* VS = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT result = D3DCompileFromFile(
		L"vs.hlsl",
		nullptr,
		nullptr,
		"main",
		"vs_5_0",
		D3DCOMPILE_DEBUG,
		0,
		&VS,
		&errorBlob
	);

	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		if (VS)
			VS->Release();
		return result;
	}
	
	device->CreateVertexShader(
		VS->GetBufferPointer(),
		VS->GetBufferSize(),
		nullptr,
		&vertexShader
	);

	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};
	
	device->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), VS->GetBufferPointer(), VS->GetBufferSize(), &vertexLayout);

	VS->Release();

	//create pixel shader
	ID3DBlob* PS = nullptr;
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"ps.hlsl",
		nullptr,
		nullptr,
		"main",
		"ps_5_0",
		D3DCOMPILE_DEBUG,
		0,
		&PS,
		&errorBlob
	);

	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		if (PS)
			PS->Release();
		return result;
	}

	device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), nullptr, &pixelShader);
	PS->Release();

	return S_OK;
}

HRESULT Graphics::createQuadData()
{
	struct TriangleVertex
	{
		float x, y, z;
	};

	TriangleVertex triangleVertices[6] =
	{
		-1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
	};

	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(triangleVertices);

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = triangleVertices;
	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &quadBuffer);
	if (FAILED(hr))
		return hr;

	return hr;
}

void Graphics::queueBoxes(vector<Box> boxes) {
	D3D11_MAPPED_SUBRESOURCE mr;
	ZeroMemory(&mr, sizeof(D3D11_MAPPED_SUBRESOURCE));

	//Box boxArray[100];
	//ZeroMemory(boxArray, sizeof(Box) * 100);
	//memcpy(boxArray, boxes.data(), sizeof(Box) * boxes.size());
	//std::copy(boxes.begin(), boxes.end(), boxArray);
	deviceContext->Map(boxBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr);
	memcpy(mr.pData, boxes.data(), sizeof(Box) * boxes.size());
	deviceContext->Unmap(boxBuffer, 0);
}

void Graphics::queueMetaballs(vector<Sphere> metaballs) {
	D3D11_MAPPED_SUBRESOURCE mr;
	ZeroMemory(&mr, sizeof(D3D11_MAPPED_SUBRESOURCE));
	
	//Sphere metaballArray[100];
	//ZeroMemory(metaballArray, sizeof(Sphere) * 100);
	//memcpy(metaballArray, metaballs.data(), metaballs.size() * sizeof(Sphere));

	deviceContext->Map(metaballBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr);
	memcpy(mr.pData, metaballs.data(), sizeof(Sphere) * metaballs.size());
	deviceContext->Unmap(metaballBuffer, 0);
}

void Graphics::swapBuffer()
{
	float clearColor[] = { 0.0, 0.0, 0.0, 1.0 };
	deviceContext->ClearRenderTargetView(backBufferView, clearColor);
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0, 0);
	
	deviceContext->VSSetShader(vertexShader, nullptr, 0);
	deviceContext->PSSetShader(pixelShader, nullptr, 0);

	deviceContext->PSSetConstantBuffers(0, 1, &boxBuffer);
	deviceContext->PSSetConstantBuffers(1, 1, &metaballBuffer);

	UINT32 vertexSize = sizeof(float) * 3;
	UINT32 offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &quadBuffer, &vertexSize, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->IASetInputLayout(vertexLayout);

	deviceContext->Draw(6, 0);
	swapChain->Present(0, 0);
}

Graphics::~Graphics()
{
	deviceContext->ClearState();

	SAFE_RELEASE(swapChain);
	SAFE_RELEASE(device);
	SAFE_RELEASE(deviceContext);
	SAFE_RELEASE(debugger);
	SAFE_RELEASE(backBufferView);
	SAFE_RELEASE(depthStencilView);
}