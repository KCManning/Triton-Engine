#version 400

attribute vec3 vertex;
attribute vec2 UV;
attribute vec3 normal;
attribute vec4 tangent;
attribute vec4 weights;
attribute uvec4 groups;

uniform mat4 camera;

out vec2 texCoord0;
out vec3 normal0;
out vec3 worldPos0;

void main()
{
	gl_Position = vec4(vertex, 1.0);
	normal0 = normal;
	texCoord0 = UV;
	worldPos0 = vertex;
}