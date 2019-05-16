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
	ID3D11BlendState* blendState;
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	ID3D11VertexShader* movingBoxVertexShader = nullptr;
	ID3D11PixelShader* movingBoxPixelShader = nullptr;
	ID3D11VertexShader* staticBoxVertexShader = nullptr;
	ID3D11PixelShader* staticBoxPixelShader = nullptr;
	ID3D11VertexShader* laserVertexShader = nullptr;
	ID3D11PixelShader* laserPixelShader = nullptr;

	ID3D11InputLayout* boxVertexLayout;
	ID3D11InputLayout* laserVertexLayout;
	ID3D11InputLayout* quadVertexLayout;
	ID3D11Buffer* quadBuffer;

	static const int maxMetaballs = 15;
	static const int maxLasers = 100;
	static const int maxStaticBoxes = 200;
	static const int maxMovingBoxes = 50;

	ID3D11Buffer* staticBoxVertexBuffer;
	ID3D11Buffer* staticBoxColorBuffer;
	int staticBoxInstances = 0;

	ID3D11Buffer* boxVertexBuffer;
	ID3D11Buffer* movingBoxTransformBuffer;
	int movingBoxInstances = 0;
	
	ID3D11Buffer* metaballBuffer;
	int laserInstances = 0;
	
	ID3D11Buffer* laserTransformBuffer;
	ID3D11Buffer* viewProjBuffer;
	ID3D11Buffer* cameraBuffer;
	ID3D11Buffer* cornerBuffer;

	glm::vec3 camera;
	glm::mat4 proj;
	glm::mat4 invProj;
	glm::mat4 viewProj;
	glm::mat4 invTransposedWVP;
	glm::vec3 boxVertices[36 * 2]; // position + normals for 36 triangles
	glm::vec4 frustumCorners[4];
	Box cullingBox;

	HRESULT createDeviceContext(HWND wndHandle, bool windowed);
	HRESULT createResources(HWND wndHandle);
	HRESULT createQuadData();
	HRESULT createBoxData();
	void createCBuffers();
	void updateFrustumCorners();
	void updateCullingBox();
public:
	HRESULT init(HWND wndHandle, bool windowed);
	HRESULT createShaders();
	void setMetaballs(const vector<Sphere>& metaballs);
	void setLasers(const vector<Line>& lines);
	void setMovingBoxes(const vector<Box>& boxes);
	void setStaticBoxes(const vector<Box>& boxes);
	void setCameraPos(glm::vec3 pos); // Set camera position before setting boxes / metaballs
	void swapBuffer();
	~Graphics();
};
