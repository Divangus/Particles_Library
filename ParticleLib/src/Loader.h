#pragma once
#include "ParticleGlobals.h"

namespace ParticleLoader
{
    struct ParticleImage
    {
        void* pixels;
        glm::ivec2 size;
        int   nchannels;
        int   stride;
    };

    unsigned int Shader(const char* vertexPath, const char* fragmentPath);

    ParticleImage LoadImage(const char* filename);

    void FreeImage(ParticleImage image);

    unsigned int CreateTexture2DFromImage(ParticleImage image);

    unsigned int LoadTexture2D(const char* filepath);
}