#include "ParticleLib.h"
#include "Loader.h"

namespace Particles
{
	void Init()
	{
		shaderProgram = Loader::Shader("ParticleVShader.glsl", "ParticleFShader.glsl");
	}
}