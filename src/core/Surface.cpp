#include "Surface.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Logger.hpp"

Surface::Surface()
	:surface(nullptr) {}

int Surface::Load(SDL_RWops *file){
	TextResource textRes;
	if(textRes.Load(file))
		return 1;
	int width,height,channels;
	unsigned char *image=stbi_load_from_memory((unsigned char*)textRes.data,textRes.size,&width,&height,&channels,STBI_rgb_alpha);
	if(!image){
		Log(LEVEL_ERROR,stbi_failure_reason());
		return 1;
	}
	surface=SDL_CreateRGBSurfaceWithFormatFrom(image,width,height,32,4*width,SDL_PIXELFORMAT_RGBA32);
	if(!surface){
		Log(LEVEL_ERROR,SDL_GetError());
		return 1;
	}
	return 0;
}
int Surface::Save(SDL_RWops *file) const{
	return 1;
}
Surface::~Surface(){
	if(surface!=nullptr)
		SDL_FreeSurface(surface);
}
