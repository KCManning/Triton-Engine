//-------------------------------------------------------------------------------------------------
#include "Armature.h"

using namespace Triton;

Armature::Armature()
{

}

void Armature::play()
{
	if (++currentFrame > frameSets[frameSet].endFrame)
		currentFrame = frameSets[frameSet].startFrame;
}