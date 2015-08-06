//-------------------------------------------------------------------------------------------------
#pragma once

#include "SDL2\SDL.h"
#include "Mesh.h"
#include "Armature.h"
#include "Material.h"

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

		Mesh* mesh;
		// Armature* armature;
		Material* material;

		// mesh is manipulated and animated by this object, a copy of the object pointed by 
		// armature
		// Armature rig;

		// event handles; changes private values that move are referenced by update
		void input(SDL_Event& e);
		// update, updates have to happen everyframe whereas input are only called when event polls
		// return true
		void update();

		void draw();

		ObjectEntity();
		~ObjectEntity();
	private:

	};
}



