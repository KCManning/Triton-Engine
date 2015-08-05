//-------------------------------------------------------------------------------------------------
#include "SceneLevel.h"

using namespace Triton;

SceneLevel::SceneLevel() : camera(nullptr)
{
}

void SceneLevel::update()
{
	camera->update();

	for (list<ObjectEntity*>::const_iterator it = objects.cbegin(); it != objects.end(); ++it)
	{
		(*it)->update();
	}
}

void SceneLevel::draw()
{
	for (list<ObjectEntity*>::const_iterator it = objects.cbegin(); it != objects.end(); ++it)
	{
		(*it)->draw();
	}
}

void SceneLevel::input(SDL_Event& e)
{
	camera->input(e);
	
	for (list<ObjectEntity*>::const_iterator it = objects.cbegin(); it != objects.end(); ++it)
	{
		(*it)->input(e);
	}
}

SceneLevel::~SceneLevel()
{
	delete camera;

	for (list<ObjectEntity*>::iterator it = objects.begin(); it != objects.end(); ++it)
		delete *it;

	objects.clear();
	
	for (list<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it)
		delete *it;

	meshes.clear();

	for (list<Armature*>::iterator it = armatures.begin(); it != armatures.end(); ++it)
		delete *it;

	armatures.clear();

	for (list<Material*>::iterator it = materials.begin(); it != materials.end(); ++it)
		delete *it;

	materials.clear();

	for (list<Texture*>::iterator it = textures.begin(); it != textures.end(); ++it)
		delete *it;

	textures.clear();

	for (list<Shader*>::iterator it = shaders.begin(); it != shaders.end(); ++it)
		delete *it;

	shaders.clear();
}
