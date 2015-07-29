//-------------------------------------------------------------------------------------------------
#include "Game.h"

using namespace Triton;

Game::Game()
{
	windowWidth, windowHeight = 0;
	desktop_resolution, fullscreen = false;
	currentScene = nullptr;
}

void Game::update()
{

}

void Game::input(SDL_Event &e)
{

}

void Game::quit()
{

}

Game::~Game()
{
	if (currentScene)
		delete currentScene;
	sceneDirectories.clear();
}
