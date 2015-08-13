//-------------------------------------------------------------------------------------------------
#pragma once
#include "GL\glew.h"
#include "glm\glm.hpp"
#include "Shader.h"

#include <vector>

using glm::vec3;
using glm::vec2;
using glm::vec4;
using glm::uvec4;
using std::vector;

namespace Triton
{
	// a wrapper of a group of vector arrays, vectors instead of lists because they need to be 
	// contiguous in memory to pass into openGL shaders
	struct Mesh
	{
		enum Buffers
		{
			VERTEX_BUFFER,
			UV_BUFFER,
			NORMAL_BUFFER,
			TANGENT_BUFFER,
			WEIGHT_BUFFER,
			GROUP_INDEX_BUFFER,
			INDEX_BUFFER,
			MAX_BUFFER_COUNT
		};
		
		// position in 3d space
		vector<vec3> vertices;
		// coordinates to align with a texture for each vertex
		vector<vec2> UVs;
		// where the face should be facing, also decides how light reflects off the surface
		vector<vec3> normals;
	
		// tangent space, vectors perpendicular to the normal, for better lighting calculation
		// allows for normal maps to be used
		vector<vec4> tangents;
	
		// weights for, size of each element will be determined by the number 
		// each vertex will be affected by a max of four bones
		vector<vec4> weights;
		vector<uvec4> groups;
	
		// possibly a light group buffer?
		// vector<vec4> lightGroup;
	
		// indices of each unique vertex
		// size of this this vector will be the drawcount
		vector<unsigned int> indices;
		
		// essentially the handle for this mesh i openGL
		GLuint vao;
		// handles for the pieces of the mesh, each vector array available, reduce buffer object count
		GLuint vbo[MAX_BUFFER_COUNT];
		
		Mesh() {}

		// draws the mesh in the openGL context
		void draw();

		void init();
	
		~Mesh();
	};
}