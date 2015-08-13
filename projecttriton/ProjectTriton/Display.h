//-------------------------------------------------------------------------------------------------
#pragma once

#pragma warning(disable:4996)

#include "SDL2\SDL.h"
#include "GL\glew.h"
#include "glm\glm.hpp"

#include <string>

using namespace glm;

namespace Triton
{
	// class that manipulates and updates the window and openGL context
	class Display
	{
	public:
		//default constructor, to be used as a member of the engine class
		Display();
		
		//constructor parameters
		//screenWidth = width of the window display
		//screenHeight = height of the window display
		//windowName = title that shows up on the top bar of the window
		void init(unsigned int screenWidth, unsigned int screenHeight, const char* windowName);

		//swaps frame buffers each frame to update screen
		void update();

		//clears display, if need be called outside class
		void clear();

		// sets display to fullScreen, getters and setters here because of the way sdl handles its
		// window class
		// flags = {SDL_WINDOW_FULLSCREEN, SDL_WINDOW_FULLSCREEN_DESKTOP, 0}
		// SDL_WINDOW_FULLSCREEN is for true fullscreen video mode
		// SDL_WINDOW_FULLSCREEN_DESKTOP is for fullscreen at window resolution
		// 0 is for windowed mode
		void setFullScreen(Uint32 flag);

		// sets window size to specified parameters
		void setWindowSize(unsigned int width, unsigned int height);

		// sets parameters to the windows current width and height
		void getWindowSize(int * width, int * height);

		// sets the title that shows on the top bar of the window
		void setWindowName(const char* windowName);

		// sets clear color
		void setClearColor(float r, float g, float b, float a);

		//destructor, destroys window and openGL context
		~Display();

	private:
		// pointer to the window we're using
		SDL_Window* m_window;
		// the openGL context this program is using
		SDL_GLContext m_context;
	};
}
