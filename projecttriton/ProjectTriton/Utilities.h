//-------------------------------------------------------------------------------------------------
//	File: [File Name]
//
//	Functions:
//	[Functions Used]
//
//-------------------------------------------------------------------------------------------------
#pragma once
#include "Game.h"
#include <fstream>
#include <unordered_map>

using std::unordered_map;
using std::list;

//---------------------------------------------------------------------
//	Function: [Name of this Function]
//
//	Title: main program
//
//	Description: 
//	[Description of Function's purpose]
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

namespace Triton
{
	// static class to store values to accessed by parse fuctions
	class Parser{
	public:
		// current game Directory being parsed from
		string currentGameDirectory;
		// current scene for which assets and objects are being parsed
		SceneLevel*& currentScene;
		// current string value id's and the index of where they are in their
		// class's object array in the currentScene Object of the 
		// currentGame
		unordered_map<string, unsigned short> meshMap;
		unordered_map<string, unsigned short> armatureMap;
		unordered_map<string, unsigned short> objectMap;
		unordered_map<string, unsigned short> materialMap;
		unordered_map<string, unsigned short> textureMap;
		unordered_map<string, unsigned short> shaderMap;

		Parser() : currentScene(m_nullScene) { m_nullScene = nullptr; }

		void clear();

		~Parser(){ clear(); }

	private:
		// solely for initializing currentScene pointer reference
		SceneLevel* m_nullScene;

	};
	
	// converts file in filepath specified and returns the strings of said file, removing 
	// linefeeds, comments and other unnecessary lines from the file, either xml or script
	list<string> getTokensFromFile(const char* filepath);
	// split's line from file into a list of string tokens
	list<string> split(const string &data);
	// parses an game xml file
	void parse(const char* filepath, Game*& type);
	// parses scene script xml file that calls asset and object parse functions
	void parse(const char* filepath, SceneLevel*& type);
	// parses object xml file and assigns pointers to object components
	string parse(const char* filepath, ObjectEntity*& type);
	// parses a blender exported mesh xml file
	string parse(const char* filepath, Mesh*& type);
	// parses a blender exported armature xml file
	string parse(const char* filepath, Armature*& type);
	// reads material xml file and looks for pointers to material components using string id values
	// from file
	string parse(const char* filepath, Material*& type);
	// gets GLSL strings for contructing shader components
	string parse(const char* filepath, Shader*& type);
	// uses sdl image to load a texture into memory
	void parse(const char* filepath, Texture*& type);
	// grabs the line of the file in filepath, and puts them through the shader constructor
	string parse(const char* filepath, Shader*& type);
}
