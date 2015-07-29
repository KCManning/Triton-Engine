attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;

uniform mat4 camera;
uniform mat4 transform;

out vec2 texCoord0;
out vec3 normal0;
out vec3 worldPos0;

void main()
{
	gl_Position = camera * transform * vec4(position, 1.0);
	//normal0 = (camera * vec4(normal, 0.0)).xyz;
	normal0 = (transform * vec4(normal, 1.0)).xyz;
	texCoord0 = texCoord;
	worldPos0 = (transform * vec4(position, 1.0)).xyz;
}