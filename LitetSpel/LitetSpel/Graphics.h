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
	ID3D11RenderTargetView* geometryBufferView;
	ID3D11RenderTargetView* backBufferView;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11ShaderResourceView* depthResourceView;
	ID3D11ShaderResourceView* geometryResourceView;
	ID3D11ShaderResourceView* skyboxResourceView;
	ID3D11ShaderResourceView* radianceResourceView;
	ID3D11ShaderResourceView* irradianceResourceView;
	ID3D11SamplerState* samplerState;
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	ID3D11VertexShader* boxRasterVertexShader = nullptr;
	ID3D11PixelShader* boxRasterPixelShader = nullptr;

	ID3D11InputLayout* quadVertexLayout;
	ID3D11Buffer* quadBuffer;

	static const int maxMetaballs = 15;
	static const int maxLines = 10;

	ID3D11InputLayout* vertexLayout;
	ID3D11Buffer* boxTransformBuffer;
	ID3D11Buffer* boxVertexBuffer;
	int boxInstances = 0;
	ID3D11Buffer* metaballBuffer;
	ID3D11Buffer* lineBuffer;
	ID3D11Buffer* viewProjBuffer;
	ID3D11Buffer* cameraBuffer;
	ID3D11Buffer* cornerBuffer;

	glm::vec3 camera;
	glm::mat4 proj;
	glm::mat4 viewProj;
	glm::mat4 invTransposedWVP;

	HRESULT createDeviceContext(HWND wndHandle, bool windowed);
	HRESULT createResources(HWND wndHandle);
	HRESULT createQuadData();
	HRESULT createBoxData();
	void createCBuffers();
public:
	HRESULT init(HWND wndHandle, bool windowed);
	HRESULT createShaders();
	void setMetaballs(const vector<Sphere>& metaballs);
	void setLines(const vector<Line>& lines);
	void setBoxes(const vector<Box>& boxes);
	void setCameraPos(glm::vec3 pos); // Set camera position before setting boxes / metaballs
	void swapBuffer();
	~Graphics();
};
