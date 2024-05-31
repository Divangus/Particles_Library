#pragma once
#include "Globals.h"
#include "MathGeoLib.h"

#include <algorithm>

#define MaxParticles 1000

enum class BILLBOARDTYPE
{
	NO_ALIGN,
	SCREENALIGN,
	WORLDALIGN,
	AXISALIGN,
};

enum class AXISALIGNBB
{
	X_AXIS,
	Y_AXIS,
	Z_AXIS,
};

struct ParticleProps
{
	float3 pos, rot;
	float3 speed, speedVariation;
	float4 Color, endColor;

	float3 beginScale, endScale, scaleVariaton;

	bool gravity = false;

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

	float cameradistance;

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
	void Render(GLuint shader);

	void ScreenAlignBBoard(Particle& particle);
	void WorldAlignBBoard(Particle& particle);
	void AxisAlignBBoard(Particle& particle);

	void SortParticles();

	bool text = false;

	std::vector<Particle> ParticleList;
	uint32_t currentParticle = MaxParticles - 1;

	BILLBOARDTYPE typeBB = BILLBOARDTYPE::NO_ALIGN;
	AXISALIGNBB alignAxis = AXISALIGNBB::Y_AXIS;

	uint id_indices = 0;
	uint id_vertices = 0;
	uint textID = 0;

	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ebo = 0;
};

