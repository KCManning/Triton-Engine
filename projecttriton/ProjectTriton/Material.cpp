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
	if (shader)
		shader->bind();
	if (diffuse)
		diffuse->bind();
}