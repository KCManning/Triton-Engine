//-------------------------------------------------------------------------------------------------
#include "Camera.h"

using namespace Triton;

Camera::Camera(bool orthographic, unsigned short screenWidth, unsigned short screenHeight, 
	vec3 _position, vec3 _up, vec3 _forward, float field_of_view, float nearClippingDistance, 
	float farClippingDistance) : Position(_position), Up(_up), Forward(_forward), moveVector(0.f),
	rotationVector(1.f, vec3(0.f)), ViewWidth(screenWidth), ViewHeight(screenHeight)
{
	LookDirection = Forward;
	if (orthographic)
		Projection = ortho(-screenWidth / 2.f, screenWidth / 2.f, -screenHeight / 2.f, 
			screenHeight / 2.f, nearClippingDistance, farClippingDistance);
	else
		Projection = perspective(field_of_view, screenWidth / (float)screenHeight, 
			nearClippingDistance, farClippingDistance);

	View = Projection * lookAt(Position, Position + LookDirection, Up);
}

void Camera::Rotate(float euler_x, float euler_y, float euler_z)
{
	quat Look = quat(0.f, LookDirection);
	rotationVector = rotate(Look, euler_x, Right()) * rotate(Look, euler_y, Up) * 
		rotate(Look, euler_z, Forward);
}

void Camera::setProjection(unsigned short screenWidth,
	unsigned short screenHeight, float field_of_view,
	float nearClippingPlane, float farClippingPlane, bool orthographic)
{	
	float Field_Of_View, NearClip, FarClip;
	
	if (screenWidth != 0)
		ViewWidth = screenWidth;
	if (screenHeight != 0)
		ViewHeight = screenHeight;
	if (field_of_view != 0.f)
		Field_Of_View = field_of_view;
	if (nearClippingPlane != -1.f)
		NearClip = nearClippingPlane;
	if (farClippingPlane != -1.f)
		FarClip = farClippingPlane;
	
	if (orthographic)
		Projection = ortho(-ViewWidth / 2.f, ViewWidth / 2.f, -ViewHeight / 2.f, ViewHeight / 2.f, 
		FarClip, NearClip);
	else
		Projection = perspective(Field_Of_View, ViewWidth / (float)ViewHeight, NearClip, FarClip);

	View = Projection * lookAt(Position, Position + LookDirection, Up);
}

// temporary just for testing later
void Camera::input(SDL_Event& e)
{
	float frametime = 1 / 60.f;
	
	if (e.type == SDL_MOUSEBUTTONUP)
	{
		if (e.button.button == SDL_BUTTON_LEFT)
			if (SDL_SetRelativeMouseMode(SDL_FALSE) < 0)
				throw "Can't get out of relative mouse mode.";
	}
	if (e.type == SDL_MOUSEBUTTONDOWN)
	{
		if (e.button.button == SDL_BUTTON_LEFT)
			if (SDL_SetRelativeMouseMode(SDL_TRUE) < 0)
				throw "Couldn't not set relative mouse mode.";
	}
	if (e.type == SDL_MOUSEMOTION)
		if (SDL_GetRelativeMouseMode())
			Rotate(atanf(e.motion.yrel / float (ViewWidth)), 
				atanf(-e.motion.xrel / float (ViewHeight)), 0.f);
	if (e.type == SDL_KEYDOWN)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_w:
			moveVector.z = frametime; break;
		case SDLK_s:
			moveVector.z = -frametime; break;
		case SDLK_a:
			moveVector.x = -frametime; break;
		case SDLK_d:
			moveVector.x = frametime; break;
		case SDLK_SPACE:
			moveVector.y = frametime; break;
		case SDLK_LSHIFT:
			moveVector.y = -frametime; break;
		}
	}
	if (e.type == SDL_KEYUP)
	{
		const Uint8* keyStates = SDL_GetKeyboardState(NULL);
		
		switch (e.key.keysym.sym)
		{
		case SDLK_w:
			if (keyStates[SDL_SCANCODE_S])
				moveVector.z = -frametime;
			else
				moveVector.z = 0.f;
			break;
		case SDLK_s:
			if (keyStates[SDL_SCANCODE_W])
				moveVector.z = frametime;
			else
				moveVector.z = 0.f;

			break;
		case SDLK_a:
			if (keyStates[SDL_SCANCODE_D])
				moveVector.x = frametime;
			else
				moveVector.x = 0.f;
			break;
		case SDLK_d:
			if (keyStates[SDL_SCANCODE_A])
				moveVector.x = -frametime;
			else
				moveVector.x = 0.f;
			break;
		case SDLK_SPACE:
			if (keyStates[SDL_SCANCODE_LSHIFT])
				moveVector.y = -frametime;
			else
				moveVector.y = 0.f;
			break;
		case SDLK_LSHIFT:
			if (keyStates[SDL_SCANCODE_SPACE])
				moveVector.y = frametime;
			else
				moveVector.y = 0.f;
			break;
		}
	}
}

void Camera::update()
{
	// quat yaw = normalize(quat(rotationVector.w, 0.f, rotationVector.y, 0.f));
	// Forward = inverse(yaw) * Forward * yaw;

	// LookDirection = inverse(rotationVector) * LookDirection * rotationVector;
	// Position += (moveVector.x * Right()) + (moveVector.y * Up) + (moveVector.z * Forward);

	View = Projection * lookAt(Position, Position + LookDirection, Up);
}
