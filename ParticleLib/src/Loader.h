#pragma once
#include "ParticleGlobals.h"

namespace Loader
{
    unsigned int Shader(const char* vertexPath, const char* fragmentPath);

    ParticleImage LoadImage(const char* filename);

    void FreeImage(ParticleImage image);

    unsigned int CreateTexture2DFromImage(ParticleImage image);

    unsigned int LoadTexture2D(const char* filepath);
}