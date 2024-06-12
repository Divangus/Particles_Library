#pragma once
#include "Globals.h"
#include "Emitter.h"

namespace Particles
{
	void Init();

	GLuint shaderProgram;
	std::vector<Emitter> emittersList;
}
