#pragma once

#include "glm/glm.hpp"
#include "Mesh.h"
#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include <stdio.h>

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


class Parser
{
public:
	string FilePath = "../Models/";
	string sFileName = "cube_animation_proper.xml";
	Parser(Mesh type) : m_FileName(FilePath + sFileName) { run(type); }
	//Parser(Armature type) : m_FileName(FilePath + sFileName) { run(type); }
	//Parser(Game type) : m_FileName(FilePath + sFileName) { run(type); }
	//Parser(Scene type) : m_FileName(FilePath + sFileName) { run(type); }
	//Parser(Object type) : m_FileName(FilePath + sFileName) { run(type); }

	void run(Mesh type);
	//void run(Armature type);
	//void run(Game type);
	//void run(Scene type);
	//void run(Object type);

	list<string> split(const string &data);
	Mesh getMesh(Mesh mesh);

	short m_countVerts;
	short m_countUVs;
	short m_countNormals;
	short m_countFaces;

	string m_FileName;

	vector<vec3> m_positions;
	vector<vec2> m_texCoords;
	vector<vec3> m_normals;
	unsigned int** m_Faces;
	mat4 m_Matrix;

private:


};//end class