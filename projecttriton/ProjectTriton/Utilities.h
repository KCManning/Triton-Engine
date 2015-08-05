//---------------------------------------------------------------------
//	File: [File Name]
//
//	Functions:
//	[Functions Used]
//
//---------------------------------------------------------------------
#pragma once
#include "Game.h"
#include <fstream>

using glm::vec2;
using glm::vec3;
using glm::mat4;
using namespace std;

const short vect2 = 2;
const short vect3 = 3;

//---------------------------------------------------------------------
//	Function: [Name of this Function]
//
//	Title: main program
//
//	Description: 
//	[Description of Function’s purpose]
//
//	Programmer(s):
//	[Names involved in creation of this Function]
//
//	Date: [Date Function was started]
//
//	Version: [Version of Function. Release is always 1.0]
//
//	Testing Environment: 
//		Hardware: [Type of Machine code was tested on]
//
//		Software: [Operating System it was written on]
//		[Name of software Function was written on]
//
//	Input: [Input Types]
//
//	Output: [Output Types]
//
//	Parameters:
//	[Parameters of the Functions]
// 
//
//	Returns:
//	[Expected returns and causes]
// 
// 
//	Called By: [Other Functions calling this one]
// 
//	Calls: [Other functions called by this one]
//
//	History Log: 
//	[Initials, date, and succinct list of changes to the Function]
// 
//---------------------------------------------------------------------
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

	//Mesh getMesh(Mesh mesh); //*** Holdover?

}
