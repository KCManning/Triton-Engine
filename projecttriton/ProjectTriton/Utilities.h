//-------------------------------------------------------------------------------------------------
#pragma once
#include "Game.h"
#include <fstream>

using glm::vec2;
using glm::vec3;
using glm::mat4;
using namespace std;

const short vect2 = 2;
const short vect3 = 3;

enum storageType
{
	typeMesh,
	typeArmature,
	typeGame,
	typeScene,
	typeObject
};

//useful helper functions or classes or just objects in general

namespace Triton
{
	Game* parseGameFile(const char* filepath);
	void parseSceneFile(const char* filepath, Game * game);
	unordered_map<string, ObjectEntity *>::iterator parseObjectFile(const char* filepath, SceneLevel * scene);
	list<string> parseOutComments(const char* filepath);
	list<string> split(const string &data);

	void parse(Mesh type, const char* filepath);
	void parse(Armature type, const char* filepath);
	void parse(Game type, const char* filepath);
	//void parse(Scene type);
	//void parse(Object type);

	Mesh getMesh(Mesh mesh); //*** Holdover?

}
