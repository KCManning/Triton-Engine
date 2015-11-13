//-------------------------------------------------------------------------------------------------
#include "ObjectEntity.h"

using namespace Triton;

ObjectEntity::ObjectEntity()
{
	Mesh* mesh = nullptr;
	Armature* armature = nullptr;
	Material* material = nullptr;
}

void ObjectEntity::input(SDL_Event& e)
{
	if (e.type == SDL_KEYDOWN)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_t:
		{
			++(armature->frameSet);
			if (armature->frameSet == armature->animCount)
			{
				armature->frameSet = 0;
			}
			break;
		}
		}
	}
}

void ObjectEntity::update()
{
	if (armature != nullptr)
		armature->play();
}

void ObjectEntity::draw()
{
	mesh->draw();
}

ObjectEntity::~ObjectEntity()
{
	
}
