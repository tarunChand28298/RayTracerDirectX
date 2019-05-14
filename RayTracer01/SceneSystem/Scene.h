#pragma once
#include <vector>
#include <DirectXMath.h>
#include <functional>
#include "../InputSystem/Inputs.h"

struct Sphere {
	DirectX::XMFLOAT3 position;
	float radius;
	DirectX::XMFLOAT3 albedo;
	DirectX::XMFLOAT3 specular;
};

struct Camera {
	DirectX::XMMATRIX cameraToWorld;
	DirectX::XMMATRIX inverseProjectionMatrix;
};

struct DirectionalLight {
	float x, y, z;
	float intensity;
};

class Scene
{
public:
	Scene();
	~Scene();

	std::vector<Sphere> spheresList;
	Camera cam;
	DirectionalLight dirLight;

	Keyboard kbd;
	Mouse mouse;
	XBoxController controller;

	float sinAngle = 0.0f;
	float timer = 0.0f;
	int spawnLocationZ = 0;

	void Start();
	void Update();

	void AddSphere(DirectX::XMFLOAT3 pos, float rad, DirectX::XMFLOAT3 albedo, DirectX::XMFLOAT3 specular);
	void RemoveSphere(int index);


	std::vector<std::function<void(const Scene&)>> subscribedFunctions;
	void OnSphereChange(std::function<void(const Scene&)> subscriber);

	//for mouse testing:
	DirectX::XMFLOAT3 mouseToWorld(POINT point) {
		using namespace DirectX;

		//normalize:
		RECT clientRegion; GetClientRect(mouse.relativeToWindow, &clientRegion);
		
		float u = -(float(point.x - clientRegion.right / 2)) / float(clientRegion.right) * 35;
		float v = (float(point.y - clientRegion.bottom / 2)) / float(clientRegion.bottom) * 35 - 8;

		//shoot out ray:
		XMVECTOR origin = DirectX::XMVector4Transform(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), cam.cameraToWorld);
		XMVECTOR directionLocal = DirectX::XMVector4Transform(DirectX::XMVectorSet(u, v, 0.0f, 1.0f), cam.inverseProjectionMatrix);
		XMVECTOR directionGlobal = DirectX::XMVector4Transform(directionLocal, cam.cameraToWorld);
		XMVector3Normalize(directionGlobal);

		float t = -XMVectorGetY(origin) / XMVectorGetY(directionGlobal);

		return XMFLOAT3(XMVectorGetX(origin)+XMVectorGetX(directionGlobal)*t, XMVectorGetY(origin)+XMVectorGetY(directionGlobal)*t, XMVectorGetZ(origin)+XMVectorGetZ(directionGlobal)*t);

	}
};
