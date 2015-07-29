//-------------------------------------------------------------------------------------------------
#pragma once

#pragma warning (disable : 4996)

#include "SDL2\SDL.h"
#include "SDL2\SDL_image.h"
#include "GL\glew.h"
#include <string>

namespace Triton
{
	// helper function for Texture class that obtains the pixel information in an SDL Surface,
	// at specified index in the pixel array
	Uint32 getPixel(SDL_Surface* surface, int i);
	
	struct Texture
	{
		// array of pixel components for the texture
		unsigned char* pixels;
		bool error;
		char* errorMessage;
		// handle of the texture in openGL
		GLuint handle;

		// cosntructor: fills the pixels array and assigns the handle for the texture
		Texture(const char* texturePath);

		// binds this texture to used for coloring the current mesh in memory
		void bind();

		// unbinds this texture from memory
		void unbind();

		// destructor
		~Texture();
	};
}
	

