//-------------------------------------------------------------------------------------------------
#include "Shader.h"
#define ERROR_MAX 1024

using namespace Triton;

// declaration of static member variables
list<Shader::ShaderComponent> Shader::Components;

Shader::ShaderComponent::ShaderComponent(GLenum shaderType, string& GLSLstrings, 
	unsigned short comp_index)
{
	handle = glCreateShader(shaderType);

	// can't call checkShader error because we don't have a handle
	if (handle == NULL)
		cerr << "Error: Shader creation failed!" << endl;

	// type conversion between c++ types and openGL types
	const GLchar* GLSL_c_strings[1];
	GLint GLSL_character_count[1];

	GLSL_c_strings[0] = GLSLstrings.c_str();
	GLSL_character_count[0] = GLSLstrings.length();

	// sends the shader source code to openGL
	glShaderSource(handle, 1, GLSL_c_strings, GLSL_character_count);
	// compiles the shader
	glCompileShader(handle);

	// error checking to see if the shader compiled correctly
	error = checkShaderError(handle, GL_COMPILE_STATUS, false, 
		"Error: Shader Compilation Failed: ");

	index = comp_index;
}

unsigned short Shader::createComponent(string& GLSLstrings, GLenum shaderType)
{
	// constructs shader component and adds it to the components list
	Components.emplace_back(shaderType, GLSLstrings, Components.size());

	return Components.size() - 1;
}

void Shader::init(unsigned short components[])
{
	handle = glCreateProgram();

	unsigned short length = (sizeof(components) / sizeof(short));
	for (unsigned short i = 0; i < length; ++i)
	{
		list<ShaderComponent>::iterator it;
		bool startAtBegin = (components[i] < (Components.size()/2));
		if (startAtBegin)
		{
			it = Components.begin();
			for (unsigned short j = 1; j < components[i]; ++j)
				++it;
		}
		else
		{
			it = Components.end();
			for (unsigned short j = Components.size() - 2; j > components[i]; --j)
				--it;
		}
			
		glAttachShader(handle, (*it).handle);
	}

	// binds the locations of these variables in the shader program to these attribute locations
	glBindAttribLocation(handle, VERTEX, "vertex");
	glBindAttribLocation(handle, UV, "UV");
	glBindAttribLocation(handle, NORMAL, "normal");
	glBindAttribLocation(handle, TANGENT, "tangent");
	glBindAttribLocation(handle, WEIGHTS, "weights");
	glBindAttribLocation(handle, GROUPS, "groups");

	glLinkProgram(handle);
	error = checkShaderError(handle, GL_LINK_STATUS, true, "Error: Program Linking Failed: ");

	glValidateProgram(handle);
	error = checkShaderError(handle, GL_VALIDATE_STATUS, true, "Error: Program is Invalid: ");
}

void Shader::bind()
{
	glUseProgram(handle);
}

bool Triton::checkShaderError(GLuint shaderHandle, GLuint errorFlag, bool isProgram,
	const string& errorMessage)
{
	GLint success = 0;
	GLchar error[ERROR_MAX] = { 0 };

	// checks if checking for error on a shader program handle or shader component handle
	if (isProgram)
		// gets info for shader program, and assigns a positive value to success if successful
		glGetProgramiv(shaderHandle, errorFlag, &success);
	else
		// gets info for shader component, and assigns a positive value to success if successful
		glGetShaderiv(shaderHandle, errorFlag, &success);

	// checks if success is not false
	if (!success)
	{
		if (isProgram)
			// gets error mess
			glGetProgramInfoLog(shaderHandle, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shaderHandle, sizeof(error), NULL, error);

		// prints errormessage to console
		cerr << errorMessage << ": '" << error << "'" << endl;
	}

	return (success != GL_FALSE);
}