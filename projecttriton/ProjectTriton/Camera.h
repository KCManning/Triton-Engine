//-------------------------------------------------------------------------------------------------
#pragma once

#include "SDL2\SDL.h"
#include "glm\glm.hpp"
#include "glm\gtc\quaternion.hpp"
#include "glm\gtx\transform.hpp"

using namespace glm;


namespace Triton
{
	struct Camera
	{
		mat4 Projection;
		mat4 View;

		vec3 Position;
		// should be normalized
		vec3 Up;
		vec3 Forward;
		vec3 LookDirection;

		// x, y, z speeds, when camera is moving
		vec3 moveVector;
		quat rotationVector;

		// stored for temporary input function
		unsigned short ViewWidth;
		unsigned short ViewHeight;

		// constructor
		Camera(bool orthographic = false, unsigned short screenWidth = 640,
			unsigned short screenHeight = 480, vec3 _position = vec3(0.f, 0.f, 0.f),
			vec3 _up = vec3(0.f, 1.f, 0.f), vec3 _forward = vec3(0.f, 0.f, 1.f),
			float field_of_view = 70.f, float nearClippingDistance = 0.1f, 
			float farClippingDistance = 1000.f);

		// adjusts Project matrix
		void setProjection(unsigned short screenWidth = 0, unsigned short screenHeight = 0, 
			float field_of_view = 0.f, float nearClippingPlane = -1.f, 
			float farClippingPlane = -1.f, bool orthographic = false);

		vec3 Left(){ return cross(Forward, -Up); }

		vec3 Right(){ return cross(Forward, Up); }

		// rotate with euler angles in radians
		void Rotate(float euler_x, float euler_y, float euler_z);

		// updates orientation vectors and matrices
		void update();

		// temporary
		void input(SDL_Event& e);
	};
}
