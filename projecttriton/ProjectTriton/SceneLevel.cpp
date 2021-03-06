//-------------------------------------------------------------------------------------------------
#include "SceneLevel.h"

using namespace Triton;
using std::vector;

Camera SceneLevel::camera;

SceneLevel::SceneLevel()
{
	for (unsigned short i = 0; i < MAX_POINTLIGHTS; ++i)
	{
		pointlights[i].color = vec3();
		pointlights[i].atten = vec3();
		pointlights[i].intensity = 0;
		pointlights[i].position = vec3();
	}
}

void SceneLevel::update()
{
	// camera->update();
	camera.update();

	for (vector<ObjectEntity*>::const_iterator it = objects.cbegin(); it != objects.cend(); ++it)
	{
		(*it)->update();
	}
}

void SceneLevel::draw()
{
	// naive draw loop
	for (vector<ObjectEntity*>::const_iterator it = objects.cbegin(); it != objects.end(); ++it)
	{
		(*it)->material->bind();
		// Shader::active->uniforms[Shader::Uniforms::CAMERA] = glGetUniformLocation(Shader::active->handle, "camera");

		vec3 empty = vec3();

		glUniformMatrix4fv(Shader::active->uniforms[Shader::Uniforms::CAMERA], 1, GL_FALSE,
		 	&camera.getViewProjection()[0][0]);
		glUniform3fv(Shader::active->uniforms[Shader::Uniforms::CAMERAPOS], 1,
			&camera.pos[0]);
		glUniform3fv(Shader::active->uniforms[Shader::Uniforms::OBJECTPOS], 1, 
			&(*it)->position[0]);
		if ((*it)->armature != nullptr)
		{
			for (unsigned short i = 0; i < (*it)->armature->boneCount; ++i)
			{
				glUniform3fv(Shader::active->uniforms[Shader::Uniforms::UNIFORM_COUNT + i], 1,
					&(*it)->armature->anims[i].offsets[(*it)->armature->currentFrame][0]);
			}
			for (unsigned short i = (*it)->armature->boneCount; i < MAX_BONES; ++i)
			{
				glUniform3fv(Shader::active->uniforms[Shader::Uniforms::UNIFORM_COUNT + i], 1,
					&(*it)->armature->anims[i].offsets[(*it)->armature->currentFrame][0]);
			}
			for (unsigned short i = 0; i < (*it)->armature->boneCount; ++i)
			{
				glUniform4fv(Shader::active->uniforms[Shader::Uniforms::UNIFORM_COUNT + MAX_BONES + i], 1,
					&(*it)->armature->anims[i].rotations[(*it)->armature->currentFrame][0]);
			}
			for (unsigned short i = (*it)->armature->boneCount; i < MAX_BONES; ++i)
			{
				glUniform4fv(Shader::active->uniforms[Shader::Uniforms::UNIFORM_COUNT + MAX_BONES + i], 1,
					&(*it)->armature->anims[i].rotations[(*it)->armature->currentFrame][0]);
			}
			for (unsigned short i = 0; i < (*it)->armature->boneCount; ++i)
			{
				glUniform1i(Shader::active->uniforms[Shader::Uniforms::UNIFORM_COUNT + MAX_BONES * 2 + i],
					(*it)->armature->bones[i].parentIndex);
			}
			for (unsigned short i = (*it)->armature->boneCount; i < MAX_BONES; ++i)
			{
				glUniform1i(Shader::active->uniforms[Shader::Uniforms::UNIFORM_COUNT + MAX_BONES * 2 + i],
					-2);
			}
			for (unsigned short i = 0; i < (*it)->armature->boneCount; ++i)
			{
				glUniform3fv(Shader::active->uniforms[Shader::Uniforms::UNIFORM_COUNT + MAX_BONES * 3 + i], 1,
					&(*it)->armature->bones[i].head[0]);
			}
			for (unsigned short i = (*it)->armature->boneCount; i < MAX_BONES; ++i)
			{
				glUniform3fv(Shader::active->uniforms[Shader::Uniforms::UNIFORM_COUNT + MAX_BONES * 3 + i], 1,
					&empty[0]);
			}
			for (unsigned short i = 0; i < (*it)->armature->boneCount; ++i)
			{
				glUniform3fv(Shader::active->uniforms[Shader::Uniforms::UNIFORM_COUNT + MAX_BONES * 4 + i], 1,
					&(*it)->armature->anims[i].scales[(*it)->armature->currentFrame][0]);
			}
			for (unsigned short i = (*it)->armature->boneCount; i < MAX_BONES; ++i)
			{
				glUniform3fv(Shader::active->uniforms[Shader::Uniforms::UNIFORM_COUNT + MAX_BONES * 4 + i], 1,
					&(*it)->armature->anims[i].scales[(*it)->armature->currentFrame][0]);
			}
			for (unsigned short i = 0; i < MAX_POINTLIGHTS; ++i)
			{
				glUniform3fv(Shader::active->uniforms[Shader::Uniforms::UNIFORM_COUNT + MAX_BONES * 5 + i * 4], 1,
					&pointlights[i].color[0]);
				glUniform3fv(Shader::active->uniforms[Shader::Uniforms::UNIFORM_COUNT + MAX_BONES * 5 + i * 4 + 1], 1,
					&pointlights[i].atten[0]);
				glUniform1f(Shader::active->uniforms[Shader::Uniforms::UNIFORM_COUNT + MAX_BONES * 5 + i * 4 + 2],
					pointlights[i].color[0]);
				glUniform3fv(Shader::active->uniforms[Shader::Uniforms::UNIFORM_COUNT + MAX_BONES * 5 + i * 4 + 3], 1,
					&pointlights[i].position[0]);
			}
		}
		
		(*it)->draw();
	}
}

void SceneLevel::input(SDL_Event& e)
{
	// camera->input(e);
	camera.input(e);
	
	for (vector<ObjectEntity*>::const_iterator it = objects.cbegin(); it != objects.end(); ++it)
	{
		(*it)->input(e);
	}
}

SceneLevel::~SceneLevel()
{
	// delete camera;
	// camera = nullptr;

	for (vector<ObjectEntity*>::iterator it = objects.begin(); it != objects.end(); ++it)
		delete *it;

	objects.clear();
	
	for (vector<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it)
		delete *it;

	meshes.clear();

	for (vector<Armature*>::iterator it = armatures.begin(); it != armatures.end(); ++it)
		delete *it;

	armatures.clear();

	for (vector<Material*>::iterator it = materials.begin(); it != materials.end(); ++it)
		delete *it;

	materials.clear();

	for (vector<Texture*>::iterator it = textures.begin(); it != textures.end(); ++it)
		delete *it;

	textures.clear();

	for (vector<Shader*>::iterator it = shaders.begin(); it != shaders.end(); ++it)
		delete *it;

	shaders.clear();
}
