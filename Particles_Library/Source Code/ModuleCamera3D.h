#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib.h"


class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Rotation();

	void SetCam();
	void GenBuffer();

	float* GetViewMatrix();
	float* GetProjectionMatrix();

	void LookAt(const float3& target);
	
	float mouseSens = 0.50f;

	Frustum FrustumCam;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;

	int FOV = 60.0f;

	unsigned int cameraBuffer;
	unsigned int frameBuffer;

	float3 target;

private:

	bool click = false;
	unsigned int renderObjBuffer;

};