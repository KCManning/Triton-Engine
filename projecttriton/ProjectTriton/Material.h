//-------------------------------------------------------------------------------------------------
#pragma once
#include "Texture.h"
#include "Shader.h"

namespace Triton
{
	// object made up of texture pointers
	struct Material
	{
		Texture* diffuse;
		// Texture* normal;
		// Texture* height;
		// Texture* ambient_occlussion;
		// Texture* specular;
		Shader* shader;

		Material();

		void bind();
	};
}
	

