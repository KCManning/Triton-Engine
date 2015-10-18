//-------------------------------------------------------------------------------------------------
#include "Armature.h"

using namespace Triton;

Armature::Armature()
{
	
}

void Armature::play()
{
	if (++currentFrame >= endFrame)
		currentFrame = 0;
}