attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;

uniform mat4 camera;

varying vec2 texCoord0;
varying vec3 normal0;

void main()
{
	gl_Position = camera * vec4(position, 1.0);
	normal0 = (camera * vec4(normal, 0.0)).xyz;
	//normal0 = normal;
	texCoord0 = texCoord;
}