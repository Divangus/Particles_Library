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
	particleProps.endScale = float3::zero;
	particleProps.scaleVariaton = float3::one;
	particleProps.speed = float3(0.0f, 10.0f, 0.0f);
	particleProps.speedVariation = float3(5.0f, 1.0f, 5.0f);
	particleProps.Color = float4(0.0f, 0.0f, 1.0f, 1.0f); //r g b a
	particleProps.endColor = float4(1.0f, 1.0f, 1.0f, 1.0f); //r g b a

	particleProps.gravity = true;

	particleProps.LifeTime = 5.0f;

	emitter.typeBB = BILLBOARDTYPE::SCREENALIGN;
	currentBBoard = BBtype[0];

	emitter.ParticleBuffer();

	return ret;
}

update_status ModuleParticleSystem::Update(float dt)
{
	//Emit particles and update them

	emitter.Emit(particleProps);

	emitter.Update(dt);

	ParticlesMenu();

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

void ModuleParticleSystem::ParticlesMenu()
{
	if (particleMenu)
	{
		ImGui::Begin("Particles Editor");

		ImGui::Text("X\t\t Y\t\t Z");
		ImGui::InputFloat3("Particle Begin Scale", particleProps.beginScale.ptr());

		ImGui::Text("X\t\t Y\t\t Z");
		ImGui::InputFloat3("Particle End Scale", particleProps.endScale.ptr());
		ImGui::NewLine();

		ImGui::Text("X\t\t Y\t\t Z");
		ImGui::InputFloat3("Speed", particleProps.speed.ptr());
		ImGui::Text("X\t\t Y\t\t Z");
		ImGui::InputFloat3("Speed Variaton", particleProps.speedVariation.ptr());
		ImGui::NewLine();

		ImGui::InputFloat("Life Time", &particleProps.LifeTime);
		ImGui::NewLine();

		ImGui::ColorEdit4("Color", particleProps.Color.ptr());
		ImGui::ColorEdit4("End Color", particleProps.endColor.ptr());
		ImGui::NewLine();

		ImGui::Checkbox("Gravity", &particleProps.gravity);
		ImGui::NewLine();

		if (ImGui::CollapsingHeader("BillBoard: ", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::BeginMenu("Select your BillBoard"))
			{

				if (ImGui::MenuItem("Screen Align BillBoard"))
				{
					emitter.typeBB = BILLBOARDTYPE::SCREENALIGN;
					currentBBoard = BBtype[0];
				}
				else if (ImGui::MenuItem("World Align BillBoard"))
				{
					emitter.typeBB = BILLBOARDTYPE::WORLDALIGN;
					currentBBoard = BBtype[1];
				}/*
				else if (ImGui::MenuItem("Axis Align BillBoard"))
				{
					emitter.typeBB = BILLBOARDTYPE::AXISALIGN;
					currentBBoard = BBtype[2];
				}*/
				else if (ImGui::MenuItem("No Align BillBoard"))
				{
					emitter.typeBB = BILLBOARDTYPE::NO_ALIGN;
					currentBBoard = BBtype[3];
				}

				ImGui::End();
			}

			ImGui::TextColored(ImVec4(0, 1, 0, 1), currentBBoard.c_str());
		}
		ImGui::End();
	}	
}