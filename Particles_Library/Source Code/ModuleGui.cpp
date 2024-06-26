#include "Application.h"
#include "ModuleGui.h"
#include "ModuleWindow.h"
#include "MemLeaks.h"

#include "ModuleRenderer3D.h"

ModuleGui::ModuleGui(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

ModuleGui::~ModuleGui()
{}

// -----------------------------------------------------------------
bool ModuleGui::Start()
{
	LOG("Setting up the UI");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleGui::CleanUp()
{
	LOG("Cleaning UI");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleGui::PreUpdate(float dt)
{


	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
update_status ModuleGui::Update(float dt)
{

	if (ImGui::BeginMainMenuBar()) { //Bar--------------------------
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Quit ESC")) {
				ImGui::EndMenu();
				return UPDATE_STOP;
				
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View")) {
			ImGui::MenuItem("Configuration", NULL, &config);
			ImGui::MenuItem("Particle Editor", NULL, &App->particleSystem->particleMenu);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help")) {
			ImGui::MenuItem("About", NULL, &about);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	if (about) { //------------------------------------------------------- ABOUT WINDOW --------------
		if (ImGui::Begin("About")) {
			ImGui::Text("Particle Library ~ ~");
			ImGui::Separator();
			ImGui::NewLine();
			ImGui::Text("A particle library made with OpenGL ");
			ImGui::Text("By David Benages");
			ImGui::NewLine();
			ImGui::Text("3rd Partie Libraries:");
			ImGui::BulletText("SDL 2.0");
			ImGui::BulletText("ImGui 1.88");
			ImGui::BulletText("Glew 2.1.0");
			ImGui::BulletText("OpenGL 3.1");
			ImGui::BulletText("PhysFS");
			ImGui::NewLine();
			ImGui::Text("License:");
			ImGui::NewLine();
			PrintLicense();
		}
		ImGui::End();
	}
	if (config) { //------------------------------------------------------- CONFIG WINDOW -------------
		if (ImGui::Begin("Configuration")) {
			if (ImGui::CollapsingHeader("Application")) {
				static char string[128] = "Particle Library ~ ~";
				ImGui::InputText("Engine Name", string, IM_ARRAYSIZE(string));
				static char string2[128] = "David Benages";
				ImGui::InputText("Author", string2, IM_ARRAYSIZE(string2));
			}
			if (ImGui::CollapsingHeader("Window")) {
				if (ImGui::Checkbox("Full Screen\t", &App->window->fullScreen)) {
					App->window->resizable = false; App->window->borderless = false; App->window->desktop = false;
					App->window->SwitchScreen();
				}
				ImGui::SameLine();
				if (ImGui::Checkbox("Resizable", &App->window->resizable)) {
					App->window->fullScreen = false; App->window->borderless = false; App->window->desktop = false;
					App->window->SwitchScreen();
				}
				if (ImGui::Checkbox("Borderless \t", &App->window->borderless)) {
					App->window->fullScreen = false; App->window->resizable = false; App->window->desktop = false;
					App->window->SwitchScreen();
				}
				ImGui::SameLine();
				if (ImGui::Checkbox("Full Desktop", &App->window->desktop)) {
					App->window->fullScreen = false; App->window->resizable = false; App->window->borderless = false;
					App->window->SwitchScreen();
				}
				//ImGui::SliderInt("Brightness", );
				ImGui::SliderInt("Width", &width, 640, 1920);
				ImGui::SliderInt("Height", &height, 480, 1080);
				SDL_SetWindowSize(App->window->window, width, height);
			}
			if (ImGui::CollapsingHeader("File System")) {
				/*if(ImGui::Checkbox("Active\t")){}*/
				char* base_path = SDL_GetBasePath();
				ImGui::Text("Base Path:"); ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", base_path);
				ImGui::Text("Read Path:");

				ImGui::Text("Write Path:");
			}
			if (ImGui::CollapsingHeader("Input")) {
				ImGuiIO& io = ImGui::GetIO();
				int count = IM_ARRAYSIZE(io.MouseDown);
				ImGui::Text("Mouse Position:"); ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%g, %g", io.MousePos.x, io.MousePos.y);
				ImGui::Text("Mouse Motion:"); ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%g, %g", io.MouseDelta.x, io.MouseDelta.y);
				ImGui::Text("Mouse Wheel:");
				ImGui::Text("Mouse down:");         for (int i = 0; i < count; i++) if (ImGui::IsMouseDown(i)) { ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f),"b%d (%.02f secs)", i, io.MouseDownDuration[i]); }

			}
			if (ImGui::CollapsingHeader("Hardware")) {
				
				ImGui::Text("RAM: %d", SDL_GetSystemRAM);
				ImGui::Text("CPU: %d", SDL_GetCPUCount);


			}
		}
		ImGui::End();
	}
	
	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
update_status ModuleGui::PostUpdate(float dt)
{
	

	return UPDATE_CONTINUE;
}

void ModuleGui::PrintLicense() {

	ImGui::Text("MIT License");
	ImGui::NewLine();
	ImGui::Text("Copyright(c) 2024 & David Benages");
	ImGui::NewLine();
	ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy");
	ImGui::Text("of this softwareand associated documentation files(the 'Software'), to deal");
	ImGui::Text("in the Software without restriction, including without limitation the rights");
	ImGui::Text("to use, copy, modify, merge, publish, distribute, sublicense, and /or sell");
	ImGui::Text("copies of the Software, and to permit persons to whom the Software is");
	ImGui::Text("furnished to do so, subject to the following conditions :");
	ImGui::NewLine();
	ImGui::Text("The above copyright noticeand this permission notice shall be included in all");
	ImGui::Text("copies or substantial portions of the Software.");
	ImGui::NewLine();
	ImGui::Text("THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR");
	ImGui::Text("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,");
	ImGui::Text("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE");
	ImGui::Text("AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER");
	ImGui::Text("LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,");
	ImGui::Text("OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE");
	ImGui::Text("SOFTWARE.");
}