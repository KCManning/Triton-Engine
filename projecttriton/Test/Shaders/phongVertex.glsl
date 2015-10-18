#version 400

const int MAX_BONES = 4;

attribute vec3 vertex;
attribute vec2 UV;
attribute vec3 normal;
attribute vec4 tangent;
attribute vec2 weights;
attribute vec2 weight_indices;

uniform mat4 camera;
uniform vec3 objectPos;
uniform vec3 offsets[MAX_BONES];
uniform vec4 rotations[MAX_BONES];

out vec2 texCoord0;
out vec3 normal0;
out vec3 worldPos0;

void main()
{
	int index0 = int(weight_indices.x);
	int index1 = int(weight_indices.y);
	
	vec4 q0 = rotations[index0].yzwx;
	vec4 q1 = rotations[index1].yzwx;
	vec3 t0 = offsets[index0];
	vec3 t1 = offsets[index1];
	
	mat4 boneMat0 = mat4(1.0 - 2.0*q0.y*q0.y - 2.0*q0.z*q0.z, 2.0*q0.x*q0.y - 2.0*q0.z*q0.w, 2.0*q0.x*q0.z + 2.0*q0.y*q0.w, 0.0,
	2.0*q0.x*q0.y + 2.0*q0.z*q0.w, 1.0 - 2.0*q0.x*q0.x - 2.0*q0.z*q0.z, 2.0*q0.y*q0.z - 2.0*q0.x*q0.w, 0.0,
	2.0*q0.x*q0.z - 2.0*q0.y*q0.w, 2.0*q0.y*q0.z + 2.0*q0.x*q0.w, 1.0 - 2.0*q0.x*q0.x - 2.0*q0.y*q0.y, 0.0,
	0.0, 0.0, 0.0, 1.0);
	
	mat4 boneMat1 = mat4(1.0 - 2.0*q1.y*q1.y - 2.0*q1.z*q1.z, 2.0*q1.x*q1.y - 2.0*q1.z*q1.w, 2.0*q1.x*q1.z + 2.0*q1.y*q1.w, 0.0,
	2.0*q1.x*q1.y + 2.0*q1.z*q1.w, 1.0 - 2.0*q1.x*q1.x - 2.0*q1.z*q1.z, 2.0*q1.y*q1.z - 2.0*q1.x*q1.w, 0.0,
	2.0*q1.x*q1.z - 2.0*q1.y*q1.w, 2.0*q1.y*q1.z + 2.0*q1.x*q1.w, 1.0 - 2.0*q1.x*q1.x - 2.0*q1.y*q1.y, 0.0,
	0.0, 0.0, 0.0, 1.0);
	
	vec4 nVertex0 = vec4(vertex, 0.0);
	vec4 nNormal = vec4(normal, 0.0);
	
	nVertex0 = (boneMat0 * nVertex0) * weights.x;
	vec4 nNormal0 = (boneMat0 * nNormal) * weights.x;
	
	nVertex0 = vec4(nVertex0.xyz, 0.0);
	vec4 nVertex1 = vec4(vertex, 0.0);
	
	nVertex1 = (boneMat1 * nVertex1) * weights.y;
	nNormal = (boneMat1 * nNormal) * weights.y + nNormal0;
	
	nVertex1 = vec4(nVertex1.xyz, 0.0);
	
	vec4 nVertex = nVertex0 + nVertex1;
	
						
	gl_Position = (camera * vec4(nVertex.xyz + objectPos, 1.0));
	normal0 = nNormal.xyz;
	texCoord0 = UV;
	worldPos0 = nVertex.xyz;
}