//-------------------------------------------------------------------------------------------------
#pragma once
#include "Material.h"

using namespace Triton;

void Material::bind()
{
	shader->bind();
	diffuse->bind();
}