//-------------------------------------------------------------------------------------------------
#include "Engine.h"

using namespace Triton;
//using namespace std;

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

	std::cout << "Start Up took " << SDL_GetTicks() - startTime << " milliseconds" << std::endl;
}

void Engine::update()
{
	// clear stuff on the buffer you are about to draw on
	m_display.clear();
	
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
}

void Engine::loadGame(const char* GameFile)
{
	try{
		Uint32 startOfLoadTime = SDL_GetTicks();
		currentGame = new Game;
		parse(GameFile, currentGame);
		std::cout << "Game took " << SDL_GetTicks() - startOfLoadTime << " milliseconds to load"
			<< std::endl;
		inGame = true;
	}
	catch (const string& errorMessage)
	{
		std::cerr << "error occurred while loading game: " << errorMessage << std::endl;
		delete currentGame;
		currentGame = nullptr;
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
	if (currentGame != nullptr)
		currentGame->quit();
	delete currentGame;
	currentGame = nullptr;

	SDL_Quit();
}