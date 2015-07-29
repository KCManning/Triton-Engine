//-------------------------------------------------------------------------------------------------
#include "Armature.h"

using namespace Triton;

unsigned short Armature::BoneAnimation::sceneFPS = 60;

void Armature::play()
{
	for (unsigned short i = 0; i < boneCount; ++i)
		bones[i].play(currentAnimationIndex, nativeFPS);
}

void Armature::Bone::play(unsigned short animation_index, unsigned short armatureFPS)
{
	animations[animation_index].getFrame(offset, rotation, armatureFPS);
}

void Armature::BoneAnimation::getFrame(vec3& offset, quat& rotation, unsigned short armatureFPS)
{
	offset = offsets[(int)currentFrame];
	rotation = rotations[(int)currentFrame];
	currentFrame += (float)sceneFPS / armatureFPS;

	float maxFrame = (float)((sizeof(offsets) / sizeof(*offsets)));
	if (currentFrame > maxFrame)
		currentFrame = currentFrame - maxFrame;
}
