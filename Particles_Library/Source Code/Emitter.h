#pragma once
#include "Globals.h"
#include "MathGeoLib.h"

enum class BILLBOARDTYPE
{
	NO_ALIGN,
	SCREENALIGN,
	WORLDALIGN,
	AXISALIGN,
};

struct ParticleProps
{
	float3 pos, rot;
	float3 speed, speedVariation;
	float4 Color, endColor;

	float3 beginScale, endScale, scaleVariaton;

	bool gravity = false;

	//bool texture = true;

	float LifeTime = 1.0f;
};

struct Particle
{
	float3 pos, rot, scale;
	float4x4 transformMat = float4x4::identity;
	float3 speed;
	float4 Color, endColor;

	float3 beginScale, endScale;

	float LifeTime = 1.0f, LifeRemaining = 1.0f;

	bool gravity = false;

	bool Active = false;

	void SetTransformMatrix();
	void SetTransformMatrixWithQuat(Quat rotation);
	void SetTransform(float4x4 matrix);
	float4x4 GetTransformMatrix();
};
class Emitter
{
public:
	Emitter();
	~Emitter();

	void Update(float dt);

	void Emit(ParticleProps& particleProps);

	void ParticleBuffer();
	void Render();

	void ScreenAlignBBoard(Particle& particle);
	void WorldAlignBBoard(Particle& particle);
	void AxisAlignBBoard(Particle& particle);

	//bool text = true;

	std::vector<Particle> ParticleList;
	uint32_t currentParticle = 999;

	BILLBOARDTYPE typeBB = BILLBOARDTYPE::NO_ALIGN;

	uint id_indices = 0;
	uint id_vertices = 0;
};

