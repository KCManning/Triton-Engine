//-------------------------------------------------------------------------------------------------
#include "Mesh.h"

using namespace Triton;

Mesh::Mesh(vec3* _vertices, vec2* _UVs, vec3* _normals, vec3* _tangents, bool* _bitangent_signs, 
	vec4* _weights, uvec4* _groups, unsigned int* _indices)
{
	haveUVs = false;
	haveNormals = false;
	haveTangentSpace = false;
	isWeighted = false;
	isIndexed = false;
	error = false;
	empty = false;
	bufferCount = 0;
	
	if (_vertices != nullptr)
	{
		++bufferCount;
		for (unsigned short i = 0; i < sizeof(_vertices) / sizeof(*_vertices); ++i)
			vertices.push_back(_vertices[i]);

		if (_UVs != nullptr)
		{
			haveUVs = true; ++bufferCount;
			for (unsigned short i = 0; i < sizeof(_UVs) / sizeof(*_UVs); ++i)
				UVs.push_back(_UVs[i]);
		}

		if (_normals != nullptr)
		{
			haveNormals = true; ++bufferCount;
			for (unsigned short i = 0; i < sizeof(_normals) / sizeof(*_normals); ++i)
				normals.push_back(_normals[i]);
		}

		if (_tangents != nullptr)
		{
			haveTangentSpace = true; ++bufferCount;
			for (unsigned short i = 0; i < sizeof(_tangents) / sizeof(*_tangents); ++i)
				tangents.emplace_back(_tangents[i], _bitangent_signs[i]?1.f:-1.f);
		}

		if (_weights != nullptr)
		{
			isWeighted = true; bufferCount += 2;
			for (unsigned short i = 0; i < sizeof(_weights) / sizeof(*_weights); ++i)
				weights.push_back(_weights[i]);
			if (_groups != nullptr)
				for (unsigned short i = 0; i < sizeof(_groups) / sizeof(*_groups); ++i)
					groups.push_back(_groups[i]);
			else
				error = true; errorMessage = "vertex group weights incomplete!";
		}

		if (_indices != nullptr)
		{
			isIndexed = true; ++bufferCount;
			for (unsigned short i = 0; i < sizeof(_indices) / sizeof(*_indices); ++i)
				indices.push_back(_indices[i]);
			//indices.assign(_indices, _indices + sizeof(_indices) / sizeof(*_indices));
		}
	}
	else
		empty = true;

	if (!(error || empty))
	{
		// generates a vertex array object and assigns the handle to vao
		glGenVertexArrays(1, &vao);
		// binds the vao for editting
		glBindVertexArray(vao);

		// generates buffers for data to attach to the vao
		glGenBuffers(bufferCount, vbo);

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

		if (haveUVs)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo[UV_BUFFER]);
			glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(UVs[0]), &UVs[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(Shader::Attribs::UV);
			glVertexAttribPointer(Shader::Attribs::UV, 2, GL_FLOAT, GL_FALSE, 0, 0);
		}
		else
			++skippedBuffers;

		if (haveNormals)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo[NORMAL_BUFFER - skippedBuffers]);
			glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normals[0]), &normals[0], 
				GL_STATIC_DRAW);
			glEnableVertexAttribArray(Shader::Attribs::NORMAL);
			glVertexAttribPointer(Shader::Attribs::NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}
		else
			++skippedBuffers;

		if (haveTangentSpace)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo[TANGENT_BUFFER - skippedBuffers]);
			glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(tangents[0]), &tangents[0], 
				GL_STATIC_DRAW);
			glEnableVertexAttribArray(Shader::Attribs::TANGENT);
			glVertexAttribPointer(Shader::Attribs::TANGENT, 4, GL_FLOAT, GL_FALSE, 0, 0);
		}
		else
			++skippedBuffers;

		if (isWeighted)
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

		if (isIndexed)
		{
			// only one per vao
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[INDEX_BUFFER - skippedBuffers]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0],
				GL_STATIC_DRAW);
		}

		// *it's going to be rebound anyway why unbind, it's only an extra unneeded api call*
		// glBindVertexArray(0);
	}		
}

void Mesh::draw()
{
	// sends this mesh's vertex values to the shader
	glBindVertexArray(vao);

	if (isIndexed)
		// tells the gpu to draw the vertex to the screen
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, vbo[0], vertices.size());
	
	// *it's going to be rebound anyway why unbind, it's only an extra unneeded api call*
	// clears the shader's vertex array for the next mesh to be drawn
	// glBindVertexArray(0); 
}