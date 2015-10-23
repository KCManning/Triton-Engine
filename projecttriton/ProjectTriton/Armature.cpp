//-------------------------------------------------------------------------------------------------
#include "Armature.h"

using namespace Triton;

Armature::Armature()
{
	frameSets[0].startFrame = 0;
	frameSets[0].endFrame = 179;
}

void Armature::play()
{
	if (++currentFrame > frameSets[frameSet].endFrame)
		currentFrame = frameSets[frameSet].startFrame;
}