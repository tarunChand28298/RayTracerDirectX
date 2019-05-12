#include "Scene.h"

Scene::Scene()
{
	Start();
}

Scene::~Scene()
{
}

void Scene::Start()
{
	AddSphere({0,  3,  0}, 1, {1, 1, 1}, {0.5, 0.5, 0.5});
	AddSphere({3,  3,  0}, 1, {1, 1, 0}, {0.5, 0.5, 0.5});
	AddSphere({-3, 3,  0}, 1, {1, 1, 0}, {0.5, 0.5, 0.5});
	AddSphere({0,  3,  4}, 1, {1, 1, 0}, {0.5, 0.5, 0.5});
	AddSphere({0,  3, -4}, 1, {1, 1, 0}, {0.5, 0.5, 0.5});

	float NearZ = 1.0f;
	float FarZ = 10000.0f;
	float FovAngleYDeg = 120.0f;
	float FovAngleY = FovAngleYDeg * 0.0174533f;
	DirectX::XMMATRIX cameraToWorldt = DirectX::XMMATRIX(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.97f, 0.21f, 8.8f, 0.0f, 0.21f, -0.97f, -17.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMMATRIX inverseProjectiont = DirectX::XMMatrixInverse(nullptr, DirectX::XMMatrixPerspectiveFovLH(FovAngleY, float(800) / float(600), NearZ, FarZ));
	cam.cameraToWorld = DirectX::XMMatrixTranspose(cameraToWorldt);
	cam.inverseProjectionMatrix = DirectX::XMMatrixTranspose(inverseProjectiont);

	dirLight = { 0.5, -1.0, 0, 1};
}

void Scene::Update()
{
	float yPos = 5.0f + DirectX::XMScalarSin(sinAngle / 0.0174533f)*2.0f;
	spheresList[0].position.y = yPos;

	sinAngle += 0.001f;
	if (sinAngle > 360.0f) {
		sinAngle = 0.0f;
	}

	timer += 0.01;
	if (timer > 2.0f && timer < 3.0f) {

		AddSphere({ 2 * 4.0f, 3 * 2.0f, float(spawnLocationZ)  *4.0f }, 1, { 0.5f, 0.5f, 1.0f }, { 0.1f, 0.3f, 0.3f });
		spawnLocationZ++;
	}
	if (timer > 4.0f && timer < 4.9f) {
		RemoveSphere(spheresList.size() - 1);
		spawnLocationZ++;
	}

}

void Scene::AddSphere(DirectX::XMFLOAT3 pos, float rad, DirectX::XMFLOAT3 albedo, DirectX::XMFLOAT3 specular)
{
	
	Sphere s = { pos, rad, albedo, specular };
	spheresList.push_back(s);

	for (std::function<void(const Scene&)> clbk : subscribedFxns){	
		clbk(*this);
	}
}

void Scene::RemoveSphere(int index)
{
	spheresList.erase(spheresList.begin() + index);

	for (std::function<void(const Scene&)> clbk : subscribedFxns) { 
		clbk(*this); 
	}
}

void Scene::OnSphereChange(std::function<void(const Scene&)> subscriber)
{
	subscribedFxns.push_back(subscriber);
}
