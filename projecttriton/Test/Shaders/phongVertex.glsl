#version 400

attribute vec3 vertex;
attribute vec2 UV;
attribute vec3 normal;

uniform mat4 camera;

out vec2 texCoord0;
out vec3 normal0;
out vec3 worldPos0;

void main()
{
	gl_Position = camera * vec4(vertex, 1.0);
	normal0 = normal;
	texCoord0 = UV;
	worldPos0 = vertex;
}