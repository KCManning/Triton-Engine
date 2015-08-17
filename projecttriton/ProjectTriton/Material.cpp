//-------------------------------------------------------------------------------------------------
#pragma once
#include "Material.h"

using namespace Triton;

Material::Material()
{
	shader = nullptr;
	diffuse = nullptr;
}

void Material::bind()
{
	// shader->bind();
	diffuse->bind();
}