//-------------------------------------------------------------------------------------------------
#include "Game.h"

using namespace Triton;

Game::Game() : currentScene(nullptr)
{
	windowWidth, windowHeight = 0;
	desktop_resolution, fullscreen = false;
}

void Game::update()
{
	currentScene->update();
	currentScene->draw();
}

void Game::input(SDL_Event &e)
{
	currentScene->input(e);
}

void Game::quit()
{
	delete currentScene;
	currentScene = nullptr;
	sceneDirectories.clear();
}
