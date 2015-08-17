//-------------------------------------------------------------------------------------------------
#pragma once

#include "SDL2\SDL.h"
#include "glm\glm.hpp"
#include "glm\gtc\quaternion.hpp"
#include "glm\gtx\transform.hpp"
#include <string>

using namespace glm;

namespace Triton
{
	struct Camera
	{
		mat4 perspective;

		float fVel; //forward velocity
		float uVel; //upward velocity
		float lVel; //lateral velocity left and right
		float camSpeed;
		//glm::quat camRot;
		float fov;
		float screenWidth;
		float screenHeight;
		float aspect;
		float zNear;
		float zFar;

		vec3 pos;
		vec3 forward;
		vec3 up;

		// constructor
		Camera();

		void set(glm::vec3 npos, glm::vec3 nforward, glm::vec3 nup, float nfov,
			float nwidth, float nheight, float nzNear, float nzFar);

		void input(SDL_Event &e);

		void update();

		vec3 getLeft()
		{
			return glm::cross(forward, -up);
		}

		vec3 getRight()
		{
			return glm::cross(forward, up);
		}

		mat4 getViewProjection() const
		{
			return perspective * lookAt(pos, pos + forward, up);
		}

		~Camera();
	};
}
