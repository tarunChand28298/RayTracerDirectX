#pragma once
#include <vector>
#include <DirectXMath.h>
#include <functional>

struct Sphere {
	float x, y, z;
	float radius;
	float albedoX, albedoY, albedoZ;
	float specX, specY, specZ;
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

	float sinAngle = 0.0f;
	float timer = 0.0f;
	int spawnLocationZ = 0;

	void Start();
	void Update();

	void AddSphere(float x, float y, float z, float r, float ax, float ay, float az, float sx, float sy, float sz);
	void RemoveSphere(int index);

	std::vector< std::function<void(const Scene&)>> callbacks;
	void OnSphereChange(std::function<void(const Scene&)> callback);
};
