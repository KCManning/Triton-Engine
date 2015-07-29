//-------------------------------------------------------------------------------------------------
#pragma once
#include "GL\glew.h"
#define GLM_SWIZZLE 

#include <iostream>
#include <string>
#include <list>

using namespace std;

namespace Triton
{
	// an encapsulatin class for a shader program handle 
	class Shader
	{
	public:
		// attributes
		enum Attribs
		{
			VERTEX,
			UV,
			NORMAL,
			TANGENT,
			BITSIGN,
			WEIGHTS,
			GROUPS,

			ATTRIB_COUNT
		};
		// nested object, shader programs are made up of multiple 
		// components, a vertex and fragment shader is the minimum required for rendering 
		// the rest are optional, but can be used
		// GL_COMPUTE_SHADER, only available in openGL 4.3 and later
		// GL_VERTEX_SHADER, 
		// GL_TESS_CONTROL_SHADER, 
		// GL_TESS_EVALUATION_SHADER, 
		// GL_GEOMETRY_SHADER, 
		// or GL_FRAGMENT_SHADER
		struct ShaderComponent
		{
			// index in Shader::Components
			unsigned short index;
			
			// handle openGL returned that references the shader
			GLint handle;

			// true if there was an error during shader creation or linking
			bool error;
	
			// contructor: 
			// shaderType = type of shader(compute, vertex, tess, etc.)
			// GLSLstrings = strings of the glsl file
			ShaderComponent(GLenum shaderType, string& GLSLstrings, unsigned short comp_index);
	
			// destructor
			~ShaderComponent(){ glDeleteShader(handle); }
		};
	
		// true if there was an error during program creation or linking
		bool error;

		// static array of shader pieces that all shader programs can access
		static list<ShaderComponent> Components;
	
		// static function that adds a shader component to the list and returns the index
		// of that component in the Components array
		// GLSLstrings = strings of the glsl file, essentially taking it line by line
		// shaderType = type of shader component to be created
		static unsigned short createComponent(string& GLSLstrings, GLenum shaderType);
		
		// default contructor
		Shader() { handle = NULL; error = false; }

		// array of indices to Components to use for this program in order 
		// i.e. vertex shader first, frag shader last
		void init(unsigned short components[]);

		// sets this as the program openGL is currently using
		void bind();

		// clears the Components list, make sure to call this 
		void clearComponents() { Components.clear(); }
	
		// destructor
		~Shader()
		{
			// should delete the array of component pointers not the components
			m_components.clear();
			list<ShaderComponent*>::iterator it = m_components.begin();
			for (unsigned short i = 0; i < m_components.size(); ++i)
			{
				glDetachShader(handle, (*it)->handle);
				++it;
			}
			glDeleteProgram(handle);
		}
		// shader program handle
		GLint handle;
	
	private:
		// an array of pointers to the Components this shader program is utilizing
		list<ShaderComponent*> m_components;
	};

	// for error checking during shader creation and linking process
	// shaderHandle = handle of shader component or program you're checking for errors
	// errorFlag = type of error you're checking for
	// isProgram = true, if shaderHandle is a program handle, false if component handle
	// errorMessage = errorMessage to be concatenated to openGL's error message
	// errorMessage: 'error'
	bool checkShaderError(GLuint shaderHandle, GLuint errorFlag, bool isProgram,
		const string& errorMessage);
}

