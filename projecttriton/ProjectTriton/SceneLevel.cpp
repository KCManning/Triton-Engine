//-------------------------------------------------------------------------------------------------
#include "SceneLevel.h"

using namespace Triton;

SceneLevel::SceneLevel()
{
}

void start()
{

}

void update()
{

}

void next(SceneLevel& next_scene)
{

}

SceneLevel::~SceneLevel()
{
	meshes.clear();
	armatures.clear();
	materials.clear();
	textures.clear();
	objects.clear();
	shaders.clear();

	delete camera;
}
