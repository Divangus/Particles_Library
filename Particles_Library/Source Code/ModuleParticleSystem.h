#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib.h"
#include "Emitter.h"

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

	ParticleProps particleProps;
	Emitter emitter;
};

