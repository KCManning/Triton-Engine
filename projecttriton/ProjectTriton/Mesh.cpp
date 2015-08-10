//-------------------------------------------------------------------------------------------------
#include "Mesh.h"

using namespace Triton;
using namespace std;

void Mesh::init()
{
	if (!vertices.empty())
	{
		// generates a vertex array object and assigns the handle to vao
		glGenVertexArrays(1, &vao);
		// binds the vao for editting
		glBindVertexArray(vao);

		// generates buffers for data to attach to the vao
		glGenBuffers(MAX_BUFFER_COUNT, vbo);

		// sets up buffer to editted
		glBindBuffer(GL_ARRAY_BUFFER, vbo[VERTEX_BUFFER]);
		// type of data to be sent, size in bytes of the array, address of the first element, 
		// expected method array is to be used
		// GL_DYNAMIC_COPY, means the array is dynamic and will be both written to by the cpu
		// and openGL
		// is using GL_STATIC_DRAW for now for efficiency
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0],
			GL_STATIC_DRAW);
		// enables this vertex attribute in the openGL program, basically the variable in the glsl
		// file
		glEnableVertexAttribArray(Shader::Attribs::VERTEX);
		// attribute, number of elements in each element(vec3), whether it's normalized, 
		// amount to skip each step, amount to skip at start
		glVertexAttribPointer(Shader::Attribs::VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);

		unsigned short skippedBuffers = 0;

		if (!UVs.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo[UV_BUFFER]);
			glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(UVs[0]), &UVs[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(Shader::Attribs::UV);
			glVertexAttribPointer(Shader::Attribs::UV, 2, GL_FLOAT, GL_FALSE, 0, 0);
		}
		else
			++skippedBuffers;

		if (!normals.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo[NORMAL_BUFFER - skippedBuffers]);
			glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normals[0]), &normals[0],
				GL_STATIC_DRAW);
			glEnableVertexAttribArray(Shader::Attribs::NORMAL);
			glVertexAttribPointer(Shader::Attribs::NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}
		else
			++skippedBuffers;

		if (!tangents.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo[TANGENT_BUFFER - skippedBuffers]);
			glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(tangents[0]), &tangents[0],
				GL_STATIC_DRAW);
			glEnableVertexAttribArray(Shader::Attribs::TANGENT);
			glVertexAttribPointer(Shader::Attribs::TANGENT, 4, GL_FLOAT, GL_FALSE, 0, 0);
		}
		else
			++skippedBuffers;
		
		if (!weights.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo[WEIGHT_BUFFER - skippedBuffers]);
			glBufferData(GL_ARRAY_BUFFER, weights.size() * sizeof(weights[0]), &weights[0],
				GL_STATIC_DRAW);
			glEnableVertexAttribArray(Shader::Attribs::WEIGHTS);
			glVertexAttribPointer(Shader::Attribs::WEIGHTS, 4, GL_FLOAT, GL_FALSE, 0, 0);
		
			glBindBuffer(GL_ARRAY_BUFFER, vbo[GROUP_INDEX_BUFFER - skippedBuffers]);
			glBufferData(GL_ARRAY_BUFFER, groups.size() * sizeof(groups[0]), &groups[0],
				GL_STATIC_DRAW);
			glEnableVertexAttribArray(Shader::Attribs::GROUPS);
			glVertexAttribPointer(Shader::Attribs::GROUPS, 4, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
		}
		else
			skippedBuffers += 2;
		
		 if (!indices.empty())
		 {
		 	// only one per vao
		 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
		 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0],
		 		GL_STATIC_DRAW);
		 }

		glBindVertexArray(NULL);
	}
}

void Mesh::draw()
{
	// sends this mesh's vertex values to the shader
	glBindVertexArray(vao);

	if (!indices.empty())
		// tells the gpu to draw the vertices to the screen
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	// else
	// 	glDrawArrays(GL_TRIANGLES, vbo[0], vertices.size());

	glBindVertexArray(NULL);
}

Mesh::~Mesh()
{
	glDeleteBuffers(MAX_BUFFER_COUNT, vbo);
	glDeleteVertexArrays(1, &vao);

	vertices.clear();
	UVs.clear();
	normals.clear();
	tangents.clear();
	weights.clear();
	groups.clear();
	indices.clear();
}