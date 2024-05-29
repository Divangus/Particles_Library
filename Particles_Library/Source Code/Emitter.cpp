#include "Emitter.h"
#include "Application.h"
#include "Random.h"
#include "MemLeaks.h"

#include <iostream>

Emitter::Emitter() {

	ParticleList.resize(1000);

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
		ParticleList[i].pos += ParticleList[i].speed * dt;
		ParticleList[i].SetTransformMatrix();
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

	//Fill buffers with vertices
	glEnableClientState(GL_VERTEX_ARRAY);
	glGenBuffers(1, (GLuint*)&(id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * VERTICES, vertices, GL_STATIC_DRAW);

	//Fill buffers with indices
	glGenBuffers(1, (GLuint*)&(id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 6, indices, GL_STATIC_DRAW);

	glDisableClientState(GL_VERTEX_ARRAY);

}

void Emitter::Render() {

	//Vertices
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);

	glVertexPointer(3, GL_FLOAT, sizeof(float) * VERTICES, NULL);
	glTexCoordPointer(2, GL_FLOAT, sizeof(float) * VERTICES, (void*)(sizeof(float) * 3));
	//bind and use other buffers

	//Indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);

	for (int i = 0; i < ParticleList.size(); i++)
	{
		if (!ParticleList[i].Active)
			continue;


		float life = ParticleList[i].LifeRemaining / ParticleList[i].LifeTime;
		ParticleList[i].scale = Lerp(ParticleList[i].endScale, ParticleList[i].beginScale, life);
		float4 printColor = Lerp(ParticleList[i].endColor, ParticleList[i].Color, life);

		ParticleList[i].SetTransformMatrix();

		//ScreenAlignBBoard(ParticleList[i]);
		WorldAlignBBoard(ParticleList[i]);
		//AxisAlignBBoard(ParticleList[i]);

		glColor4f(printColor.x, printColor.y, printColor.z, printColor.w);

		glPushMatrix();

		glMultMatrixf(ParticleList[i].GetTransformMatrix().ptr());

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		glPopMatrix();
	}

	glDisableClientState(GL_VERTEX_ARRAY);

	//cleaning texture
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_COORD_ARRAY);

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
	particle.transformMat = float4x4::FromTRS(particle.pos, q, particle.scale).Transposed();
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
	particle.transformMat = float4x4::FromTRS(particle.pos, q, particle.scale).Transposed();
}

void Emitter::AxisAlignBBoard(Particle& particle)
{
	//float3 zAxisBB = (Application::GetApp()->camera->FrustumCam.pos - particle.pos).Normalized();
	//float3 xAxisBB = { 1.0f,0.0f,0.0f }; // this is always the direction the particle is initially created in.

	//float3x3 rotBB;

	//if (zAxisBB.Dot(xAxisBB) != 1.0f)
	//{
	//	float3 yAxisBB = zAxisBB.Cross(xAxisBB).Normalized();

	//	float angle = zAxisBB.AngleBetween(xAxisBB);

	//	angle = angle * DEGTORAD;
	//	rotBB.RotateAxisAngle(yAxisBB, angle);
	//}
	//else
	//{
	//	float3 yAxisBB = { 0.01f, 1, 0.01f };

	//	rotBB.RotateAxisAngle(yAxisBB, 0.0f);
	//}
	// 
	//Vector from particle to cam
	float3 zAxisBB = (Application::GetApp()->camera->FrustumCam.pos - particle.pos).Normalized();

	//X and Y axis alligned to world
	float3 yAxisBB = { 0.0f,1.0f,0.0f };

	float3 xAxisBB = { 1.0f,0.0f,0.0f };

	//Gather the axis into a 3x3 matrix
	float3x3 rotBB = float3x3::identity;
	rotBB.Set(xAxisBB.x, xAxisBB.y, xAxisBB.z, yAxisBB.x, yAxisBB.y, yAxisBB.z, zAxisBB.x, zAxisBB.y, zAxisBB.z);

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

