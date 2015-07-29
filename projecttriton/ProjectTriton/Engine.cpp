//-------------------------------------------------------------------------------------------------
#include "Engine.h"

using namespace Triton;
using namespace std;

Engine::Engine()
{
	Uint32 startTime = SDL_GetTicks();

	// for error checking later
	SDL_ClearError();
	
	Quit = false;
	inGame = false;
	currentGame = nullptr;
	
	// SDL_Init returns a negative number on Error and records it in SDL_GetError
	// for now we only need sdl for an fps counter, its opengl components, and event handling
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) < 0)
		throw SDL_GetError();

	// SDL must be initialized before this can be called
	m_display.init(640, 480, "");

	cout << "Start Up took " << SDL_GetTicks() - startTime << " milliseconds" << endl;
}

void Engine::update()
{
	if (SDL_PollEvent(&m_event))
	{
		// checking if exit button was pressed
		if (m_event.type == SDL_QUIT)
			Quit = true;
		if (m_event.type == SDL_KEYDOWN && (m_event.key.keysym.sym == SDLK_KP_ENTER || 
			m_event.key.keysym.sym == SDLK_RETURN) && !inGame)
		{
			string path;
			getline(cin, path);
			loadGame(path.c_str());
		}
		// put all event handling here
		if (inGame)
			currentGame->input(m_event);
	}

	if (inGame)
		currentGame->update();

	// swap buffers
	m_display.update();

	// clear stuff on the buffer you are about to draw on
	m_display.clear();
}

void Engine::loadGame(const char* GameFile)
{
	try{
		currentGame = parseGameFile(GameFile);
	}
	catch (exception e)
	{
		cerr << "error occurred while reading file '" << GameFile << "' " << e.what() << endl;
	}
	catch (const char* errorMessage)
	{
		cerr << "error occurred while reading file " << errorMessage << endl;
	}

	if (currentGame)
	{
		if (currentGame->windowWidth)
			m_display.setWindowSize(currentGame->windowWidth, currentGame->windowHeight);
		m_display.setWindowName(currentGame->title);
		if (currentGame->desktop_resolution)
			m_display.setFullScreen(SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_FULLSCREEN);
		else if (currentGame->fullscreen)
			m_display.setFullScreen(SDL_WINDOW_FULLSCREEN);
	}	
}

void Engine::endGame()
{
	currentGame->quit();
}

Engine::~Engine()
{
	if (currentGame != nullptr)
	{
		currentGame->quit();
		delete currentGame;
	}
		
	SDL_Quit();
}