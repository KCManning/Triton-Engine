//-------------------------------------------------------------------------------------------------
#pragma once
#include "Texture.h"

namespace Triton
{
	// object made up of texture pointers
	struct Material
	{
		Texture * diffuse;
		Texture * normal;
		Shader * shader;

		Material(Texture * _diffuse, Texture * _normal) : diffuse(_diffuse), normal(_normal) {}
	};
}
	

