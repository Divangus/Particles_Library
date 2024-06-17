#pragma once
#include "Globals.h"
#include "Camera.h"

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
	vec3 pos = vec3(0.0f);
	//Particle Speed
	vec3 speed = vec3(0.0f); 
	//Speed variation for each particle: set this variable to randomize a the speed of the particles so they don't follow all the same path
	vec3 speedVariation = vec3(0.0f);

	//Color of the particles
	//To use Color over lifetime, this property has to be vec4(0.0f) (set as default);
	vec4 Color = vec4(0.0f);

	//To use Color over lifetime, Particle Properties Color has to be vec4(0.0f, 0.0f, 0.0f, 1.0f) 
	vec4 beginColor = vec4(1.0f);
	//To use Color over lifetime, Particle Properties Color has to be vec4(0.0f, 0.0f, 0.0f, 1.0f)
	vec4 endColor = vec4(1.0f);

	//Scale of the particles
	//To use Scale over lifetime, this property has to be vec3(0.0f) (set as default);
	vec3 Scale = vec3(0.0f);

	//To use Scale over lifetime, Particle Properties Scale has to be vec3(0.0f)
	vec3 beginScale = vec3(1.0f);
	//To use Scale over lifetime, Particle Properties Scale has to be vec3(0.0f)
	vec3 endScale = vec3(1.0f);
	//To use Scale over lifetime, Particle Properties Scale has to be vec3(0.0f)
	//Scale variation for each particle: set this variable to randomize a the scale of the particles so they don't have all the same size
	vec3 scaleVariaton = vec3(0.5f);

	//If the particles are aftected by gravity or not
	bool gravity = false;

	//The LifeTime of the particles (default set to 10)
	float LifeTime = 10.0f;
};

struct Particle
{
	vec3 pos, rot, scale;
	mat4 transformMat = mat4();
	vec3 speed;
	vec4 Color, endColor;

	vec3 beginScale, endScale;

	float LifeTime = 1.0f, LifeRemaining = 1.0f;

	bool gravity = false;

	bool Active = false;

	int currentFrame = 0; // Current frame of the animation
	float totalFrames = 0; // Total frames in animation

	void SetTransformMatrix();
	void SetTransformMatrixWithQuat(quat rotation);
	mat4 GetTransformMatrix();
};
class Emitter
{
	friend class Particles;

public:
	Emitter(std::string name);
	Emitter(std::string name, mat4 ViewMatrix);
	Emitter(std::string name, ParticleProps particleProperties);
	Emitter(std::string name, ParticleProps particleProperties, mat4 ViewMatrix);

	~Emitter();

	void Update(float dt);

	void Emit();

	void ParticleBuffer();
	void Render(GLuint shader);

	void ScreenAlignBBoard(Particle& particle);
	void WorldAlignBBoard(Particle& particle);
	void AxisAlignBBoard(Particle& particle);

	ParticleProps ParticleProperties;

	BILLBOARDTYPE typeBB = BILLBOARDTYPE::SCREENALIGN;
	AXISALIGNBB alignAxis = AXISALIGNBB::Y_AXIS;

	std::string name;

private:

	u32 textID = 0;
	int MaxParticles = 10000;

	bool text = false;
	bool animatedText = false;

	int atlasColumns = 0, atlasRows = 0;

	Camera camera;

	std::vector<Particle> ParticleList;
	uint32_t currentParticle = MaxParticles - 1;

	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ebo = 0;
	GLuint instanceVBO = 0;
	GLuint instanceColorVBO = 0;
	GLuint instanceFrameVBO = 0;
};

