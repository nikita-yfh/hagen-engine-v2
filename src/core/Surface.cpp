#include "Surface.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Logger.hpp"

Surface::Surface()
	:surface(nullptr) {}

bool Surface::Load(SDL_RWops *file){
	TextResource textRes;
	if(textRes.Load(file))
		return false;
	int width, height, channels;
	unsigned char *image = stbi_load_from_memory((unsigned char*)textRes.data,
		textRes.size, &width, &height, &channels, STBI_rgb_alpha);
	if(!image){
		Log(LEVEL_ERROR, stbi_failure_reason());
		return false;
	}
	surface = SDL_CreateRGBSurfaceWithFormatFrom(image, width, height,
		32, 4*width, SDL_PIXELFORMAT_RGBA32);
	if(!surface){
		Log(LEVEL_ERROR, SDL_GetError());
		return false;
	}
	return true;
}
Surface::~Surface(){
	if(surface != nullptr)
		SDL_FreeSurface(surface);
}
