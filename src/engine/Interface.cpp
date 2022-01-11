#include "Interface.hpp"

#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "Color.hpp"
#include "Engine.hpp"
#include "Logger.hpp"
#include "Texture.hpp"

using namespace ImGui;

Interface::Interface(ResourceManager &resManager,SDL_Window *window,SDL_GLContext glcontext)
		:windows(nullptr){
	IMGUI_CHECKVERSION();
    CreateContext();
    ImGuiIO& io = GetIO();

    io.IniFilename=nullptr;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    StyleColorsDark();

    if(!ImGui_ImplSDL2_InitForOpenGL(window, glcontext) ||
				!ImGui_ImplOpenGL3_Init(nullptr)){
		Log(LEVEL_ERROR,"Failed to initilize ImGui");
	}

	/*Style style;
	resManager.LoadJSON("style.json",&style);
	ImGui::GetStyle()=style;*/
}
Interface::~Interface(){
	Window *window=windows;
	while(window){
		Window *temp=window->next;
		delete window;
		window=temp;
	}
	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}
void Interface::Render(){
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window);
	ImGui::NewFrame();

	for(Window *window=windows;window;window=window->next)
		if(window->shown)
			window->Render();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());
}
void Interface::AddWindow(Window *window){
	window->next=windows;
	windows=window;
}
void Interface::ProcessEvent(SDL_Event *event){
	 ImGui_ImplSDL2_ProcessEvent(event);
}
void Window::Hide(){
	shown=false;
}
void Window::Show(){
	shown=true;
}
