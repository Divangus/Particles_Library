#ifndef __PARTICLELIB_H__ 
#define __PARTICLELIB_H__ 

#include "ParticleGlobals.h"
#include "Emitter.h"
#include "Loader.h"

#include <ctime>

namespace Particles
{
	bool Init(float screen_width, float screen_height);

	void CleanUp();

	void CreateEmitter(std::string name);

	void CreateEmitter(std::string name, glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp);

	void CreateEmitter(std::string name, ParticleProps particleProperties);

	void CreateEmitter(std::string name, ParticleProps particleProperties, glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp);

	void UpdateParticles();

	void UpdateParticles(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp);

	void OnResize(float new_screen_width, float new_screen_height);

	void RenderParticles();

	void AddTexture(std::string name, std::string path);

	void AddAnimatedTexture(std::string name, std::string path, int atlasColumn, int atlasRows);

	void RemoveTexture(std::string name);

	//Set the max numbers of Particles for emitter (default to 10000)
	//void setMaxParticles(int MaxParticles);

	Emitter* GetEmitter(std::string name);

	extern glm::vec2 p_displaySize;
	extern std::clock_t p_lastTime;
	extern float p_deltaTime;
	extern GLuint p_shaderProgram;
	extern std::vector<Emitter*> p_emittersList;
};

#endif // __PARTICLELIB_H__
