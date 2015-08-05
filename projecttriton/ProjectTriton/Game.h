//-------------------------------------------------------------------------------------------------
#pragma once

#include "SceneLevel.h"

using namespace std;

namespace Triton
{
	// class that stores scene/SceneLevel files to be loaded, and 
	// object/entity types(like say a character or weapon) this game uses, 
	// game settings and directories
	class Game
	{
	public:
		// map of string ids for scenes
		//unordered_map<string, SceneLevel> sceneMap;
		// the current scene being displayed
		SceneLevel* currentScene;
		// Game's Directory
		string directory;
		// window size settings
		unsigned short windowHeight;
		unsigned short windowWidth;
		// fullscreen settings
		bool fullscreen;
		bool desktop_resolution;
		// game's window title
		string title;

		// list of scene ids and directories
		unordered_map<string, string> sceneDirectories;

		// constructor
		Game();

		// updates game variables
		void update();

		// handle events
		void input(SDL_Event &e);

		// ends the games, and what to call when a quit event happens, such as SDL_QUIT
		void quit();
		// destructor
		~Game(){ quit(); }
	private:

	};

}

