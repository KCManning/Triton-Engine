//-------------------------------------------------------------------------------------------------
#include "Camera.h"

using namespace Triton;
using namespace std;

Camera::Camera()
{
	pos = glm::vec3(0.f, 0.f, -4.f);
	forward = glm::vec3(0.f, 0.f, 1.f); // make forward and lookDirection different variables
	up = glm::vec3(0.f, 1.f, 0.f);
	fVel = 0.f;
	uVel = 0.f;
	lVel = 0.f;
	camSpeed = 1.f / 60.f;
	//camRot = glm::quat(1.f, 0.f, 0.f, 0.f);
	screenWidth = 640.f;
	screenHeight = 480.f;
	fov = 70.f;
	aspect = screenWidth / screenHeight;
	zNear = 0.2f;
	zFar = 1000.f;

	perspective = glm::perspective(fov, aspect, zNear, zFar);
}

void Camera::set(glm::vec3 npos, glm::vec3 nforward, glm::vec3 nup, float nfov, float nwidth, float nheight, float nzNear, float nzFar)
{
	pos = npos;
	forward = nforward;
	up = nup;
	fov = nfov;
	screenWidth = nwidth;
	screenHeight = nheight;
	aspect = screenWidth / screenHeight;
	zNear = nzNear;
	zFar = nzFar;

	perspective = glm::perspective(fov, aspect, zNear, zFar);
}

void Camera::input(SDL_Event & e)
{
	// if (camSpeed != ncamSpeed)
	// {
	// 	camSpeed = ncamSpeed;
	// }

	camSpeed = 1.f / 24;

	if (e.type == SDL_MOUSEBUTTONUP)
	{
		if (e.button.button == SDL_BUTTON_LEFT)
		{
			if (SDL_SetRelativeMouseMode(SDL_FALSE) < 0)
			{
				string err =  "Can't get out of relative mouse mode.";
				throw err;
			}
		}
	}
	if (e.type == SDL_MOUSEBUTTONDOWN)
	{
		if (e.button.button == SDL_BUTTON_LEFT)
		{
			if (SDL_SetRelativeMouseMode(SDL_TRUE) < 0)
			{
				string err = "Couldn't not set relative mouse mode.";
				throw err;
			}
		}
	}
	if (e.type == SDL_MOUSEMOTION)
	{
		if (SDL_GetRelativeMouseMode())
		{
			//glm::vec3 mouseLocX = forward + glm::vec3(e.motion.xrel / screenHeight, 0.f, 0.f);
			//glm::vec3 mouseLocY = forward + glm::vec3(-e.motion.yrel / screenHeight, 0.f, 0.f);

			//angle between vector to mouseLocX and forward
			//float yRad = acosf(glm::dot(forward, mouseLocX) / (glm::length(forward)*glm::length(mouseLocX)));
			//angle between vector to mouseLocY and forward
			//float xRad = acosf(glm::dot(forward, mouseLocY) / (glm::length(forward)*glm::length(mouseLocY)));
			//float zRad = 0.f;
			//other way to find the angles
			//float yRad = atanf(-e.motion.xrel / (screenHeight));
			//float xRad = atanf(e.motion.yrel / (screenHeight));

			//glm::mat4 rotY = glm::rotate(yRad, up);
			//glm::mat4 rotX = glm::rotate(xRad, getLeft());

			//camRot = rotY * rotX;

			//glm::quat rotqY = glm::rotate(glm::quat(0.f, forward), yRad, up);
			//glm::quat rotqX = glm::rotate(glm::quat(0.f, forward), xRad, getRight());

			//camRot = rotqY * rotqX;

			//forward = glm::conjugate(camRot) * forward * camRot;

			glm::quat camRot = glm::rotate(glm::quat(0.f, forward), atanf(-e.motion.xrel / (screenHeight)), up) *
				glm::rotate(glm::quat(0.f, forward), atanf(e.motion.yrel / (screenHeight)), getRight());

			forward = glm::inverse(camRot) * forward * camRot;

			//forward = (camRot * glm::vec4(forward, 1.f)).xyz();
			//up = (camRot * glm::vec4(up, 1.f)).xyz();

		}
	}
	if (e.type == SDL_KEYDOWN)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_w:
		{
			fVel = 1; break;
		}
		case SDLK_s:
		{
			fVel = -1; break;
		}
		case SDLK_a:
		{
			lVel = -1; break;
		}
		case SDLK_d:
		{
			lVel = 1; break;
		}
		case SDLK_SPACE:
		{
			uVel = 1; break;
		}
		case SDLK_LSHIFT:
		{
			uVel = -1; break;
		}
		}
	}
	if (e.type == SDL_KEYUP)
	{
		const Uint8* keyStates = SDL_GetKeyboardState(NULL);
		switch (e.key.keysym.sym)
		{
		case SDLK_w:
		{
			if (!keyStates[SDL_SCANCODE_S])
			{
				fVel = 0;
			}
			else
			{
				fVel = -1;
			}

			break;
		}
		case SDLK_s:
		{
			if (!keyStates[SDL_SCANCODE_W])
			{
				fVel = 0;
			}
			else
			{
				fVel = 1;
			}

			break;
		}
		case SDLK_a:
		{
			if (!keyStates[SDL_SCANCODE_D])
			{
				lVel = 0;
			}
			else
			{
				lVel = 1;
			}

			break;
		}
		case SDLK_d:
		{
			if (!keyStates[SDL_SCANCODE_A])
			{
				lVel = 0;
			}
			else
			{
				lVel = -1;
			}

			break;
		}
		case SDLK_SPACE:
		{
			if (!keyStates[SDL_SCANCODE_LSHIFT])
			{
				uVel = 0;
			}
			else
			{
				uVel = -1;
			}

			break;
		}
		case SDLK_LSHIFT:
		{
			if (!keyStates[SDL_SCANCODE_SPACE])
			{
				uVel = 0;
			}
			else
			{
				uVel = 1;
			}

			break;
		}
		}
	}
}

void Camera::update()
{
	pos += forward * fVel * camSpeed;
	pos += getRight() * lVel * camSpeed;
	pos += up * uVel * camSpeed;
}

Camera::~Camera()
{

}
