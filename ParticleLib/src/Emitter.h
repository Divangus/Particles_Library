#pragma once
#include "ParticleGlobals.h"

#include <algorithm>

namespace Particles
{
	enum class BILLBOARDTYPE
	{
		NO_ALIGN,
		SCREENALIGN,
		WORLDALIGN,
		AXISALIGN,
	};

	enum class AXISALIGNBB
	{
		X_AXIS,
		Y_AXIS,
		Z_AXIS,
	};

	struct ParticleProps
	{
		//Particle initial position
		glm::vec3 pos = glm::vec3(0.0f);
		//Particle Rotation (only works if billboard is set to NO_ALIGN)
		glm::vec3 rot = glm::vec3(0.0f);
		//Particle Speed
		glm::vec3 speed = glm::vec3(0.0f);
		//Speed variation for each particle: set this variable to randomize a the speed of the particles so they don't follow all the same path
		glm::vec3 speedVariation = glm::vec3(0.0f);

		//Color of the particles
		//To use Color over lifetime, this property has to be vec4(0.0f) (set as default);
		glm::vec4 Color = glm::vec4(0.0f);

		//To use Color over lifetime, Particle Properties Color has to be vec4(0.0f, 0.0f, 0.0f, 1.0f) 
		glm::vec4 beginColor = glm::vec4(1.0f);
		//To use Color over lifetime, Particle Properties Color has to be vec4(0.0f, 0.0f, 0.0f, 1.0f)
		glm::vec4 endColor = glm::vec4(1.0f);

		//Scale of the particles
		//To use Scale over lifetime, this property has to be vec3(0.0f) (set as default);
		glm::vec3 Scale = glm::vec3(0.0f);

		//To use Scale over lifetime, Particle Properties Scale has to be vec3(0.0f)
		glm::vec3 beginScale = glm::vec3(1.0f);
		//To use Scale over lifetime, Particle Properties Scale has to be vec3(0.0f)
		glm::vec3 endScale = glm::vec3(1.0f);
		//Scale variation for each particle: set this variable to randomize a the scale of the particles so they don't have all the same size
		glm::vec3 scaleVariaton = glm::vec3(0.5f);

		//If the particles are aftected by gravity or not
		bool gravity = false;

		//The LifeTime of the particles (default set to 10)
		float LifeTime = 10.0f;

		//Particle initial position
		void SetPosition(float x, float y, float z);
		//Particle Speed
		void SetSpeed(float x, float y, float z);
		//Speed variation for each particle: set this variable to randomize a the speed of the particles so they don't follow all the same path
		void SetSpeedVariation(float x, float y, float z);

		//Particle Rotation (only works if billboard is set to NO_ALIGN)
		void SetRotation(float x, float y, float z);

		//Scale of the particles
		//To use Scale over lifetime, this property has to be vec3(0.0f) (set as default);
		void SetScale(float x, float y, float z);
		//To use Scale over lifetime, Particle Properties Scale has to be vec3(0.0f)
		void SetBeginScale(float x, float y, float z);
		//To use Scale over lifetime, Particle Properties Scale has to be vec3(0.0f)
		void SetEndScale(float x, float y, float z);
		//Scale variation for each particle: set this variable to randomize a the scale of the particles so they don't have all the same size
		void SetScaleVariation(float x, float y, float z);

		//Color of the particles
		//To use Color over lifetime, this property has to be vec4(0.0f) (set as default);
		void SetColor(float r, float g, float b, float a);
		//To use Color over lifetime, Particle Properties Color has to be vec4(0.0f, 0.0f, 0.0f, 1.0f) 
		void SetBeginColor(float r, float g, float b, float a);
		//To use Color over lifetime, Particle Properties Color has to be vec4(0.0f, 0.0f, 0.0f, 1.0f) 
		void SetEndColor(float r, float g, float b, float a);

		//If the particles are aftected by gravity or not
		void SetGravity(bool gravity);
		//The LifeTime of the particles (default set to 10)
		void SetLifeTime(bool LifeTime);
	};

	struct Particle
	{
		glm::vec3 pos, rot, scale;
		glm::mat4 transformMat = glm::mat4();
		glm::vec3 speed;
		glm::vec4 Color, endColor;

		glm::vec3 beginScale, endScale;

		float LifeTime = 1.0f, LifeRemaining = 1.0f;

		bool gravity = false;

		bool Active = false;

		int currentFrame = 0; // Current frame of the animation
		float totalFrames = 0; // Total frames in animation

		void SetTransformMatrix();
		void SetTransformMatrixWithQuat(glm::quat rotation);
		glm::mat4 GetTransformMatrix();
	};

	struct ParticleCamera
	{
		glm::vec3 cameraPos;
		glm::vec3 cameraFront;
		glm::vec3 cameraUp;

		glm::mat4 viewMatrix;

		float znear;
		float zfar;

		float aspectRatio;

		ParticleCamera();
		ParticleCamera(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp);
	
		void UpdateCamera(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp);
		glm::mat4 GetViewMatrix();
		glm::mat4 GetProjMatrix();
		void SetFrustrumPlanes(float znear, float zfar);
		void SetAspectRatio(float displaySizeX, float displaySizeY);
	};

	class Emitter
	{
	public:
		Emitter(std::string name);
		Emitter(std::string name, glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp);
		Emitter(std::string name, ParticleProps particleProperties);
		Emitter(std::string name, ParticleProps particleProperties, glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp);

		~Emitter();

		void Update(float dt);

		void Emit();

		void ParticleBuffer();
		void Render(GLuint shader);

		void ScreenAlignBBoard(Particle& particle);
		void WorldAlignBBoard(Particle& particle);
		void AxisAlignBBoard(Particle& particle);

		void SetParticlesPerSecond(bool active, int particlesPerSecond);

		void SetTexture(unsigned int textureID);
		void SetAnimatedTexture(unsigned int textureID, int atlasRows, int atlasColumns);
		void CleanTexture();

		//Camera
		void UpdateCamera(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp);
		void SetAspectRatio(float displaySizeX, float displaySizeY);

		void SetBillboardType(BILLBOARDTYPE typeBB);

		void SetAxisBillboardType(AXISALIGNBB alignAxis);

		void SetMaxParticles(int MaxParticles);

		ParticleProps ParticleProperties;

		bool EmissionControl = false;
		int ParticlesPerSecond = 1;
		float accumulator = 0.0f;

		std::string name;

	private:

		BILLBOARDTYPE typeBB = BILLBOARDTYPE::SCREENALIGN;
		AXISALIGNBB alignAxis = AXISALIGNBB::Y_AXIS;

		void ResizeParticleVector();

		unsigned int textID = 0;
		int MaxParticles = 10000;

		bool text = false;
		bool animatedText = false;

		int atlasColumns = 0, atlasRows = 0;

		ParticleCamera camera;

		std::vector<Particle> ParticleList;
		uint32_t currentParticle;

		GLuint vao = 0;
		GLuint vbo = 0;
		GLuint ebo = 0;
		GLuint instanceVBO = 0;
		GLuint instanceColorVBO = 0;
		GLuint instanceFrameVBO = 0;
	};

}

