#pragma once
#include "Module.h"
#include "Globals.h"
#include "Light.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_opengl3.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

	ImGuiIO* io = nullptr;

	Light lights[MAX_LIGHTS];
	SDL_GLContext context = nullptr;

	GLuint shaderProgram;
};