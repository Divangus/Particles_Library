#include "Application.h"
#include "ModuleParticleSystem.h"
#include "MemLeaks.h"
#include "ModuleTexture.h"

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

	emitter.alignAxis = AXISALIGNBB::Y_AXIS;
	currentAxis = AxisAlign[1];

	emitter.textID = App->texture->checkerID;
	defaultName = "NO TEXTURE LOADED (Drag and drop a PNG or DDS file to load it as texture)";
	textureName = defaultName;

	emitter.ParticleBuffer();

	return ret;
}

update_status ModuleParticleSystem::Update(float dt)
{
	//Emit particles and update them

	if (EmissionControl)
	{
		accumulator += dt;
		while (accumulator > 1.0 / ParticlesPerSecond) {
			emitter.Emit(particleProps);
			accumulator -= 1.0 / ParticlesPerSecond;
		}

	}
	else {
		emitter.Emit(particleProps);
	}

	emitter.Update(dt);

	if (particleMenu)
	{
		ParticlesMenu();
	}

	return UPDATE_CONTINUE;
}

update_status ModuleParticleSystem::PostUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

void ModuleParticleSystem::RenderParticles()
{
	emitter.Render(App->renderer3D->shaderProgram);
}

// -----------------------------------------------------------------
bool ModuleParticleSystem::CleanUp()
{
	return true;
}

void ModuleParticleSystem::ParticlesMenu()
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

	ImGui::Checkbox("Gravity", &particleProps.gravity);
	ImGui::NewLine();

	ImGui::InputFloat("Life Time", &particleProps.LifeTime);
	ImGui::NewLine();

	ImGui::ColorEdit4("Color", particleProps.Color.ptr());
	ImGui::ColorEdit4("End Color", particleProps.endColor.ptr());
	ImGui::NewLine();

	if (ImGui::CollapsingHeader("Emission: ", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Active", &EmissionControl);
		ImGui::InputInt("Particles per second ", &ParticlesPerSecond);
	}

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
			}
			else if (ImGui::MenuItem("Axis Align BillBoard"))
			{
				emitter.typeBB = BILLBOARDTYPE::AXISALIGN;
				currentBBoard = BBtype[2];
			}
			else if (ImGui::MenuItem("No Align BillBoard"))
			{
				emitter.typeBB = BILLBOARDTYPE::NO_ALIGN;
				currentBBoard = BBtype[3];
			}

			ImGui::End();
		}

		ImGui::TextColored(ImVec4(0, 1, 0, 1), currentBBoard.c_str());

		if (emitter.typeBB == BILLBOARDTYPE::AXISALIGN)
		{
			if (ImGui::BeginMenu("Change Axis"))
			{

				if (ImGui::MenuItem("X"))
				{
					emitter.alignAxis = AXISALIGNBB::X_AXIS;
					currentAxis = AxisAlign[0];
				}
				else if (ImGui::MenuItem("Y"))
				{
					emitter.alignAxis = AXISALIGNBB::Y_AXIS;
					currentAxis = AxisAlign[1];
				}
				else if (ImGui::MenuItem("Z"))
				{
					emitter.alignAxis = AXISALIGNBB::Z_AXIS;
					currentAxis = AxisAlign[2];
				}

				ImGui::End();
			}
			ImGui::TextColored(ImVec4(0, 1, 0, 1), "Current Axis: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0, 1, 0, 1), currentAxis.c_str());
		}
		
	}
	if (ImGui::CollapsingHeader("Texture: ", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Active Texture", &emitter.text);
		ImGui::Text("Loaded Texture: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), textureName.c_str());
		ImGui::Checkbox("Animated Texture", &emitter.animatedText);
		if(emitter.animatedText) {
			ImGui::InputInt("Atlas Columns ", &emitter.atlasColumns);
			ImGui::InputInt("Atlas Rows ", &emitter.atlasRows);
		}

		if (ImGui::Button("Clear Texture")) 
		{
			ClearTexture();
		}
	}
	ImGui::End();
}

void ModuleParticleSystem::LoadParticleTexture(std::string fileName)
{
	textureName = fileName.substr(fileName.find_last_of("\\") + 1);

	emitter.textID = App->texture->LoadTexture(fileName);
}

void ModuleParticleSystem::ClearTexture()
{
	textureName = defaultName;

	emitter.textID = App->texture->checkerID;
}