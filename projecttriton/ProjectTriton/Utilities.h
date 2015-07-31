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

	//string FilePath = "../Models/";
	//string sFileName = "cube_animation_proper.xml";
	//Utility(Mesh type) : m_FileName(FilePath + sFileName) { run(type); }
	//Parser(Armature type) : m_FileName(FilePath + sFileName) { run(type); }
	//Parser(Game type) : m_FileName(FilePath + sFileName) { run(type); }
	//Parser(Scene type) : m_FileName(FilePath + sFileName) { run(type); }
	//Parser(Object type) : m_FileName(FilePath + sFileName) { run(type); }

	void parse(Mesh type, const char* filepath);
	//void parse(Armature type);
	//void parse(Game type);
	//void parse(Scene type);
	//void parse(Object type);

	Mesh getMesh(Mesh mesh);

	//short m_countVerts;
	//short m_countUVs;
	//short m_countNormals;
	//short m_countFaces;

	//string m_FileName;

	//vector<vec3> m_positions;
	//vector<vec2> m_texCoords;
	//vector<vec3> m_normals;
	//unsigned int** m_Faces;
	//mat4 m_Matrix;

}
