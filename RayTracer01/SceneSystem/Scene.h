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
};
