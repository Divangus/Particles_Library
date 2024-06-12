#pragma once
#include "Globals.h"

namespace Loader
{
	GLuint Shader(const char* vertexPath, const char* fragmentPath);

    Image LoadImage(const char* filename);

    void FreeImage(Image image);

    GLuint CreateTexture2DFromImage(Image image);

    u32 LoadTexture2D(const char* filepath);
}