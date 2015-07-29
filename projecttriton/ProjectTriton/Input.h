//-------------------------------------------------------------------------------------------------
#pragma once

// won't be implemented till later

#include "SDL2\SDL.h"

namespace Triton
{
	// Similar to SDL_Event but is also able to throw physics type events,
	// and events defined by scripts, and allows for key rebindings
	struct GameEvent
	{
		enum EventType
		{
			SDL_EVENT,
			COLLISION_EVENT,
			SCRIPT_EVENT,
			NUMBER_OF_EVENT_TYPES
		};

		SDL_Event& e;
		unsigned short type;

		GameEvent(SDL_Event& _e, unsigned short _type = SDL_EVENT) : e(_e), type(_type) {}
	};
}

