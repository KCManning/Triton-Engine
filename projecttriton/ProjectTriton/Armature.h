//-------------------------------------------------------------------------------------------------
#pragma once
#include "glm\glm.hpp"
#include "glm\gtc\quaternion.hpp"
#include <vector>
#include <unordered_map>

#define MAX_BONE_CHILDREN 10
#define MAX_BONES 80

using glm::vec3;
using glm::quat;
using std::string;

namespace Triton
{
	struct Bone
	{
		string name;
		// locations of bone's head and tail
		vec3 head;
		// vec3 tail;
		// the root is zero, what level of the bone hierarchy
		// short level;
		// short childCount;

		// Bone* children[MAX_BONE_CHILDREN];
		// Bone* parent;
	};
	struct Animation
	{
		// aligned with Armature::bones
		std::vector<vec3> offsets;
		std::vector<quat> rotations;
	};
	// structure that contains information for mesh skin animations
	class Armature
	{
	public:
		Armature();
		Bone bones[MAX_BONES];
		Animation anims[MAX_BONES];
		short boneCount;
		short currentFrame = 0;
		short endFrame = 0;
		// Bone* root;
		void play();
	};
}
	

