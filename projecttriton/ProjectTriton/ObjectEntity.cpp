//-------------------------------------------------------------------------------------------------
#include "ObjectEntity.h"

using namespace Triton;

ObjectEntity::ObjectEntity()
{
	Mesh* mesh = nullptr;
	// Armature* armature = nullptr;
	Material* material = nullptr;
}

void ObjectEntity::input(SDL_Event& e)
{

}

void ObjectEntity::update()
{
	
}

void ObjectEntity::draw()
{
	mesh->draw();
}

ObjectEntity::~ObjectEntity()
{
	
}
