#include "ParticleLib.h"

namespace Particles
{
	glm::vec2 p_displaySize;
	std::clock_t p_lastTime;
	float p_deltaTime;
	GLuint p_shaderProgram;
	std::vector<Emitter*> p_emittersList;

	bool Init(float screen_width, float screen_height)
	{
		if (glGetString(GL_VERSION) == NULL) {

			std::cout << "GL not initialized" << std::endl;	
			return false;

		}
		p_shaderProgram = ParticleLoader::Shader("ParticleVShader.glsl", "ParticleFShader.glsl");
		p_lastTime = std::clock();
		p_displaySize.x = screen_width;
		p_displaySize.y = screen_height;

		return true;
	}

	void CleanUp()
	{
		for (int i = 0; i < p_emittersList.size(); i++)
		{
			delete p_emittersList[i];
		}
		p_emittersList.clear();
	}

	void CreateEmitter(std::string name)
	{
		Emitter* emitter = new Emitter(name);

		emitter->SetAspectRatio(p_displaySize.x, p_displaySize.y);

		emitter->ParticleBuffer();

		p_emittersList.push_back(emitter);
	}

	void CreateEmitter(std::string name, glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp)
	{
		Emitter* emitter = new Emitter(name, cameraPos, cameraFront, cameraUp);

		emitter->SetAspectRatio(p_displaySize.x, p_displaySize.y);

		emitter->ParticleBuffer();

		p_emittersList.push_back(emitter);
	}

	void CreateEmitter(std::string name, ParticleProps particleProperties)
	{
		Emitter* emitter = new Emitter(name, particleProperties);

		emitter->SetAspectRatio(p_displaySize.x, p_displaySize.y);

		emitter->ParticleBuffer();

		p_emittersList.push_back(emitter);
	}

	void CreateEmitter(std::string name, ParticleProps particleProperties, glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp)
	{
		Emitter* emitter = new Emitter(name, particleProperties, cameraPos, cameraFront, cameraUp);

		emitter->SetAspectRatio(p_displaySize.x, p_displaySize.y);

		emitter->ParticleBuffer();

		p_emittersList.push_back(emitter);
	}

	//Update Particles
	void UpdateParticles()
	{
		std::clock_t currentTime = std::clock();
		p_deltaTime = float(currentTime - p_lastTime) / CLOCKS_PER_SEC;
		p_lastTime = currentTime;

		for (int i = 0; i < p_emittersList.size(); i++)
		{
			if (p_emittersList[i]->EmissionControl)
			{
				p_emittersList[i]->accumulator += p_deltaTime;
				while (p_emittersList[i]->accumulator > 1.0 / p_emittersList[i]->ParticlesPerSecond) {
					p_emittersList[i]->Emit();
					p_emittersList[i]->accumulator -= 1.0 / p_emittersList[i]->ParticlesPerSecond;
				}

			}
			else {
				p_emittersList[i]->Emit();
			}

			p_emittersList[i]->Update(p_deltaTime);
		}
	}

	void UpdateParticles(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp)
	{
		std::clock_t currentTime = std::clock();
		p_deltaTime = float(currentTime - p_lastTime) / CLOCKS_PER_SEC;
		p_lastTime = currentTime;

		for (int i = 0; i < p_emittersList.size(); i++)
		{
			p_emittersList[i]->UpdateCamera(cameraPos, cameraFront, cameraUp);

			if (p_emittersList[i]->EmissionControl)
			{
				p_emittersList[i]->accumulator += p_deltaTime;
				while (p_emittersList[i]->accumulator > 1.0 / p_emittersList[i]->ParticlesPerSecond) {
					p_emittersList[i]->Emit();
					p_emittersList[i]->accumulator -= 1.0 / p_emittersList[i]->ParticlesPerSecond;
				}

			}
			else {
				p_emittersList[i]->Emit();
			}

			p_emittersList[i]->Update(p_deltaTime);
		}
	}


	//Render Particles
	void RenderParticles()
	{
		for (int i = 0; i < p_emittersList.size(); i++)
		{
			p_emittersList[i]->Render(p_shaderProgram);
		}
	}

	void AddTexture(std::string name, std::string path)
	{
		unsigned int texture = ParticleLoader::LoadTexture2D(path.c_str());
		GetEmitter(name)->SetTexture(texture);
	}

	void AddAnimatedTexture(std::string name, std::string path, int atlasColumn, int atlasRows)
	{
		unsigned int texture = ParticleLoader::LoadTexture2D(path.c_str());
		GetEmitter(name)->SetAnimatedTexture(texture, atlasRows, atlasColumn);
	}

	void RemoveTexture(std::string name)
	{
		GetEmitter(name)->CleanTexture();
	}

	Emitter* GetEmitter(std::string name)
	{
		for (int i = 0; i < p_emittersList.size(); i++)
		{
			if (p_emittersList[i]->name == name) return p_emittersList[i];
		}

		return nullptr;
	}

	void SetParticlesPerSecond(std::string name, bool active, int particlesPerSecond)
	{
		GetEmitter(name)->SetParticlesPerSecond(active, particlesPerSecond);
	}

	void SetBillboard(std::string name, BILLBOARDTYPE typeBB)
	{
		GetEmitter(name)->SetBillboardType(typeBB);
	}

	void SetAxisBillboard(std::string name, AXISALIGNBB alignAxis)
	{
		GetEmitter(name)->SetAxisBillboardType(alignAxis);
	}

	void setMaxParticles(std::string name, int MaxParticles)
	{
		GetEmitter(name)->SetMaxParticles(MaxParticles);
	}

	void OnResize(float new_screen_width, float new_screen_height)
	{
		p_displaySize.x = new_screen_width;
		p_displaySize.y = new_screen_height;

		for (int i = 0; i < p_emittersList.size(); i++)
		{
			p_emittersList[i]->SetAspectRatio(p_displaySize.x, p_displaySize.y);
		}
	}
}




