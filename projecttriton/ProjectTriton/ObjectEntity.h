//-------------------------------------------------------------------------------------------------
#pragma once

#include "SDL2\SDL.h"
#include "Mesh.h"
#include "Armature.h"
#include "Material.h"
#include "Texture.h"
#include "Shader.h"

#include <unordered_map>

namespace Triton
{
	// specifies assets it uses through the indexmap 
	// (ex. currentScene->meshes[indexmap["Cube"]]) <-will be changing, if not already
	// base class for other objectEntity types, but not pure virtual
	class ObjectEntity
	{
	public:
		// objects position in 3d space
		vec3 position;

		unordered_map<string, Mesh*>::const_iterator mesh;
		// pointer to default values of the armature this mesh uses
		unordered_map<string, Armature*>::const_iterator armatureID;
		unordered_map<string, Material*>::const_iterator material;

		// armature as is manipulated and animated by this object
		Armature armature;

		// event handles; changes private values that move are referenced by update
		void input(SDL_Event& e);
		// update, updates have to happen everyframe whereas input are only called when event polls
		// return true
		void update();

		ObjectEntity() {}
		~ObjectEntity();
	private:

	};
}



