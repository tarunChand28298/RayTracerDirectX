#include "RenderingSystem.h"

RenderingSystem::RenderingSystem(Scene& srcScene, RenderingWindow& trgWindow) 
			:
	sourceScene(srcScene),
	targetWindow(trgWindow)
{
	Initialize();
	sourceScene.OnSphereChange([this](const Scene&) { this->ResizeInputBuffer(); });
}

RenderingSystem::~RenderingSystem()
{
	Destroy();
}

void RenderingSystem::Initialize()
{
	SetupDevice();
	SetupShader();
	SetupInputs();
	ResizeInputBuffer();
	SetupOutput();
}

void RenderingSystem::Destroy()
{
	device->Release();
	deviceContext->Release();
	raytracingShader->Release();
	inputSpheresBuffer->Release();
	inputConstBuffer1->Release();
	inputConstBuffer2->Release();
	swapChain->Release();
	inputBufferView->Release();
	backbufferView->Release();
}

void RenderingSystem::Render()
{
	SetupAttachments();
	UpdateInputs();
	deviceContext->Dispatch(targetWindow.width / 8, targetWindow.height / 8, 1);
	swapChain->Present(1, 0);
}

void RenderingSystem::SetupDevice()
{
	DXGI_SWAP_CHAIN_DESC scd = {};
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.SampleDesc.Count = 1;
	scd.BufferUsage = DXGI_USAGE_UNORDERED_ACCESS;
	scd.BufferCount = 2;
	scd.OutputWindow = targetWindow.windowHandle;
	scd.Windowed = true;

	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG, nullptr, 0, D3D11_SDK_VERSION, &scd, &swapChain, &device, nullptr, &deviceContext);
}

void RenderingSystem::SetupShader()
{
	ID3D10Blob* shaderData;
	D3DReadFileToBlob((LPCWSTR)L"ComputeShader.cso", &shaderData);

	device->CreateComputeShader(shaderData->GetBufferPointer(), shaderData->GetBufferSize(), nullptr, &raytracingShader);
	deviceContext->CSSetShader(raytracingShader, nullptr, 0);

	shaderData->Release();
}

void RenderingSystem::ResizeInputBuffer()
{
	inputSpheresBuffer->Release();
	inputBufferView->Release();

	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof(Sphere)*sourceScene.spheresList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.StructureByteStride = sizeof(Sphere);
	bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	D3D11_SUBRESOURCE_DATA srd = {};
	srd.pSysMem = sourceScene.spheresList.data();
	srd.SysMemPitch = 0;
	srd.SysMemSlicePitch = 0;

	device->CreateBuffer(&bd, &srd, &inputSpheresBuffer);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
	srvd.Format = DXGI_FORMAT_UNKNOWN;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvd.Buffer.ElementOffset = 0;
	srvd.Buffer.ElementWidth = sizeof(Sphere);
	srvd.Buffer.FirstElement = 0;
	srvd.Buffer.NumElements = sourceScene.spheresList.size();

	device->CreateShaderResourceView(inputSpheresBuffer, &srvd, &inputBufferView);
}

void RenderingSystem::SetupInputs()
{
	//Input Resource Buffer:
	{
		D3D11_BUFFER_DESC bd = {};
		bd.ByteWidth = sizeof(Sphere)*sourceScene.spheresList.size();
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.StructureByteStride = sizeof(Sphere);
		bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

		D3D11_SUBRESOURCE_DATA srd = {};
		srd.pSysMem = sourceScene.spheresList.data();
		srd.SysMemPitch = 0;
		srd.SysMemSlicePitch = 0;

		device->CreateBuffer(&bd, &srd, &inputSpheresBuffer);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
		srvd.Format = DXGI_FORMAT_UNKNOWN;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvd.Buffer.ElementOffset = 0;
		srvd.Buffer.ElementWidth = sizeof(Sphere);
		srvd.Buffer.FirstElement = 0;
		srvd.Buffer.NumElements = sourceScene.spheresList.size();

		device->CreateShaderResourceView(inputSpheresBuffer, &srvd, &inputBufferView);
	}

	//Input Constant Buffer:
	{
		D3D11_BUFFER_DESC cbd1 = {};
		cbd1.ByteWidth = sizeof(Camera);
		cbd1.Usage = D3D11_USAGE_DEFAULT;
		cbd1.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd1.CPUAccessFlags = 0u;
		cbd1.StructureByteStride = 0;
		cbd1.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA csd1 = {};
		csd1.pSysMem = &sourceScene.cam;
		device->CreateBuffer(&cbd1, &csd1, &inputConstBuffer1);
		deviceContext->CSSetConstantBuffers(0, 1, &inputConstBuffer1);

		D3D11_BUFFER_DESC cbd2 = {};
		cbd2.ByteWidth = sizeof(Camera);
		cbd2.Usage = D3D11_USAGE_DEFAULT;
		cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd2.CPUAccessFlags = 0u;
		cbd2.StructureByteStride = 0;
		cbd2.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA csd2 = {};
		csd2.pSysMem = &sourceScene.cam;
		device->CreateBuffer(&cbd2, &csd2, &inputConstBuffer2);
		deviceContext->CSSetConstantBuffers(0, 1, &inputConstBuffer2);
	}
}

void RenderingSystem::SetupAttachments()
{
	UINT uavInitCount = 0;
	deviceContext->CSSetShader(raytracingShader, nullptr, 0);
	deviceContext->CSSetShaderResources(0, 1, &inputBufferView);
	deviceContext->CSSetConstantBuffers(0, 1, &inputConstBuffer1);
	deviceContext->CSSetConstantBuffers(1, 1, &inputConstBuffer2);
	deviceContext->CSSetUnorderedAccessViews(0, 1, &backbufferView, &uavInitCount);
}

void RenderingSystem::UpdateInputs()
{
	deviceContext->UpdateSubresource(inputSpheresBuffer, 0, 0, sourceScene.spheresList.data(), 0, 0);
	deviceContext->UpdateSubresource(inputConstBuffer1, 0, 0, &sourceScene.cam, 0, 0);
	deviceContext->UpdateSubresource(inputConstBuffer2, 0, 0, &sourceScene.dirLight, 0, 0);
}

void RenderingSystem::SetupOutput()
{
	ID3D11Texture2D* backBuffer = nullptr;
	D3D11_TEXTURE2D_DESC backBufferDesc = {};
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	backBuffer->GetDesc(&backBufferDesc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.Flags = 0;
	uavDesc.Buffer.NumElements = backBufferDesc.ArraySize;
	uavDesc.Format = backBufferDesc.Format;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	UINT uavInitialCount = 0;

	device->CreateUnorderedAccessView(backBuffer, &uavDesc, &backbufferView);


	backBuffer->Release();
}
