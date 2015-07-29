//-------------------------------------------------------------------------------------------------
#pragma once
#include "glm\glm.hpp"
#include "glm\gtc\quaternion.hpp"
#include <vector>
#include <unordered_map>

using glm::vec3;
using glm::quat;
using namespace std;

namespace Triton
{
	// structure that contains information for mesh skin animations
	class Armature
	{
	public:
		struct BoneAnimation
		{
			// array of offsets for each frame, offsets are relative to armature
			vec3* offsets;
			// array of rotations for each frame, rotations are relative to rest_rotation 
			quat* rotations;

			// fps of the scene in case it's different from the fps the blender file was exported
			static unsigned short sceneFPS;

			// current frame of this animation to be floored
			float currentFrame;

			BoneAnimation(vec3* _offsets, quat* _rotations) :
				offsets(_offsets), rotations(_rotations), currentFrame(0) {}

			// moves animation up a number of frames as according to sceneFPS and armatureFPS, assigns 
			// vec3 and quat for the currentFrame to the offset and rotation parameters
			void getFrame(vec3& offset, quat& rotation, unsigned short armatureFPS = sceneFPS);

			~BoneAnimation() { delete[] offsets; delete[] rotations; }
		};
		
		struct Bone
		{
			// bone base location relative to armature
			vec3 offset; 
			// rotation quaternion of pose bone or rest bone, animations will be a set quaternion
			// of rotations relative to this rotation
			quat rotation; 
			// set of offsets and rotations for each frame for this bone, with a respective poseID string
			BoneAnimation* animations;

			// constuctor
			Bone(vec3 _offset, quat _rotation) : 
				offset(_offset), rotation(_rotation) {}

			// sets offset and rotations to their values according to the current frame
			void play(unsigned short animation_index, unsigned short armatureFPS);
		};
		// array of bones
		Bone* bones;
		// number of bones in array
		unsigned short boneCount;

		// map of poseIDs, to animation indexes in the bones
		unordered_map<string, unsigned short> animations;

		// calls play across bone array using the currentAnimationIndex
		void play();

		// gets animation index from animations map using AnimID as a key
		void setCurrentAnimation(const string& AnimID) { currentAnimationIndex = animations[AnimID]; }

		// constructor
		Armature(Bone* _bones = nullptr, unsigned short FPS = 60) : bones(_bones), 
			currentAnimationIndex(0), boneCount(0), nativeFPS(FPS) {}

		// destructor
		~Armature() { delete[] bones; animations.clear(); }

	private:
		// current animation index
		unsigned short currentAnimationIndex;
		// armature's exported FPS
		unsigned short nativeFPS;
	};
}
	

