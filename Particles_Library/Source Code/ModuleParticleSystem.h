#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib.h"
#include "Emitter.h"

#include <string>

class ModuleParticleSystem : public Module
{
public:
	ModuleParticleSystem(Application* app, bool start_enabled = true);
	~ModuleParticleSystem();

	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void RenderParticles();
	void ParticlesMenu();

	void LoadParticleTexture(std::string fileName);
	void ClearTexture();

	ParticleProps particleProps;
	Emitter emitter;

	std::string BBtype[4] = { "Screen Align","World Align","Axis Align","No Align" };
	std::string currentBBoard;

	std::string textureName, defaultName;

	bool particleMenu = true;
};

