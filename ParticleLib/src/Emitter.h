#pragma once
#include "ParticleGlobals.h"
#include "ParticleCamera.h"

#include <algorithm>

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
	//To use Scale over lifetime, Particle Properties Scale has to be vec3(0.0f)
	//Scale variation for each particle: set this variable to randomize a the scale of the particles so they don't have all the same size
	glm::vec3 scaleVariaton = glm::vec3(0.5f);

	//If the particles are aftected by gravity or not
	bool gravity = false;

	//The LifeTime of the particles (default set to 10)
	float LifeTime = 10.0f;
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
class Emitter
{
public:
	Emitter(std::string name);
	Emitter(std::string name, glm::mat4 ViewMatrix);
	Emitter(std::string name, ParticleProps particleProperties);
	Emitter(std::string name, ParticleProps particleProperties, glm::mat4 ViewMatrix);

	~Emitter();

	void Update(float dt);

	void Emit();

	void ParticleBuffer();
	void Render(GLuint shader);

	void ScreenAlignBBoard(Particle& particle);
	void WorldAlignBBoard(Particle& particle);
	void AxisAlignBBoard(Particle& particle);

	void SetTexture(unsigned int textureID);
	void SetAnimatedTexture(unsigned int textureID, int atlasRows, int atlasColumns);
	void CleanTexture();

	void SetAspectRatio(float displaySizeX, float displaySizeY);

	float lerp(float a, float b, float t);
	glm::vec3 lerp(const glm::vec3 a, const glm::vec3 b, float t);
	glm::vec4 lerp(const glm::vec4 a, const glm::vec4 b, float t);

	ParticleProps ParticleProperties;

	BILLBOARDTYPE typeBB = BILLBOARDTYPE::SCREENALIGN;
	AXISALIGNBB alignAxis = AXISALIGNBB::Y_AXIS;

	std::string name;

private:

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

