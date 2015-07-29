//-------------------------------------------------------------------------------------------------
#include "SDL2\SDL.h"
#include "Display.h"
#include "Game.h"
#include "Utilities.h"

#include <string>

namespace Triton
{
	// wrapper class for all engine components, calls parse functions for game class object files
	// and loads game components in
	class Engine
	{
	public:
		//value checked by main loop
		bool Quit;
		//value is true if engine is running a game
		bool inGame;

		// pointer to current Game object that is running
		Game* currentGame;

		//constructor
		Engine();

		//function called by the main loop
		void update();

		//
		void loadGame(const char* GameFile);

		//
		void endGame();

		//destructor
		~Engine();

	private:
		// window wrapper class
		Display m_display;
		// event handler
		SDL_Event m_event;
	};
}
	