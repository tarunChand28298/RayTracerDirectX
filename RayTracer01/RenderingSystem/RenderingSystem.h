#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include "RenderingWindow.h"
#include "..\SceneSystem\Scene.h"

class RenderingSystem
{
public:
	RenderingSystem(Scene& srcScene, RenderingWindow& trgWindow);
	~RenderingSystem();

	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;
	ID3D11ComputeShader* raytracingShader = nullptr;
	ID3D11Buffer* inputSpheresBuffer = nullptr;
	ID3D11Buffer* inputConstBuffer1 = nullptr;
	ID3D11Buffer* inputConstBuffer2 = nullptr;
	IDXGISwapChain* swapChain = nullptr;
	ID3D11ShaderResourceView* inputBufferView = nullptr;
	ID3D11UnorderedAccessView* backbufferView = nullptr;

	Scene& sourceScene;
	RenderingWindow& targetWindow;

	void Initialize();
	void Destroy();
	void Render();

	void SetupDevice();
	void SetupShader();
	void ResizeInputBuffer();
	void SetupInputs();
	void SetupAttachments();
	void UpdateInputs();
	void SetupOutput();
};
