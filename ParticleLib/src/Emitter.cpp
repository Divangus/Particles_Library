#include "Emitter.h"
#include "Random.h"

namespace Particles
{
	Emitter::Emitter(std::string name) {

		this->name = name;

		camera = ParticleCamera();

		ResizeParticleVector();
	}

	Emitter::Emitter(std::string name, glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp) {

		this->name = name;

		camera = ParticleCamera(cameraPos, cameraFront, cameraUp);

		ResizeParticleVector();
	}

	Emitter::Emitter(std::string name, ParticleProps particleProperties)
	{
		this->name = name;

		camera = ParticleCamera();

		ParticleProperties = particleProperties;

		ResizeParticleVector();
	}

	Emitter::Emitter(std::string name, ParticleProps particleProperties, glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp)
	{
		this->name = name;

		camera = ParticleCamera(cameraPos, cameraFront, cameraUp);

		ParticleProperties = particleProperties;

		ResizeParticleVector();

	}

	Emitter::~Emitter()
	{

	}

	void Emitter::ResizeParticleVector()
	{
		ParticleList.resize(MaxParticles);

		currentParticle = ParticleList.size() - 1;
	}

	void Emitter::SetMaxParticles(int MaxParticles)
	{
		this->MaxParticles = MaxParticles;
		ResizeParticleVector();
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
				ParticleList[i].speed += glm::vec3(0.0f, -9.81f, 0.0f) * dt * 0.5f;
			}
			ParticleList[i].pos += ParticleList[i].speed * dt;

			float life = ParticleList[i].LifeRemaining / ParticleList[i].LifeTime;
			ParticleList[i].scale = glm::mix(ParticleList[i].endScale, ParticleList[i].beginScale, life);
			ParticleList[i].SetTransformMatrix();

			if (animatedText)
			{
				ParticleList[i].totalFrames = atlasColumns * atlasRows;
				// Update animation frame based on life remaining
				ParticleList[i].currentFrame = glm::mix(0.0f, ParticleList[i].totalFrames, life);
			}

			switch (typeBB) {
			case BILLBOARDTYPE::NO_ALIGN:
				break;
			case BILLBOARDTYPE::SCREENALIGN:
				ParticleList[i].rot = glm::vec3(0.0f);
				ScreenAlignBBoard(ParticleList[i]);
				break;
			case BILLBOARDTYPE::WORLDALIGN:
				ParticleList[i].rot = glm::vec3(0.0f);
				WorldAlignBBoard(ParticleList[i]);
				break;
			case BILLBOARDTYPE::AXISALIGN:
				ParticleList[i].rot = glm::vec3(0.0f);
				AxisAlignBBoard(ParticleList[i]);
				break;
			default:
				break;
			}
		}
	}

	void Emitter::Emit()
	{
		if (currentParticle <= 0)
		{
			currentParticle = ParticleList.size() - 1;
		}

		Particle& particle = ParticleList[currentParticle];
		particle.Active = true;
		particle.pos = ParticleProperties.pos;
		particle.rot = ParticleProperties.rot;

		if (ParticleProperties.Scale != glm::vec3(0.0f))
		{
			particle.beginScale = ParticleProperties.Scale;
			particle.endScale = ParticleProperties.Scale;
		}
		else
		{
			particle.beginScale = ParticleProperties.beginScale + ParticleProperties.scaleVariaton * (Random::RandomFloat() - 0.5f); //Random number between -0.5 / 0.5
			particle.endScale = ParticleProperties.endScale;
		}

		//text = particleProps.texture;
		// Velocity
		particle.speed = ParticleProperties.speed;
		particle.speed.x += ParticleProperties.speedVariation.x * (Random::RandomFloat() - 0.5f);
		particle.speed.y += ParticleProperties.speedVariation.y * (Random::RandomFloat() - 0.5f);
		particle.speed.z += ParticleProperties.speedVariation.z * (Random::RandomFloat() - 0.5f);

		particle.gravity = ParticleProperties.gravity;

		// Color
		if (ParticleProperties.Color != glm::vec4(0.0f))
		{
			particle.Color = ParticleProperties.Color;
			particle.endColor = ParticleProperties.Color;
		}
		else
		{
			particle.Color = ParticleProperties.beginColor;
			particle.endColor = ParticleProperties.endColor;
		}

		particle.LifeTime = ParticleProperties.LifeTime;
		particle.LifeRemaining = ParticleProperties.LifeTime;

		currentParticle--;
	}

	void Emitter::ParticleBuffer()
	{
		unsigned int indices[]
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
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indices, GL_STATIC_DRAW);

		// Instance buffer
		glGenBuffers(1, &instanceVBO);
		glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
		glBufferData(GL_ARRAY_BUFFER, MaxParticles * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);

		// Set attribute pointers for matrix (4 times vec4)
		for (int i = 0; i < 4; i++) {
			glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * sizeof(glm::vec4)));
			glEnableVertexAttribArray(2 + i);
			glVertexAttribDivisor(2 + i, 1);
		}

		// Instance color buffer
		glGenBuffers(1, &instanceColorVBO);
		glBindBuffer(GL_ARRAY_BUFFER, instanceColorVBO);
		glBufferData(GL_ARRAY_BUFFER, MaxParticles * sizeof(glm::vec4), nullptr, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
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

		glm::vec3 cameraPosition = camera.cameraPos;

		std::vector<std::pair<float, Particle*>> distances;
		for (int i = 0; i < ParticleList.size(); i++) {
			if (ParticleList[i].Active) {
				float distance = glm::length2(ParticleList[i].pos - cameraPosition);
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

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &camera.GetViewMatrix()[0][0]);
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, &camera.GetProjMatrix()[0][0]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textID);
		glUniform1i(glGetUniformLocation(shader, "uTexture"), 0);
		glUniform1i(glGetUniformLocation(shader, "text"), text);
		glUniform1i(glGetUniformLocation(shader, "animatedText"), animatedText);

		glUniform2f(glGetUniformLocation(shader, "frameSize"), 1.0f / atlasColumns, 1.0f / atlasRows); // Pass frame size to shader

		std::vector<glm::mat4> instanceMatrices;
		std::vector<glm::vec4> instanceColors;
		std::vector<int> instanceFrames;

		for (const auto& pair : distances) {
			Particle* particle = pair.second;

			if (!particle->Active) continue;

			float life = particle->LifeRemaining / particle->LifeTime;
			glm::vec4 printColor = glm::mix(particle->endColor, particle->Color, life);

			instanceColors.push_back(printColor);
			instanceMatrices.push_back(particle->GetTransformMatrix());
			instanceFrames.push_back(particle->currentFrame);
		}

		glBindVertexArray(vao);

		// Update instance buffer
		glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, instanceMatrices.size() * sizeof(glm::mat4), instanceMatrices.data());

		// Update instance buffer for colors
		glBindBuffer(GL_ARRAY_BUFFER, instanceColorVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, instanceColors.size() * sizeof(glm::vec4), instanceColors.data());

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
		glm::vec3 activecamfront = camera.cameraFront;
		//Vector UP is the same as the cam
		glm::vec3 activecamup = camera.cameraUp;

		//Z-AXIS MUST BE INVERTED 
		glm::vec3 zAxisBB = -activecamfront;
		//Y-AXIS KEEPS THE SAME VALUE
		glm::vec3 yAxisBB = activecamup;

		//COMPUTE CROSS PRODUCT IN ORDER TO GET THE REMAINING AXIS
		glm::vec3 xAxisBB = glm::normalize(glm::cross(yAxisBB, zAxisBB));

		//Gather the axis into a 3x3 matrix
		glm::mat3 rotBB(
			xAxisBB.x, xAxisBB.y, xAxisBB.z,
			yAxisBB.x, yAxisBB.y, yAxisBB.z,
			zAxisBB.x, zAxisBB.y, zAxisBB.z
		);


		//Apply the rotation to the particle
		glm::quat q = glm::quat_cast(rotBB);
		particle.SetTransformMatrixWithQuat(q);
	}

	void Emitter::WorldAlignBBoard(Particle& particle)
	{
		//Vector from particle to cam
		glm::vec3 zAxisBB = glm::normalize(camera.cameraPos - particle.pos);

		//Vector UP is the same as the cam

		glm::vec3 yAxisBB = camera.cameraUp;

		//COMPUTE CROSS PRODUCT IN ORDER TO GET THE REMAINING AXIS

		glm::vec3 xAxisBB = glm::normalize(glm::cross(yAxisBB, zAxisBB));

		//COMPUTE Y AXIS AGAIN IN ORDER TO BE SURE THAT THE ANGLE BETWEEN Z AND Y IS 90 degrees

		yAxisBB = glm::normalize(glm::cross(zAxisBB, xAxisBB));

		//Gather the axis into a 3x3 matrix
		glm::mat3 rotBB(
			xAxisBB.x, xAxisBB.y, xAxisBB.z,
			yAxisBB.x, yAxisBB.y, yAxisBB.z,
			zAxisBB.x, zAxisBB.y, zAxisBB.z
		);

		//Apply the rotation to the particle
		glm::quat q = glm::quat_cast(rotBB);
		particle.SetTransformMatrixWithQuat(q);;
	}

	void Emitter::AxisAlignBBoard(Particle& particle)
	{
		//Vector from particle to cam
		glm::vec3 zAxisBB = glm::normalize(camera.cameraPos - particle.pos);

		glm::vec3 yAxisBB;

		//Define the arbitrary up vector (can be y-axis or any other vector)
		switch (alignAxis) {
		case AXISALIGNBB::X_AXIS:
			yAxisBB = glm::vec3(1.0f, 0.0f, 0.0f);
			break;
		case AXISALIGNBB::Y_AXIS:
			yAxisBB = glm::vec3(0.0f, 1.0f, 0.0f);
			break;
		case AXISALIGNBB::Z_AXIS:
			yAxisBB = glm::vec3(0.0f, 0.0f, 1.0f);
			break;
		default:
			yAxisBB = glm::vec3(0.0f, 1.0f, 0.0f);
			break;
		}

		// Calculate the right vector using cross product of up and look vectors
		glm::vec3 xAxisBB = glm::normalize(glm::cross(yAxisBB, zAxisBB));

		// Recalculate the up vector to ensure it is orthogonal
		glm::vec3 correctedYAxisBB = glm::normalize(glm::cross(zAxisBB, xAxisBB));

		// Check if the vectors are valid and not zero vectors (handle parallel case)
		if (glm::length(xAxisBB) < 0.0001f || glm::length(correctedYAxisBB) < 0.0001f) {
			// Omitir c�lculos adicionales en este caso raro
			return;
		}

		//Gather the axis into a 3x3 matrix
		glm::mat3 rotBB(
			xAxisBB.x, xAxisBB.y, xAxisBB.z,
			correctedYAxisBB.x, correctedYAxisBB.y, correctedYAxisBB.z,
			zAxisBB.x, zAxisBB.y, zAxisBB.z
		);

		//Apply the rotation to the particle
		glm::quat q = glm::quat_cast(rotBB);
		particle.SetTransformMatrixWithQuat(q);
	}

	void Emitter::SetParticlesPerSecond(bool active, const int particlesPerSecond)
	{
		EmissionControl = active;
		ParticlesPerSecond = particlesPerSecond;
	}

	void Emitter::SetBillboardType(BILLBOARDTYPE typeBB)
	{
		this->typeBB = typeBB;
	}

	void Emitter::SetAxisBillboardType(AXISALIGNBB alignAxis)
	{
		this->alignAxis = alignAxis;
	}

	void Emitter::SetTexture(unsigned int textureID)
	{
		textID = textureID;
		text = true;
		animatedText = false;
	}

	void Emitter::SetAnimatedTexture(unsigned int textureID, int atlasRows, int atlasColumns)
	{
		this->atlasRows = atlasRows;
		this->atlasColumns = atlasColumns;
		textID = textureID;
		text = true;
		animatedText = true;
	}

	void Emitter::CleanTexture()
	{
		textID = 0;
		text = false;
		animatedText = false;
	}

	void Particle::SetTransformMatrix()
	{
		float x = rot.x * DEGTORAD;
		float y = rot.y * DEGTORAD;
		float z = rot.z * DEGTORAD;

		glm::quat q = glm::quat(glm::vec3(x, y, z));

		glm::mat4 transform = glm::translate(pos);

		transform = transform * glm::mat4_cast(q);

		transform = glm::scale(transform, scale);

		transformMat = transform;
	}

	void Particle::SetTransformMatrixWithQuat(glm::quat rotation)
	{
		float x = rot.x * DEGTORAD;
		float y = rot.y * DEGTORAD;
		float z = rot.z * DEGTORAD;

		glm::quat q = rotation * glm::quat(glm::vec3(x, y, z));

		glm::mat4 transform = glm::translate(pos);

		transform = transform * glm::mat4_cast(q);

		transform = glm::scale(transform, scale);

		transformMat = transform;
	}

	glm::mat4 Particle::GetTransformMatrix()
	{
		return transformMat;
	}

	void Emitter::UpdateCamera(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp)
	{
		camera.UpdateCamera(cameraPos, cameraFront, cameraUp);
	}

	void Emitter::SetAspectRatio(float displaySizeX, float displaySizeY)
	{
		camera.SetAspectRatio(displaySizeX, displaySizeY);
	}

	ParticleCamera::ParticleCamera()
	{
		cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		znear = 0.1f;
		zfar = 1000.0f;
	}

	ParticleCamera::ParticleCamera(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp)
	{
		this->cameraPos = cameraPos;
		this->cameraFront = cameraFront;
		this->cameraUp = cameraUp;

		viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		znear = 0.1f;
		zfar = 1000.0f;
	}

	void ParticleCamera::UpdateCamera(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp)
	{
		this->cameraPos = cameraPos;
		this->cameraFront = cameraFront;
		this->cameraUp = cameraUp;

		viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}

	glm::mat4 ParticleCamera::GetViewMatrix()
	{
		return viewMatrix;
	}

	glm::mat4 ParticleCamera::GetProjMatrix()
	{
		glm::mat4 projection = glm::perspective(glm::radians(60.0f), aspectRatio, znear, zfar);
		return projection;
	}

	void ParticleCamera::SetFrustrumPlanes(float znear, float zfar)
	{
		this->znear = znear;
		this->zfar = zfar;
	}

	void ParticleCamera::SetAspectRatio(float displaySizeX, float displaySizeY)
	{
		aspectRatio = displaySizeX / displaySizeY;
	}

	void ParticleProps::SetPosition(float x, float y, float z)
	{
		pos = glm::vec3(x, y, z);
	}
	void ParticleProps::SetSpeed(float x, float y, float z)
	{
		speed = glm::vec3(x, y, z);
	}
	void ParticleProps::SetSpeedVariation(float x, float y, float z)
	{
		speedVariation = glm::vec3(x, y, z);
	}
	void ParticleProps::SetRotation(float x, float y, float z)
	{
		rot = glm::vec3(x, y, z);
	}
	void ParticleProps::SetScale(float x, float y, float z)
	{
		Scale = glm::vec3(x, y, z);
	}
	void ParticleProps::SetBeginScale(float x, float y, float z)
	{
		beginScale = glm::vec3(x, y, z);
	}
	void ParticleProps::SetEndScale(float x, float y, float z)
	{
		endScale = glm::vec3(x, y, z);
	}
	void ParticleProps::SetScaleVariation(float x, float y, float z)
	{
		scaleVariaton = glm::vec3(x, y, z);
	}

	void ParticleProps::SetColor(float r, float g, float b, float a)
	{
		Color = glm::vec4(r, g, b, a);
	}
	void ParticleProps::SetBeginColor(float r, float g, float b, float a)
	{
		beginColor = glm::vec4(r, g, b, a);
	}
	void ParticleProps::SetEndColor(float r, float g, float b, float a)
	{
		endColor = glm::vec4(r, g, b, a);
	}

	void ParticleProps::SetGravity(bool gravity)
	{
		this->gravity = gravity;
	}
	void ParticleProps::SetLifeTime(float LifeTime)
	{
		this->LifeTime = LifeTime;
	}

}
