//-------------------------------------------------------------------------------------------------
#pragma once

#pragma warning (disable : 4996)

#include "SDL2\SDL.h"
#include "SDL2\SDL_image.h"
#include "GL\glew.h"
#include <vector>

namespace Triton
{
	// helper function for Texture class that obtains the pixel information in an SDL Surface,
	// at specified index in the pixel array
	Uint32 getPixel(SDL_Surface* surface, int x, int y);
	
	struct Texture
	{
		// array of pixel components for the texture
		std::vector<unsigned char> pixels;
		// handle of the texture in openGL
		GLuint handle;

		// default constructor
		Texture() { handle = NULL; }
		
		// fills the pixels array and assigns the handle for the texture
		void load(const char* texturePath);

		// binds this texture to used for coloring the current mesh in memory
		void bind();

		// unbinds this texture from memory
		void unbind();

		// destructor
		~Texture();
	};
}
	

