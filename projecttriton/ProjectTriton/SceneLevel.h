//-------------------------------------------------------------------------------------------------
#pragma once

#include "ObjectEntity.h"
#include "Camera.h"
#include "Texture.h"
#include "Shader.h"
#include <vector>

using std::vector;

namespace Triton
{
	// class that loads in and stores the asset files(meshes and textures and whatnot), 
	// object files(script files of data for say like a game character or weapon), and 
	// script files that script code for executing events related to SceneLevel wide variables,
	// like say a cutscene, or a building collapsing after a specific amount of time
	// ^^subject to change^^
	class SceneLevel
	{
	public:
		vector<Mesh*> meshes;
		vector<Armature*> armatures;
		vector<Texture*> textures;
		vector<Shader*> shaders;
		vector<Material*> materials;
		vector<ObjectEntity*> objects;
		PointLight pointlights[MAX_POINTLIGHTS];

		static Camera camera;

		// properties *future feature once scripting is better implemented*

		// constructor
		SceneLevel();
		// updates scene and all objects in it
		void update();

		void input(SDL_Event& e);
		// draws scene
		void draw();

		// destroys scene objects, be careful of memory leaks, but also keep an eye on performance
		~SceneLevel();
	private:

	};
}



