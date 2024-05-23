#pragma once
#include "Globals.h"

struct ParticleProps
{
	vec3 pos, rot;
	vec3 speed, speedVariation;
	vec4 Color, endColor;

	vec3 beginScale, endScale, scaleVariaton;

	bool texture = true;

	float LifeTime = 1.0f;
};

struct Particle
{
	vec3 pos, rot, scale;
	mat4x4 transformMat = IdentityMatrix;
	vec3 speed;
	vec4 Color, endColor;

	vec3 beginScale, endScale;

	float LifeTime = 1.0f, LifeRemaining = 0.0f;

	bool Active = false;

	void SetTransformMatrix();
	void SetTransform(mat4x4 matrix);
	mat4x4 GetTransformMatrix();
};
class Emitter
{
	Emitter();
	~Emitter();

	void Update();

	void Emit(ParticleProps& particleProps);

	void ParticleBuffer();
	void Render();

	bool text = true;

	std::vector<Particle> ParticleList;
	uint32_t currentParticle = 999;

	uint id_indices = 0;
	uint id_vertices = 0;
};

