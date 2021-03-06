#include "Interface.hpp"

#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "Engine.hpp"
#include "Logger.hpp"
#include "Texture.hpp"
#include "Style.hpp"

using namespace ImGui;

Interface::Interface()
	: windows(nullptr), window(nullptr) {}

Interface::~Interface(){
	while(windows){
		Window *next = windows->next;
		delete windows;
		windows = next;
	}
}
bool Interface::Create(ResourceManager &resManager, SDL_Window *window,
		SDL_GLContext glcontext) {
	initialized = false;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
    ImGuiIO& io = GetIO();

    io.IniFilename = nullptr;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();

    if(!ImGui_ImplSDL2_InitForOpenGL(window, glcontext) ||
				!ImGui_ImplOpenGL3_Init(nullptr)){
		Log(LEVEL_ERROR, "Failed to initilize ImGui");
		return false;
	}

	resManager.LoadResource("ui/strings.json", &locale);

	Style style;
	if(resManager.LoadJSON("ui/style.json", style))
		ImGui::GetStyle() = style;
	initialized = true;
	return true;
}
void Interface::Destroy() {
	if(initialized) {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}
}
void Interface::Render(){
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window);
	ImGui::NewFrame();

	for(Window *window = windows; window; window = window->next)
		if(window->shown)
			window->Render(locale);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());
}
void Interface::AddWindow(Window *window){
	window->next = windows;
	windows = window;
}
void Interface::ProcessEvent(SDL_Event *event){
	 ImGui_ImplSDL2_ProcessEvent(event);
}
