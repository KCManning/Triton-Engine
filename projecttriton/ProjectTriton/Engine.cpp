//-------------------------------------------------------------------------------------------------
#include "Engine.h"

using namespace Triton;
using namespace std;

Engine::Engine() : Quit(false), inGame(false), currentGame(nullptr)
{
	Uint32 startTime = SDL_GetTicks();

	// for error checking later
	SDL_ClearError();
	
	Quit = false;
	inGame = false;
	
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
	Uint32 frameStartTime = SDL_GetTicks();
	
	while (SDL_PollEvent(&m_event))
	{
		// checking if exit button was pressed
		if (m_event.type == SDL_QUIT)
			Quit = true;

		// put all event handling here
		if (inGame)
			currentGame->input(m_event);
	}

	if (inGame)
		currentGame->update();

	// swap buffers
	m_display.update();

	Uint32 frameTime = SDL_GetTicks() - frameStartTime;
	
	if (float(frameTime) < 1000.f / 60)
	{
		SDL_Delay(Uint32(1000.f / 60 - frameTime));
	}
	
	// clear stuff on the buffer you are about to draw on
	m_display.clear();
}

void Engine::loadGame(const char* GameFile)
{
	try{
		Uint32 startOfLoadTime = SDL_GetTicks();
		currentGame = new Game;
		parse(GameFile, currentGame);
		cout << "Game took " << SDL_GetTicks() - startOfLoadTime << " milliseconds to load" 
			<< endl;
		inGame = true;
	}
	catch (const char* errorMessage)
	{
		cerr << "error occurred while loading game: " << errorMessage << endl;
		delete currentGame;
	}

	// sets game window parameters
	if (inGame)
	{
		if (currentGame->windowWidth)
			m_display.setWindowSize(currentGame->windowWidth, currentGame->windowHeight);
		m_display.setWindowName(currentGame->title.c_str());
		if (currentGame->desktop_resolution)
			m_display.setFullScreen(SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_FULLSCREEN | 
				SDL_WINDOW_OPENGL);
		else if (currentGame->fullscreen)
			m_display.setFullScreen(SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
	}
}

void Engine::endGame()
{
	currentGame->quit();
	inGame = false;
}

Engine::~Engine()
{
	currentGame->quit();
	delete currentGame;
	currentGame = nullptr;
	Shader::clearComponents();

	SDL_Quit();
}