//-------------------------------------------------------------------------------------------------
#pragma once
#include "GL\glew.h"
#define GLM_SWIZZLE 

#include <string>
#include <list>

using std::string;

namespace Triton
{
	// an encapsulatin class for a shader program handle 
	class Shader
	{
	public:
		// vertex attributes in shader
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

		enum Uniforms
		{
			CAMERA,
			OBJECTPOS,
			UNIFORM_COUNT
		};

		enum ShaderType
		{
			VERTEX_SHADER,
			FRAGMENT_SHADER,
			SHADERTYPE_COUNT
		};

		// GL_COMPUTE_SHADER, only available in openGL 4.3 and later
		// GL_VERTEX_SHADER, 
		// GL_TESS_CONTROL_SHADER, 
		// GL_TESS_EVALUATION_SHADER, 
		// GL_GEOMETRY_SHADER, 
		// or GL_FRAGMENT_SHADER
		
		// the currently active shader
		static Shader* active;
	
		// static function that adds a shader component to the list and returns the index
		// of that component in the Components array
		// GLSLstrings = strings of the glsl file, essentially taking it line by line
		// shaderType = type of shader component to be created
		void addComponent(string& GLSLstrings, GLenum shaderType);
		
		// default contructor
		Shader();

		// compile shader program
		void compile();

		// sets this as the program openGL is currently using
		void bind();

		GLuint uniforms[UNIFORM_COUNT];
	
		// destructor
		~Shader();
		
		// shader program handle
		GLint handle;
	private:
		GLuint m_components[SHADERTYPE_COUNT];
		unsigned short m_componentCount;
	};

	// for error checking during shader creation and linking process
	// shaderHandle = handle of shader component or program you're checking for errors
	// errorFlag = type of error you're checking for
	// isProgram = true, if shaderHandle is a program handle, false if component handle
	// errorMessage = errorMessage to be concatenated to openGL's error message
	// errorMessage: 'error'
	// return false if no error and true if an error occured
	void checkShaderError(GLuint shaderHandle, GLuint errorFlag, bool isProgram,
		const string& errorMessage);
}

