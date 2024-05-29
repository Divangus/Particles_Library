#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleInput.h"
#include "MemLeaks.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{	
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;
	target = float3(0.0f, 2.0f, 0.0f);

	SetCam();
	GenBuffer();

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	int wheel = -App->input->GetMouseZ();
	float speed = 8.0f * dt;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * 2 * dt;

	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) FrustumCam.pos.y += speed;
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) FrustumCam.pos.y -= speed;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) FrustumCam.pos += FrustumCam.front * speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) FrustumCam.pos -= FrustumCam.front * speed;


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) FrustumCam.pos -= FrustumCam.WorldRight() * speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) FrustumCam.pos += FrustumCam.WorldRight() * speed;

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
			LookAt(target);

			float TargetDist = FrustumCam.pos.Distance(target);

			Rotation();

			FrustumCam.pos = target + (FrustumCam.front * -TargetDist);
	}

	if (wheel != 0) FrustumCam.pos += FrustumCam.front * 10 * -wheel;


	return UPDATE_CONTINUE;
}

void ModuleCamera3D::Rotation()
{
	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	Quat dir;
	FrustumCam.WorldMatrix().Decompose(float3(), dir, float3());

	if (dy != 0) {
		float DeltaY = (float)dy * mouseSens;

		Quat Y = Quat::identity;
		Y.SetFromAxisAngle(float3(1.0f, 0.0f, 0.0f), DeltaY * DEGTORAD);

		dir = dir * Y;
	}

	if (dx != 0) {
		float DeltaX = (float)dx * mouseSens;

		Quat X = Quat::identity;
		X.SetFromAxisAngle(float3(0.0f, 1.0f, 0.0f), DeltaX * DEGTORAD);

		dir = X * dir;
	}

	float4x4 matrix = FrustumCam.WorldMatrix();
	matrix.SetRotatePart(dir.Normalized());
	FrustumCam.SetWorldMatrix(matrix.Float3x4Part());
}

void ModuleCamera3D::SetCam()
{
	FrustumCam.type = FrustumType::PerspectiveFrustum;
	FrustumCam.nearPlaneDistance = 0.1f;
	FrustumCam.farPlaneDistance = 500.f;
	FrustumCam.front = float3::unitZ;
	FrustumCam.up = float3::unitY;

	FrustumCam.verticalFov = FOV * DEGTORAD;
	FrustumCam.horizontalFov = 2.0f * atanf(tanf(FrustumCam.verticalFov / 2.0f) * 1.7f);

	FrustumCam.pos = float3(0, 2, 10);
	LookAt(target);
}

void ModuleCamera3D::GenBuffer()
{
	glGenFramebuffers(1, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenTextures(1, &cameraBuffer);
	glBindTexture(GL_TEXTURE_2D, cameraBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	float color[4] = { 0.1,0.1,0.1,0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cameraBuffer, 0);

	glGenRenderbuffers(1, &renderObjBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderObjBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderObjBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		LOG("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

float* ModuleCamera3D::GetViewMatrix()
{

	viewMatrix = FrustumCam.ViewMatrix();

	viewMatrix.Transpose();

	return viewMatrix.ptr();
}

float* ModuleCamera3D::GetProjectionMatrix()
{

	projectionMatrix = FrustumCam.ProjectionMatrix();

	projectionMatrix.Transpose();

	return projectionMatrix.ptr();
}

void ModuleCamera3D::LookAt(const float3& target)
{
	FrustumCam.front = (target - FrustumCam.pos).Normalized();
	float3 X = float3(0, 1, 0).Cross(FrustumCam.front).Normalized();
	FrustumCam.up = FrustumCam.front.Cross(X);
}

