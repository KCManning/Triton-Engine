attribute vec3 vertex;
attribute vec2 UV;
attribute vec3 normal;

out vec2 texCoord0;
out vec3 normal0;
out vec3 worldPos0;

void main()
{
	gl_Position = vec4(vertex, 1.0);
	normal0 = vec4(normal, 1.0).xyz;
	texCoord0 = UV;
	worldPos0 = vec4(vertex, 1.0).xyz;
}