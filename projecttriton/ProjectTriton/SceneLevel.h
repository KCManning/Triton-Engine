//-------------------------------------------------------------------------------------------------
#pragma once

#include "ObjectEntity.h"
#include "Camera.h"

namespace Triton
{
	// class that loads in and stores the asset files(meshes and textures and whatnot), 
	// object files(script files of data for say like a game character or weapon), and 
	// script files that script code for executing events related to level wide variables,
	// like say a cutscene, or a building collapsing after a specific amount of time
	// ^^subject to change^^
	class SceneLevel
	{
	public:
		// map of ids and assets this scene uses
		unordered_map<string, Mesh*> meshes;
		unordered_map<string, Armature*> armatures;

		unordered_map<string, Material*> materials;
		unordered_map<string, Texture*> textures;

		unordered_map<string, ObjectEntity*> objects;
		
		unordered_map<string, Shader*> shaders;

		Camera * camera;

		// properties *future feature once scripting is better implemented*

		// constructor
		SceneLevel();
		// plays scene using the scene scripts
		void start();
		// updates scene and all objects in it
		void update();
		// ends scene, moves same assets into next scene rather than unloading and reloading them
		void next(SceneLevel& next_scene);

		// destroys scene objects, be careful of memory leaks, but also keep an eye on performance
		~SceneLevel();
	private:

	};
}



