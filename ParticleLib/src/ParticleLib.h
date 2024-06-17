#pragma once
#include "Globals.h"
#include "Emitter.h"
#include "Loader.h"
#include "Camera.h"

#include <ctime>

class Particles
{
public:
	void Init(float screen_width, float screen_height);

	void CreateEmitter(std::string name);

	void CreateEmitter(std::string name, mat4 ViewMatrix);

	void CreateEmitter(std::string name, ParticleProps particleProperties);

	void CreateEmitter(std::string name, ParticleProps particleProperties, mat4 ViewMatrix);

	void UpdateParticles();

	void RenderParticles();

	void AddTexture(std::string name, std::string path);

	void AddAnimatedTexture(std::string name, std::string path, int atlasColumn, int atlasRows);

	void RemoveTexture(std::string name);

	//Set the max numbers of Particles for emitter (default to 10000)
	void setMaxParticles(int MaxParticles);

	Emitter* GetEmitter(std::string name);

private:
	vec2 displaySize;
	std::clock_t lastTime;
	float deltaTime;

	GLuint shaderProgram;
	std::vector<Emitter*> emittersList;
};
