#include "Globals.h"
#include "Graphics.h"
#include "Collisions.h"

#include "../../INCLUDE/glm/glm/glm.hpp"
#include "../../INCLUDE/glm/glm/gtc/type_ptr.hpp"
#include "../../INCLUDE/glm/glm/gtc/matrix_transform.hpp"
#include "../../INCLUDE/DDSTextureLoader/DDSTextureLoader.h"

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
	createBoxData();
	
	float c = 1.0 / glm::tan(glm::radians(22.5f));
	float a = 16.f / 9.f;
	float f = 300.f;
	float n = 50.f;

	proj = glm::mat4(
		c / a, 0., 0., 0.,
		0.   , c , 0., 0.,
		0.   , 0., f/(f-n), -(f*n)/(f-n),
		0.   , 0., 1, 0.
	);

	camera = glm::vec3(0, 0, -200);
	viewProj = glm::transpose(glm::lookAtLH(camera, camera + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 1.0, 0.0))) * proj;

	invProj = glm::inverse(proj);
	updateFrustumCorners();
	updateCullingBox();

	return hr;
}

HRESULT Graphics::createDeviceContext(HWND wndHandle, bool windowed)
{
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	
	scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	scd.BufferCount = 2;
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
		desc.Format = DXGI_FORMAT_R32_TYPELESS;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
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

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;

		hr = device->CreateShaderResourceView(depthTexture, &srvDesc, &depthResourceView);
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

		ID3D11Texture2D* geometryBuffer = nullptr;
		D3D11_TEXTURE2D_DESC texDesc;
		backBuffer->GetDesc(&texDesc);
		texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

		device->CreateTexture2D(&texDesc, NULL, &geometryBuffer);

		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		ZeroMemory(&rtvDesc, sizeof(rtvDesc));
		rtvDesc.Format = texDesc.Format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;

		hr = device->CreateRenderTargetView(geometryBuffer, &rtvDesc, &geometryBufferView);
		if (FAILED(hr)) {
			if (errorBlob)
			{
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
				errorBlob->Release();
			}
			return hr;
		}

		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(samplerDesc));
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		hr = device->CreateSamplerState(&samplerDesc, &samplerState);
		if (FAILED(hr)) {
			if (errorBlob)
			{
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
				errorBlob->Release();
			}
			return hr;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;

		hr = device->CreateShaderResourceView(geometryBuffer, &srvDesc, &geometryResourceView);
		if (FAILED(hr)) {
			if (errorBlob)
			{
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
				errorBlob->Release();
			}
			return hr;
		}

		hr = DirectX::CreateDDSTextureFromFile(device, L"radiance.dds", nullptr, &radianceResourceView);
		if (FAILED(hr)) {
			if (errorBlob)
			{
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
				errorBlob->Release();
			}
			return hr;
		}

		hr = DirectX::CreateDDSTextureFromFile(device, L"irradiance.dds", nullptr, &irradianceResourceView);
		if (FAILED(hr)) {
			if (errorBlob)
			{
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
				errorBlob->Release();
			}
			return hr;
		}
		
		backBuffer->Release();
		geometryBuffer->Release();
	}

	createCBuffers();

	D3D11_BLEND_DESC blendDesc;
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = device->CreateBlendState(&blendDesc, &blendState);
	if (FAILED(hr)) {
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
	}

	return hr;
}

void Graphics::createCBuffers() {
	D3D11_BUFFER_DESC constBufferDesc;
	memset(&constBufferDesc, 0, sizeof(constBufferDesc));
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.ByteWidth = sizeof(glm::mat4) * maxMovingBoxes + sizeof(glm::vec4) * maxMovingBoxes;
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	device->CreateBuffer(&constBufferDesc, NULL, &movingBoxTransformBuffer);


	memset(&constBufferDesc, 0, sizeof(constBufferDesc));
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.ByteWidth = sizeof(glm::vec4) * maxStaticBoxes;
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	device->CreateBuffer(&constBufferDesc, NULL, &staticBoxColorBuffer);


	memset(&constBufferDesc, 0, sizeof(constBufferDesc));
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.ByteWidth = sizeof(Sphere) * maxMetaballs + sizeof(glm::vec4);
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	device->CreateBuffer(&constBufferDesc, NULL, &metaballBuffer);



	memset(&constBufferDesc, 0, sizeof(constBufferDesc));
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.ByteWidth = sizeof(glm::mat4) * maxLasers + sizeof(glm::vec4) * maxLasers;
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	device->CreateBuffer(&constBufferDesc, NULL, &laserTransformBuffer);



	memset(&constBufferDesc, 0, sizeof(constBufferDesc));
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.ByteWidth = sizeof(glm::mat4);
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	device->CreateBuffer(&constBufferDesc, NULL, &viewProjBuffer);



	memset(&constBufferDesc, 0, sizeof(constBufferDesc));
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.ByteWidth = sizeof(glm::vec4);
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	device->CreateBuffer(&constBufferDesc, NULL, &cameraBuffer);



	memset(&constBufferDesc, 0, sizeof(constBufferDesc));
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.ByteWidth = sizeof(glm::vec4) * 4;
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	device->CreateBuffer(&constBufferDesc, NULL, &cornerBuffer);



	memset(&constBufferDesc, 0, sizeof(constBufferDesc));
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.ByteWidth = sizeof(glm::vec4);
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	device->CreateBuffer(&constBufferDesc, NULL, &shadowBuffer);
}

void Graphics::updateFrustumCorners() {
	frustumCorners[0] = glm::vec4(-1.0, 1.0, 1.0, 1.0)  * invProj;
	frustumCorners[1] = glm::vec4(1.0, 1.0, 1.0, 1.0) * invProj;
	frustumCorners[2] = glm::vec4(-1.0, -1.0, 1.0, 1.0) * invProj;
	frustumCorners[3] = glm::vec4(1.0, -1.0, 1.0, 1.0) * invProj;
	frustumCorners[0] /= frustumCorners[0].w;
	frustumCorners[1] /= frustumCorners[1].w;
	frustumCorners[2] /= frustumCorners[2].w;
	frustumCorners[3] /= frustumCorners[3].w;

	D3D11_MAPPED_SUBRESOURCE mr;
	ZeroMemory(&mr, sizeof(D3D11_MAPPED_SUBRESOURCE));
	deviceContext->Map(cornerBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr);
	memcpy(mr.pData, frustumCorners, sizeof(glm::vec4) * 4);
	deviceContext->Unmap(cornerBuffer, 0);
}

void Graphics::updateCullingBox() {
	cullingBox.center = glm::vec4(camera.x, camera.y, 0.0, 1.0);
	cullingBox.halfLengths = frustumCorners[1] + glm::vec4(0.0, 0.0, camera.z, 0.0);
}

HRESULT Graphics::createShaders()
{
	HRESULT result;
	ID3DBlob* errorBlob = nullptr;

	/*** RAY SHADER ***/
	{
		SAFE_RELEASE(vertexShader);
		SAFE_RELEASE(pixelShader);

		ID3DBlob* VS = nullptr;

		result = D3DCompileFromFile(
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

		device->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), VS->GetBufferPointer(), VS->GetBufferSize(), &quadVertexLayout);

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
	}



	/*** BOX RASTERIZATION SHADERS ***/
	{
		SAFE_RELEASE(movingBoxVertexShader);
		SAFE_RELEASE(movingBoxPixelShader);

		ID3DBlob* rVS = nullptr;

		result = D3DCompileFromFile(
			L"box_raster_vs.hlsl",
			nullptr,
			nullptr,
			"main",
			"vs_5_0",
			D3DCOMPILE_DEBUG,
			0,
			&rVS,
			&errorBlob
		);

		if (FAILED(result))
		{
			if (errorBlob)
			{
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
				errorBlob->Release();
			}
			if (rVS)
				rVS->Release();
			return result;
		}

		device->CreateVertexShader(
			rVS->GetBufferPointer(),
			rVS->GetBufferSize(),
			nullptr,
			&movingBoxVertexShader
		);

		D3D11_INPUT_ELEMENT_DESC rasterInputDesc[] = {
			{
				"POSITION",
				0,
				DXGI_FORMAT_R32G32B32_FLOAT,
				0,
				0,
				D3D11_INPUT_PER_VERTEX_DATA,
				0
			},
			{
				"NORMAL",
				0,
				DXGI_FORMAT_R32G32B32_FLOAT,
				0,
				12,
				D3D11_INPUT_PER_VERTEX_DATA,
				0
			}
		};

		device->CreateInputLayout(rasterInputDesc, ARRAYSIZE(rasterInputDesc), rVS->GetBufferPointer(), rVS->GetBufferSize(), &boxVertexLayout);

		rVS->Release();

		//create pixel shader
		ID3DBlob* rPS = nullptr;
		if (errorBlob) errorBlob->Release();
		errorBlob = nullptr;

		result = D3DCompileFromFile(
			L"box_raster_ps.hlsl",
			nullptr,
			nullptr,
			"main",
			"ps_5_0",
			D3DCOMPILE_DEBUG,
			0,
			&rPS,
			&errorBlob
		);

		if (FAILED(result))
		{
			if (errorBlob)
			{
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
				errorBlob->Release();
			}
			if (rPS)
				rPS->Release();
			return result;
		}

		device->CreatePixelShader(rPS->GetBufferPointer(), rPS->GetBufferSize(), nullptr, &movingBoxPixelShader);
		rPS->Release();
	}

	{
		SAFE_RELEASE(staticBoxVertexShader);
		SAFE_RELEASE(staticBoxPixelShader);

		ID3DBlob* VS = nullptr;

		result = D3DCompileFromFile(
			L"static_box_raster_vs.hlsl",
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
			&staticBoxVertexShader
		);

		D3D11_INPUT_ELEMENT_DESC rasterInputDesc[] = {
			{
				"POSITION",
				0,
				DXGI_FORMAT_R32G32B32_FLOAT,
				0,
				0,
				D3D11_INPUT_PER_VERTEX_DATA,
				0
			},
			{
				"NORMAL",
				0,
				DXGI_FORMAT_R32G32B32_FLOAT,
				0,
				12,
				D3D11_INPUT_PER_VERTEX_DATA,
				0
			}
		};

		device->CreateInputLayout(rasterInputDesc, ARRAYSIZE(rasterInputDesc), VS->GetBufferPointer(), VS->GetBufferSize(), &boxVertexLayout);

		VS->Release();

		//create pixel shader
		ID3DBlob* PS = nullptr;
		if (errorBlob) errorBlob->Release();
		errorBlob = nullptr;

		result = D3DCompileFromFile(
			L"static_box_raster_ps.hlsl",
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

		device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), nullptr, &staticBoxPixelShader);
		PS->Release();
	}


	/*** LASERS ***/
	{
		SAFE_RELEASE(laserVertexShader);
		SAFE_RELEASE(laserPixelShader);

		ID3DBlob* VS = nullptr;

		result = D3DCompileFromFile(
			L"laser_raster_vs.hlsl",
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
			&laserVertexShader
		);

		D3D11_INPUT_ELEMENT_DESC rasterInputDesc[] = {
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

		device->CreateInputLayout(rasterInputDesc, ARRAYSIZE(rasterInputDesc), VS->GetBufferPointer(), VS->GetBufferSize(), &laserVertexLayout);

		VS->Release();

		//create pixel shader
		ID3DBlob* PS = nullptr;
		if (errorBlob) errorBlob->Release();
		errorBlob = nullptr;

		result = D3DCompileFromFile(
			L"laser_raster_ps.hlsl",
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

		device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), nullptr, &laserPixelShader);
		PS->Release();
	}

	return S_OK;
}

HRESULT Graphics::createQuadData()
{
	glm::vec3 triangleVertices[6] =
	{
		glm::vec3(-1.0f, -1.0f, 0.0f),
		glm::vec3(-1.0f,  1.0f, 0.0f),
		glm::vec3( 1.0f, -1.0f, 0.0f),
		glm::vec3(-1.0f,  1.0f, 0.0f),
		glm::vec3( 1.0f,  1.0f, 0.0f),
		glm::vec3( 1.0f, -1.0f, 0.0f)
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

HRESULT Graphics::createBoxData()
{
	glm::vec3 quadVertices[6 * 2] =
	{
		/* front face */					/* normals */
		glm::vec3(-1.0f, -1.0f, -1.0f),		glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)),
		glm::vec3(-1.0f,  1.0f, -1.0f),     glm::normalize(glm::vec3(-1.0f,  1.0f, -1.0f)),
		glm::vec3( 1.0f, -1.0f, -1.0f),     glm::normalize(glm::vec3( 1.0f, -1.0f, -1.0f)),
		glm::vec3(-1.0f,  1.0f, -1.0f),     glm::normalize(glm::vec3(-1.0f,  1.0f, -1.0f)),
		glm::vec3( 1.0f,  1.0f, -1.0f),     glm::normalize(glm::vec3( 1.0f,  1.0f, -1.0f)),
		glm::vec3( 1.0f, -1.0f, -1.0f),     glm::normalize(glm::vec3( 1.0f, -1.0f, -1.0f))
	};

	for (int i = 0; i < 4; ++i)
	{
		glm::mat4 rotation = glm::rotate(glm::mat4(1.f), glm::radians(90.f * i), glm::vec3(0.f, 1.f, 0.f));
		boxVertices[i * 12 + 0] = (glm::vec4(quadVertices[0], 1.0) * rotation);
		boxVertices[i * 12 + 1] = (glm::vec4(quadVertices[1], 1.0) * rotation);
		boxVertices[i * 12 + 2] = (glm::vec4(quadVertices[2], 1.0) * rotation);
		boxVertices[i * 12 + 3] = (glm::vec4(quadVertices[3], 1.0) * rotation);
		boxVertices[i * 12 + 4] = (glm::vec4(quadVertices[4], 1.0) * rotation);
		boxVertices[i * 12 + 5] = (glm::vec4(quadVertices[5], 1.0) * rotation);
		boxVertices[i * 12 + 6] = (glm::vec4(quadVertices[6], 0.0) * rotation);
		boxVertices[i * 12 + 7] = (glm::vec4(quadVertices[7], 0.0) * rotation);
		boxVertices[i * 12 + 8] = (glm::vec4(quadVertices[8], 0.0) * rotation);
		boxVertices[i * 12 + 9] = (glm::vec4(quadVertices[9], 0.0) * rotation);
		boxVertices[i * 12 + 10] = (glm::vec4(quadVertices[10], 0.0) * rotation);
		boxVertices[i * 12 + 11] = (glm::vec4(quadVertices[11], 0.0) * rotation);
	}
	
	glm::mat4 rotation = glm::rotate(glm::mat4(1.f), glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
	boxVertices[4 * 12 + 0] = (glm::vec4(quadVertices[0], 1.0) * rotation);
	boxVertices[4 * 12 + 1] = (glm::vec4(quadVertices[1], 1.0) * rotation);
	boxVertices[4 * 12 + 2] = (glm::vec4(quadVertices[2], 1.0) * rotation);
	boxVertices[4 * 12 + 3] = (glm::vec4(quadVertices[3], 1.0) * rotation);
	boxVertices[4 * 12 + 4] = (glm::vec4(quadVertices[4], 1.0) * rotation);
	boxVertices[4 * 12 + 5] = (glm::vec4(quadVertices[5], 1.0) * rotation);
	boxVertices[4 * 12 + 6] = (glm::vec4(quadVertices[6], 0.0) * rotation);
	boxVertices[4 * 12 + 7] = (glm::vec4(quadVertices[7], 0.0) * rotation);
	boxVertices[4 * 12 + 8] = (glm::vec4(quadVertices[8], 0.0) * rotation);
	boxVertices[4 * 12 + 9] = (glm::vec4(quadVertices[9], 0.0) * rotation);
	boxVertices[4 * 12 + 10] = (glm::vec4(quadVertices[10], 0.0) * rotation);
	boxVertices[4 * 12 + 11] = (glm::vec4(quadVertices[11], 0.0) * rotation);

	rotation = glm::rotate(glm::mat4(1.f), glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));
	boxVertices[5 * 12 + 0] = (glm::vec4(quadVertices[0], 1.0) * rotation);
	boxVertices[5 * 12 + 1] = (glm::vec4(quadVertices[1], 1.0) * rotation);
	boxVertices[5 * 12 + 2] = (glm::vec4(quadVertices[2], 1.0) * rotation);
	boxVertices[5 * 12 + 3] = (glm::vec4(quadVertices[3], 1.0) * rotation);
	boxVertices[5 * 12 + 4] = (glm::vec4(quadVertices[4], 1.0) * rotation);
	boxVertices[5 * 12 + 5] = (glm::vec4(quadVertices[5], 1.0) * rotation);
	boxVertices[5 * 12 + 6] = (glm::vec4(quadVertices[6], 0.0) * rotation);
	boxVertices[5 * 12 + 7] = (glm::vec4(quadVertices[7], 0.0) * rotation);
	boxVertices[5 * 12 + 8] = (glm::vec4(quadVertices[8], 0.0) * rotation);
	boxVertices[5 * 12 + 9] = (glm::vec4(quadVertices[9], 0.0) * rotation);
	boxVertices[5 * 12 + 10] = (glm::vec4(quadVertices[10], 0.0) * rotation);
	boxVertices[5 * 12 + 11] = (glm::vec4(quadVertices[11], 0.0) * rotation);

	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(boxVertices);

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = boxVertices;
	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &boxVertexBuffer);
	return hr;
}

void Graphics::setMovingBoxes(const vector<Box>& boxes)
{
	D3D11_MAPPED_SUBRESOURCE mr;
	ZeroMemory(&mr, sizeof(D3D11_MAPPED_SUBRESOURCE));

	struct Transforms {
		glm::mat4 WVP[maxMovingBoxes];
		glm::vec4 color[maxMovingBoxes];
	} transforms;
	memset(&transforms, 0, sizeof(Transforms));
	int transformIndex = 0;
	for (int i = 0; i < boxes.size(); ++i)
	{
		if (CollisionManager::intersect(boxes[i], cullingBox)) {
			glm::mat4 t = glm::translate(glm::mat4(1.0), glm::vec3(boxes[i].center));
			t = glm::scale(t, glm::vec3(boxes[i].halfLengths));
			transforms.WVP[transformIndex] = transpose(t);
			transforms.color[transformIndex] = boxes[i].color;
			++transformIndex;
		}
	}

	deviceContext->Map(movingBoxTransformBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr);
	memcpy(mr.pData, &transforms, sizeof(Transforms));
	deviceContext->Unmap(movingBoxTransformBuffer, 0);

	movingBoxInstances = transformIndex;
}

void Graphics::setStaticBoxes(const vector<Box>& boxes)
{
	D3D11_MAPPED_SUBRESOURCE mr;
	ZeroMemory(&mr, sizeof(D3D11_MAPPED_SUBRESOURCE));
	vector<glm::vec3> worldVertices;
	worldVertices.reserve(boxes.size() * 36 * 2);

	struct Colors {
		glm::vec4 color[maxStaticBoxes];
	} colors;
	memset(&colors, 0, sizeof(glm::vec4) * maxStaticBoxes);

	for (int i = 0; i < boxes.size(); ++i)
	{
		glm::mat4 t = glm::translate(glm::mat4(1.0), glm::vec3(boxes[i].center));
		t = glm::scale(t, glm::vec3(boxes[i].halfLengths));
		
		for (int j = 0; j < 36 * 2; j += 2) {
			worldVertices.push_back(glm::vec3(t * glm::vec4(boxVertices[j], 1.0)));
			worldVertices.push_back(boxVertices[j + 1]);
		}

		colors.color[i] = boxes[i].color;
	}

	deviceContext->Map(staticBoxColorBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr);
	memcpy(mr.pData, &colors, sizeof(Colors));
	deviceContext->Unmap(staticBoxColorBuffer, 0);

	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = worldVertices.size() * sizeof(glm::vec3);

	SAFE_RELEASE(staticBoxVertexBuffer);
	
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = worldVertices.data();
	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &staticBoxVertexBuffer);

	staticBoxInstances = boxes.size();
}

void Graphics::setLasers(const vector<Line>& lines)
{
	D3D11_MAPPED_SUBRESOURCE mr;
	ZeroMemory(&mr, sizeof(D3D11_MAPPED_SUBRESOURCE));

	struct Transforms {
		glm::mat4 WVP[maxLasers];
		glm::vec4 color[maxLasers];
	} transforms;
	memset(&transforms, 0, sizeof(glm::mat4) * maxLasers + sizeof(glm::vec4) * maxLasers);

	for (int i = 0; i < lines.size(); ++i)
	{
		glm::vec3 lengths = lines[i].start - lines[i].end;
		glm::mat4 t = glm::mat4(1.0);
		t = glm::translate(t, (lines[i].start - lengths * 0.5f));
		t = glm::rotate(t, atan2f(lengths.y, lengths.x), glm::vec3(0.0, 0.0, 1.0));
		t = glm::scale(t, glm::vec3(length(lengths) * 0.5, 1.5, 0.01));

		transforms.WVP[i] = transpose(t) * viewProj;
		transforms.color[i] = glm::vec4(lines[i].color, 0.0);
	}

	deviceContext->Map(laserTransformBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr);
	memcpy(mr.pData, &transforms, sizeof(Transforms));
	deviceContext->Unmap(laserTransformBuffer, 0);

	laserInstances = lines.size();
}

void Graphics::setMetaballs(const vector<Sphere>& metaballs)
{
	D3D11_MAPPED_SUBRESOURCE mr;
	ZeroMemory(&mr, sizeof(D3D11_MAPPED_SUBRESOURCE));
	struct MetaballStruct {
		Sphere spheres[maxMetaballs];
		int nSpheres;
	} metaballStruct;
	ZeroMemory(&metaballStruct, sizeof(MetaballStruct));

	int structIndex = 0;
	for (int i = 0; i < metaballs.size(); ++i) {
		Box boundingBox;
		boundingBox.center = metaballs[i].centerRadius;
		boundingBox.halfLengths = glm::vec4(metaballs[i].centerRadius.w);

		if (CollisionManager::intersect(boundingBox, cullingBox)) {
			metaballStruct.spheres[structIndex++] = metaballs[i];
			metaballStruct.nSpheres = min(maxMetaballs, structIndex);
		}
	}

	deviceContext->Map(metaballBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr);
	memcpy(mr.pData, &metaballStruct, sizeof(MetaballStruct));
	deviceContext->Unmap(metaballBuffer, 0);
}


void Graphics::setCameraPos(glm::vec3 pos, bool panCamera)
{
	if (panCamera && pos != camera) {
		glm::vec3 cameraDir = pos - camera;
		glm::vec3 delta = glm::normalize(cameraDir) * glm::smoothstep(-10.f, 75.f, glm::length(cameraDir)) * cameraSpeed;
		if (glm::length(delta) > glm::length(cameraDir)) {
			camera = pos;
		}
		else {
			camera += delta;
		}
	}
	else {
		camera = pos;
	}
	viewProj = glm::transpose(glm::lookAtLH(camera, camera + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 1.0, 0.0))) * proj;

	updateFrustumCorners();
	updateCullingBox();

	D3D11_MAPPED_SUBRESOURCE mr;
	ZeroMemory(&mr, sizeof(D3D11_MAPPED_SUBRESOURCE));
	deviceContext->Map(cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr);
	memcpy(mr.pData, glm::value_ptr(camera), sizeof(glm::vec3));
	deviceContext->Unmap(cameraBuffer, 0);

}

void Graphics::castPlayerShadow(glm::vec3 playerPos)
{
	glm::vec4 shadowPoint = glm::vec4(playerPos, 100.0);

	D3D11_MAPPED_SUBRESOURCE mr;
	ZeroMemory(&mr, sizeof(D3D11_MAPPED_SUBRESOURCE));
	deviceContext->Map(shadowBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr);
	memcpy(mr.pData, glm::value_ptr(shadowPoint), sizeof(glm::vec4));
	deviceContext->Unmap(shadowBuffer, 0);
}

void Graphics::swapBuffer()
{
	// clear buffers
	float clearColor[] = { 0.0, 0.0, 0.0, 0.0 };
	deviceContext->ClearRenderTargetView(backBufferView, clearColor);
	deviceContext->ClearRenderTargetView(geometryBufferView, clearColor);
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0, 0);

	// update view projection matrix buffer
	D3D11_MAPPED_SUBRESOURCE mr;
	ZeroMemory(&mr, sizeof(D3D11_MAPPED_SUBRESOURCE));
	deviceContext->Map(viewProjBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr);
	memcpy(mr.pData, glm::value_ptr(viewProj), sizeof(glm::mat4));
	deviceContext->Unmap(viewProjBuffer, 0);

	// draw moving boxes
	deviceContext->VSSetShader(movingBoxVertexShader, nullptr, 0);
	deviceContext->PSSetShader(movingBoxPixelShader, nullptr, 0);

	deviceContext->VSSetConstantBuffers(0, 1, &movingBoxTransformBuffer);
	deviceContext->VSSetConstantBuffers(1, 1, &viewProjBuffer);
	deviceContext->PSSetConstantBuffers(0, 1, &shadowBuffer);
	UINT32 vertexSize = sizeof(glm::vec3) * 2;
	UINT32 offset = 0;
	
	deviceContext->OMSetRenderTargets(1, &geometryBufferView, depthStencilView);
	deviceContext->IASetVertexBuffers(0, 1, &boxVertexBuffer, &vertexSize, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->IASetInputLayout(boxVertexLayout);
	deviceContext->DrawInstanced(36, movingBoxInstances, 0, 0);

	// draw static boxes
	deviceContext->VSSetShader(staticBoxVertexShader, nullptr, 0);
	deviceContext->PSSetShader(staticBoxPixelShader, nullptr, 0);

	deviceContext->VSSetConstantBuffers(0, 1, &staticBoxColorBuffer);
	deviceContext->VSSetConstantBuffers(1, 1, &viewProjBuffer);
	deviceContext->PSSetConstantBuffers(0, 1, &shadowBuffer);

	deviceContext->IASetVertexBuffers(0, 1, &staticBoxVertexBuffer, &vertexSize, &offset);
	deviceContext->Draw(36 * staticBoxInstances, 0);

	// draw lasers
	deviceContext->VSSetShader(laserVertexShader, nullptr, 0);
	deviceContext->PSSetShader(laserPixelShader, nullptr, 0);

	deviceContext->OMSetBlendState(blendState, 0, UINT_MAX);
	deviceContext->IASetVertexBuffers(0, 1, &boxVertexBuffer, &vertexSize, &offset);
	deviceContext->IASetInputLayout(laserVertexLayout);
	deviceContext->VSSetConstantBuffers(0, 1, &laserTransformBuffer);
	deviceContext->DrawInstanced(36, laserInstances, 0, 0);

	// lighting / metaball pass
	deviceContext->VSSetShader(vertexShader, nullptr, 0);
	deviceContext->PSSetShader(pixelShader, nullptr, 0);

	deviceContext->PSSetConstantBuffers(0, 1, &laserTransformBuffer);
	deviceContext->PSSetConstantBuffers(1, 1, &metaballBuffer);
	deviceContext->PSSetConstantBuffers(2, 1, &cameraBuffer);
	deviceContext->PSSetConstantBuffers(3, 1, &cornerBuffer);
	deviceContext->PSSetConstantBuffers(4, 1, &viewProjBuffer);

	vertexSize = sizeof(float) * 3;
	offset = 0;

	deviceContext->OMSetRenderTargets(1, &backBufferView, nullptr);
	deviceContext->IASetVertexBuffers(0, 1, &quadBuffer, &vertexSize, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->IASetInputLayout(quadVertexLayout);
	deviceContext->PSSetShaderResources(0, 1, &geometryResourceView);
	deviceContext->PSSetShaderResources(1, 1, &depthResourceView);
	deviceContext->PSSetShaderResources(2, 1, &radianceResourceView);
	deviceContext->PSSetShaderResources(3, 1, &irradianceResourceView);
	deviceContext->PSSetSamplers(0, 1, &samplerState);

	deviceContext->Draw(6, 0);
	ID3D11ShaderResourceView* nullview = nullptr;
	deviceContext->PSSetShaderResources(0, 1, &nullview);
	deviceContext->PSSetShaderResources(1, 1, &nullview);
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
	SAFE_RELEASE(depthResourceView);
	SAFE_RELEASE(geometryResourceView);
	SAFE_RELEASE(samplerState);

	SAFE_RELEASE(vertexShader);
	SAFE_RELEASE(pixelShader);
	SAFE_RELEASE(movingBoxVertexShader);
	SAFE_RELEASE(movingBoxPixelShader);
	
	SAFE_RELEASE(quadVertexLayout);
	SAFE_RELEASE(quadBuffer);
	SAFE_RELEASE(boxVertexLayout);
	SAFE_RELEASE(boxVertexBuffer);
	SAFE_RELEASE(movingBoxTransformBuffer);
	SAFE_RELEASE(metaballBuffer);
	SAFE_RELEASE(viewProjBuffer);
	SAFE_RELEASE(cameraBuffer);
	SAFE_RELEASE(cornerBuffer);
}

glm::vec3 Graphics::windowToWorldCoord(glm::vec2 windowCoord) {
	windowCoord.y = WINDOW_HEIGHT - windowCoord.y;
	glm::vec4 projected = glm::transpose(viewProj) * glm::vec4(0.0, 0.0, 0.0, 1.0);
	projected.x = ((windowCoord.x / WINDOW_WIDTH) * 2.0 - 1.0) * projected.w;
	projected.y = ((windowCoord.y / WINDOW_HEIGHT) * 2.0 - 1.0) * projected.w;

	glm::vec3 worldPos = glm::inverse(glm::transpose(viewProj)) * projected;
	return worldPos;
}
