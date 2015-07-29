//-------------------------------------------------------------------------------------------------
#pragma once
#include "Game.h"
#include <fstream>

using namespace std;

//useful helper functions or classes or just objects in general

namespace Triton
{
	Game* parseGameFile(const char* filepath);
	void parseSceneFile(const char* filepath, Game * game);
	unordered_map<string, ObjectEntity *>::iterator parseObjectFile(const char* filepath, SceneLevel * scene);
	list<string> parseOutComments(const char* filepath);
	list<string> split(const string &data);
}
	

