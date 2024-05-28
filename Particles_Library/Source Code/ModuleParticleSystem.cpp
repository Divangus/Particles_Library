#include "Application.h"
#include "ModuleParticleSystem.h"
#include "MemLeaks.h"

ModuleParticleSystem::ModuleParticleSystem(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleParticleSystem::~ModuleParticleSystem()
{}

bool ModuleParticleSystem::Start()
{
	LOG("Setting Particles");
	bool ret = true;

	//Default Particle
	particleProps.pos = float3::zero;
	particleProps.rot = float3::zero;
	particleProps.beginScale = float3::one;
	particleProps.endScale = float3::one;
	particleProps.speed = float3(0.0f, 1.0f, 0.0f);
	particleProps.speedVariation = float3::one;
	particleProps.Color = float4(255.0f, 0.0f, 0.0f, 1.0f); //r g b a
	particleProps.endColor = float4(0.0f, 0.0f, 255.0f, 0.0f); //r g b a

	particleProps.LifeTime = 5.0f;

	emitter.ParticleBuffer();

	return ret;
}

update_status ModuleParticleSystem::Update(float dt)
{
	//Emit particles and update them

	emitter.Emit(particleProps);

	emitter.Update(dt);

	return UPDATE_CONTINUE;
}

update_status ModuleParticleSystem::PostUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

void ModuleParticleSystem::RenderParticles()
{
	emitter.Render();
}

// -----------------------------------------------------------------
bool ModuleParticleSystem::CleanUp()
{
	return true;
}