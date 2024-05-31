#include "Emitter.h"
#include "Application.h"
#include "Random.h"
#include "MemLeaks.h"

#include <iostream>

Emitter::Emitter() {

	ParticleList.resize(MaxParticles);

	currentParticle = ParticleList.size() - 1;
}

Emitter::~Emitter()
{

}

void Emitter::Update(float dt) {

	for (int i = 0; i < ParticleList.size(); i++)
	{
		if (!ParticleList[i].Active)
			continue;

		if (ParticleList[i].LifeRemaining <= 0.0f)
		{
			ParticleList[i].Active = false;
			continue;
		}

		ParticleList[i].LifeRemaining -= dt;
		if (ParticleList[i].gravity) {
			ParticleList[i].speed += float3(0.0f, -9.81f, 0.0f) * dt * 0.5f;
		}		
		ParticleList[i].pos += ParticleList[i].speed * dt;

		float life = ParticleList[i].LifeRemaining / ParticleList[i].LifeTime;
		ParticleList[i].scale = Lerp(ParticleList[i].endScale, ParticleList[i].beginScale, life);
		ParticleList[i].SetTransformMatrix();

		if (animatedText)
		{
			ParticleList[i].totalFrames = atlasColumns * atlasRows;
			// Update animation frame based on life remaining
			ParticleList[i].currentFrame = Lerp(0, ParticleList[i].totalFrames, life);;
		}		

		switch (typeBB) {
		case BILLBOARDTYPE::NO_ALIGN:
			break;
		case BILLBOARDTYPE::SCREENALIGN:
			ScreenAlignBBoard(ParticleList[i]);
			break;
		case BILLBOARDTYPE::WORLDALIGN:
			WorldAlignBBoard(ParticleList[i]);
			break;
		case BILLBOARDTYPE::AXISALIGN:
			AxisAlignBBoard(ParticleList[i]);
			break;
		default:
			break;
		}
	}
}

void Emitter::Emit(ParticleProps& particleProps)
{
	if (currentParticle <= 0)
	{
		currentParticle = ParticleList.size() - 1;
	}
	
	Particle& particle = ParticleList[currentParticle];
	particle.Active = true;
	particle.pos = particleProps.pos;
	particle.beginScale = particleProps.beginScale + particleProps.scaleVariaton * (Random::RandomFloat() - 0.5f); //Random number between -0.5 / 0.5
	particle.endScale = particleProps.endScale;

	//text = particleProps.texture;
	// Velocity
	particle.speed = particleProps.speed;
	particle.speed.x += particleProps.speedVariation.x * (Random::RandomFloat() - 0.5f);
	particle.speed.y += particleProps.speedVariation.y * (Random::RandomFloat() - 0.5f);
	particle.speed.z += particleProps.speedVariation.z * (Random::RandomFloat() - 0.5f);

	particle.gravity = particleProps.gravity;

	// Color
	particle.Color = particleProps.Color;
	particle.endColor = particleProps.endColor;

	particle.LifeTime = particleProps.LifeTime;
	particle.LifeRemaining = particleProps.LifeTime;

	currentParticle--;
}

void Emitter::ParticleBuffer()
{
	uint indices[]
	{
		0, 1, 2,
		2, 3, 0,
	};

	float vertices[]
	{
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, //0
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, //1
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, //2
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, //3

	};

	// Generar y configurar el VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 5, vertices, GL_STATIC_DRAW);

	//Vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//texture Coords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 6, indices, GL_STATIC_DRAW);

	// Instance buffer
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * sizeof(float4x4), nullptr, GL_DYNAMIC_DRAW);

	// Set attribute pointers for matrix (4 times vec4)
	for (int i = 0; i < 4; i++) {
		glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, sizeof(float4x4), (void*)(i * sizeof(vec4)));
		glEnableVertexAttribArray(2 + i);
		glVertexAttribDivisor(2 + i, 1);
	}

	// Instance color buffer
	glGenBuffers(1, &instanceColorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceColorVBO);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * sizeof(float4), nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(float4), (void*)0);
	glVertexAttribDivisor(6, 1);

	// Instance frame buffer
	glGenBuffers(1, &instanceFrameVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceFrameVBO);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * sizeof(int), nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(7);
	glVertexAttribIPointer(7, 1, GL_INT, sizeof(int), (void*)0);
	glVertexAttribDivisor(7, 1);

	glBindVertexArray(0);

}

void Emitter::Render(GLuint shader) {

	float3 cameraPosition = Application::GetApp()->camera->FrustumCam.pos;

	std::vector<std::pair<float, Particle*>> distances;
	for (int i = 0; i < ParticleList.size(); i++) {
		if (ParticleList[i].Active) {
			float distance = (ParticleList[i].pos - cameraPosition).LengthSq();
			distances.push_back(std::make_pair(distance, &ParticleList[i]));
		}
	}

	std::sort(distances.begin(), distances.end(), [](const auto& lhs, const auto& rhs) {
		return lhs.first > rhs.first;
		});

	glUseProgram(shader);
	glDepthMask(GL_FALSE);

	GLuint viewLoc = glGetUniformLocation(shader, "viewMatrix");
	GLuint projLoc = glGetUniformLocation(shader, "projectionMatrix");

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, Application::GetApp()->camera->GetViewMatrix());
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, Application::GetApp()->camera->GetProjectionMatrix());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textID);
	glUniform1i(glGetUniformLocation(shader, "uTexture"), 0);
	glUniform1i(glGetUniformLocation(shader, "text"), text);
	glUniform1i(glGetUniformLocation(shader, "animatedText"), animatedText);

	glUniform2f(glGetUniformLocation(shader, "frameSize"), 1.0f / atlasColumns, 1.0f / atlasRows); // Pass frame size to shader

	std::vector<float4x4> instanceMatrices;
	std::vector<float4> instanceColors;
	std::vector<int> instanceFrames;

	for (const auto& pair : distances) {
		Particle* particle = pair.second;

		if (!particle->Active) continue;

		float life = particle->LifeRemaining / particle->LifeTime;
		float4 printColor = Lerp(particle->endColor, particle->Color, life);

		instanceColors.push_back(printColor);
		instanceMatrices.push_back(particle->GetTransformMatrix());
		instanceFrames.push_back(particle->currentFrame);
	}

	glBindVertexArray(vao);

	// Update instance buffer
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, instanceMatrices.size() * sizeof(float4x4), instanceMatrices.data());

	// Update instance buffer for colors
	glBindBuffer(GL_ARRAY_BUFFER, instanceColorVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, instanceColors.size() * sizeof(float4), instanceColors.data());

	// Update instance buffer for frames
	glBindBuffer(GL_ARRAY_BUFFER, instanceFrameVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, instanceFrames.size() * sizeof(int), instanceFrames.data());

	// Render instanced
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, instanceMatrices.size());

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDepthMask(GL_TRUE);
	glUseProgram(0);
}

void Emitter::ScreenAlignBBoard(Particle& particle)
{
	//GET INFO ABOUT CAM AXIS
	float3 activecamfront = Application::GetApp()->camera->FrustumCam.front;
	//Vector UP is the same as the cam
	float3 activecamup = Application::GetApp()->camera->FrustumCam.up;

	//Z-AXIS MUST BE INVERTED 
	float3 zAxisBB = -activecamfront;
	//Y-AXIS KEEPS THE SAME VALUE
	float3 yAxisBB = activecamup;

	//COMPUTE CROSS PRODUCT IN ORDER TO GET THE REMAINING AXIS
	float3 xAxisBB = yAxisBB.Cross(zAxisBB).Normalized();

	//Gather the axis into a 3x3 matrix
	float3x3 rotBB;
	rotBB.Set(xAxisBB.x, xAxisBB.y, xAxisBB.z, yAxisBB.x, yAxisBB.y, yAxisBB.z, zAxisBB.x, zAxisBB.y, zAxisBB.z);

	//Apply the rotation to the particle
	Quat q = rotBB.Inverted().ToQuat();
	particle.SetTransformMatrixWithQuat(q);
}

void Emitter::WorldAlignBBoard(Particle& particle)
{
	//Vector from particle to cam
	float3 zAxisBB = (Application::GetApp()->camera->FrustumCam.pos - particle.pos).Normalized();

	//Vector UP is the same as the cam

	float3 yAxisBB = Application::GetApp()->camera->FrustumCam.up;

	//COMPUTE CROSS PRODUCT IN ORDER TO GET THE REMAINING AXIS

	float3 xAxisBB = yAxisBB.Cross(zAxisBB).Normalized();

	//COMPUTE Y AXIS AGAIN IN ORDER TO BE SURE THAT THE ANGLE BETWEEN Z AND Y IS 90 degrees

	yAxisBB = zAxisBB.Cross(xAxisBB).Normalized();

	//Gather the axis into a 3x3 matrix
	float3x3 rotBB;
	rotBB.Set(xAxisBB.x, xAxisBB.y, xAxisBB.z, yAxisBB.x, yAxisBB.y, yAxisBB.z, zAxisBB.x, zAxisBB.y, zAxisBB.z);

	//Apply the rotation to the particle
	Quat q = rotBB.Inverted().ToQuat();
	particle.SetTransformMatrixWithQuat(q);;
}

void Emitter::AxisAlignBBoard(Particle& particle)
{
	//Vector from particle to cam
	float3 zAxisBB = (Application::GetApp()->camera->FrustumCam.pos - particle.pos).Normalized();

	float3 yAxisBB = float3::zero;

	//Define the arbitrary up vector (can be y-axis or any other vector)
	switch (alignAxis) {
	case AXISALIGNBB::X_AXIS:
		yAxisBB = float3(1.0f, 0.0f, 0.0f);
		break;
	case AXISALIGNBB::Y_AXIS:
		yAxisBB = float3(0.0f, 1.0f, 0.0f);
		break;
	case AXISALIGNBB::Z_AXIS:
		yAxisBB = float3(0.0f, 0.0f, 1.0f);
		break;
	default:
		yAxisBB = float3(0.0f, 1.0f, 0.0f);
		break;
	}

	// Calculate the right vector using cross product of up and look vectors
	float3 xAxisBB = yAxisBB.Cross(zAxisBB).Normalized();

	// Recalculate the up vector to ensure it is orthogonal
	float3 correctedYAxisBB = zAxisBB.Cross(xAxisBB).Normalized();

	// Check if the vectors are valid and not zero vectors (handle parallel case)
	if (xAxisBB.Length() < 0.0001f || correctedYAxisBB.Length() < 0.0001f) {
		//skip further calculation in this rare case
		return;
	}

	//Gather the axis into a 3x3 matrix
	float3x3 rotBB = float3x3::identity;
	rotBB.Set(xAxisBB.x, xAxisBB.y, xAxisBB.z, correctedYAxisBB.x, correctedYAxisBB.y, correctedYAxisBB.z, zAxisBB.x, zAxisBB.y, zAxisBB.z);

	//Apply the rotation to the particle
	Quat q = rotBB.Inverted().ToQuat();
	particle.SetTransformMatrixWithQuat(q);
}

void Particle::SetTransformMatrix()
{
	float x = rot.x * DEGTORAD;
	float y = rot.y * DEGTORAD;
	float z = rot.z * DEGTORAD;

	Quat q = Quat::FromEulerXYZ(x, y, z);

	transformMat = float4x4::FromTRS(pos, q, scale).Transposed();
}

void Particle::SetTransformMatrixWithQuat(Quat rotation)
{
	float x = rot.x * DEGTORAD;
	float y = rot.y * DEGTORAD;
	float z = rot.z * DEGTORAD;

	Quat q = rotation * Quat::FromEulerXYZ(x, y, z);

	transformMat = float4x4::FromTRS(pos, q, scale).Transposed();
}

void Particle::SetTransform(float4x4 matrix)
{
	Quat rotation;
	matrix.Decompose(pos, rotation, scale);

	rot = rotation.ToEulerXYZ();
}

float4x4 Particle::GetTransformMatrix()
{
	return transformMat;
}

