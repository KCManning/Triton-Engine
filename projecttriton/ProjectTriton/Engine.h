//-------------------------------------------------------------------------------------------------
#include "Display.h"
#include "Utilities.h"

namespace Triton
{
	// wrapper class for all engine components, calls parse functions for game class object files
	// and loads game components in
	class Engine
	{
	public:
		//value checked by main loop
		bool Quit;

		// pointer to current Game object that is running, only one game can run at a time
		Game* currentGame;

		//constructor
		Engine();

		//function called by the main loop
		void update();

		// loads game from file
		void loadGame(const char* GameFile);

		// exits out of game, without deleting it from memory
		void endGame();

		//destructor
		~Engine();

	private:
		// window wrapper class
		Display m_display;
		// event handler
		SDL_Event m_event;
		// value is true if engine is running a game
		bool inGame;
	};
}
	