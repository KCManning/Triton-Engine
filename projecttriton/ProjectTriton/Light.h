#include "glm\glm.hpp"

#define MAX_POINTLIGHTS 5

using glm::vec3;

namespace Triton
{
	struct PointLight
	{
		vec3 color;
		vec3 atten;
		float intensity;
		vec3 position;

		PointLight() {}
	};
}