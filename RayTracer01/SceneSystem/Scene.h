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
	DirectX::XMFLOAT3 mousePosToWorld(POINT point) {

		//normalize:
		RECT clientRegion; GetClientRect(mouse.relativeToWindow, &clientRegion);
		float u, v;
		u = float((point.x + 0.5f) / float(clientRegion.right - clientRegion.left) * 2.0f - 1.0f);
		v = float((point.y + 0.5f) / float(clientRegion.bottom - clientRegion.top) * 2.0f - 1.0f);
		v = -v;

		//shoot out ray:
		DirectX::XMVECTOR origin = DirectX::XMVector4Transform(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), cam.cameraToWorld);
		float originX = DirectX::XMVectorGetX(origin);
		float originY = DirectX::XMVectorGetY(origin);
		float originZ = DirectX::XMVectorGetZ(origin);

		DirectX::XMVECTOR direction = DirectX::XMVector4Transform(DirectX::XMVectorSet(u, v, 0.0f, 1.0f), cam.inverseProjectionMatrix);
		float directionX = DirectX::XMVectorGetX(direction);
		float directionY = DirectX::XMVectorGetY(direction);
		float directionZ = DirectX::XMVectorGetZ(direction);
		direction = DirectX::XMVector4Transform(DirectX::XMVectorSet(directionX, directionY, directionZ, 0.0f), cam.cameraToWorld);
		directionX = DirectX::XMVectorGetX(direction);
		directionY = DirectX::XMVectorGetY(direction);
		directionZ = DirectX::XMVectorGetZ(direction);
		directionX /= sqrt((directionX*directionX) + (directionY*directionY) + (directionZ*directionZ));
		directionY /= sqrt((directionX*directionX) + (directionY*directionY) + (directionZ*directionZ));
		directionZ /= sqrt((directionX*directionX) + (directionY*directionY) + (directionZ*directionZ));

		float t = originY / directionY;
		return DirectX::XMFLOAT3(originX + (directionX * t), originY + (directionY * t), originZ + (directionZ * t));
	}
};
