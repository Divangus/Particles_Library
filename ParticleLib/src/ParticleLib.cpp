#include "ParticleLib.h"

void Particles::Init(float screen_width, float screen_height)
{
	shaderProgram = Loader::Shader("ParticleVShader.glsl", "ParticleFShader.glsl");
	lastTime = std::clock();
	displaySize.x = screen_width;
	displaySize.y = screen_height;
}

void Particles::CreateEmitter(std::string name)
{
	Emitter* emitter = new Emitter(name);

	emitter->ParticleBuffer();

	emittersList.push_back(emitter);
}

void Particles::CreateEmitter(std::string name, mat4 ViewMatrix)
{
	Emitter* emitter = new Emitter(name, ViewMatrix);

	emitter->ParticleBuffer();

	emittersList.push_back(emitter);
}

void Particles::CreateEmitter(std::string name, ParticleProps particleProperties)
{
	Emitter* emitter = new Emitter(name, particleProperties);

	emitter->ParticleBuffer();

	emittersList.push_back(emitter);
}

void Particles::CreateEmitter(std::string name, ParticleProps particleProperties, mat4 ViewMatrix)
{
	Emitter* emitter = new Emitter(name, particleProperties, ViewMatrix);

	emitter->ParticleBuffer();

	emittersList.push_back(emitter);
}

//Update Particles
void Particles::UpdateParticles()
{
	std::clock_t currentTime = std::clock();
	deltaTime = float(currentTime - lastTime) / CLOCKS_PER_SEC;
	lastTime = currentTime;

	for (int i = 0; i < emittersList.size(); i++)
	{
		emittersList[i]->Emit();
		emittersList[i]->Update(deltaTime);
	}
}

//Render Particles
void Particles::RenderParticles()
{
	for (int i = 0; i < emittersList.size(); i++)
	{
		emittersList[i]->Render(shaderProgram);
	}
}

void Particles::AddTexture(std::string name, std::string path)
{
}

void Particles::AddAnimatedTexture(std::string name, std::string path, int atlasColumn, int atlasRows)
{
}

void Particles::RemoveTexture(std::string name)
{
}

Emitter* Particles::GetEmitter(std::string name)
{
	for (int i = 0; i < emittersList.size(); i++) 
	{
		if (emittersList[i]->name == name) return emittersList[i];
	}

	return nullptr;
}



