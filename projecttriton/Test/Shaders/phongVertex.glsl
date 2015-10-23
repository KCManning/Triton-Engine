#version 400

const int MAX_BONES = 20;

attribute vec3 vertex;
attribute vec2 UV;
attribute vec3 normal;
attribute vec4 tangent;
attribute vec4 weights;
attribute vec4 weight_indices;

uniform mat4 camera;
uniform vec3 objectPos;
uniform vec3 offsets[MAX_BONES];
uniform vec4 rotations[MAX_BONES];
uniform int parentIndices[MAX_BONES];

out vec2 texCoord0;
out vec3 normal0;
out vec3 worldPos0;

vec4 conjugate(vec4 quat)
{
	return vec4(quat.x, -quat.y, -quat.z, -quat.w);
}

// this is just here in case i want to format stuff
vec4 format(vec4 quat)
{
	return vec4(quat.x, -quat.y, quat.z, quat.w);
}

mat4 quatToMatrix(vec4 q0)
{
	return mat4(
	1.0 - 2.0*q0.y*q0.y - 2.0*q0.z*q0.z, 	2.0*q0.x*q0.y - 2.0*q0.z*q0.w, 			2.0*q0.x*q0.z + 2.0*q0.y*q0.w, 			0.0,
	2.0*q0.x*q0.y + 2.0*q0.z*q0.w, 			1.0 - 2.0*q0.x*q0.x - 2.0*q0.z*q0.z, 	2.0*q0.y*q0.z - 2.0*q0.x*q0.w, 			0.0,
	2.0*q0.x*q0.z - 2.0*q0.y*q0.w, 			2.0*q0.y*q0.z + 2.0*q0.x*q0.w, 			1.0 - 2.0*q0.x*q0.x - 2.0*q0.y*q0.y, 	0.0,
	0.0, 								0.0, 								0.0, 								1.0);
}

void main()
{
	int index0 = int(weight_indices.x);
	int index1 = int(weight_indices.y);
	int index2 = int(weight_indices.z);
	int index3 = int(weight_indices.w);
	
	vec4 q0 = format(rotations[index0]);
	vec4 q1 = format(rotations[index1]);
	vec4 q2 = format(rotations[index2]);
	vec4 q3 = format(rotations[index3]);
	vec3 t0 = offsets[index0];
	vec3 t1 = offsets[index1];
	vec3 t2 = offsets[index2];
	vec3 t3 = offsets[index3];
	
	mat4 boneMat0 = quatToMatrix(q0);
	mat4 boneMat1 = quatToMatrix(q1);
	mat4 boneMat2 = quatToMatrix(q2);
	mat4 boneMat3 = quatToMatrix(q3);
	
	vec4 nVertex0 = vec4(vertex - t0, 1.0);
	vec4 nVertex1 = vec4(vertex - t1, 1.0);
	vec4 nVertex2 = vec4(vertex - t2, 1.0);
	vec4 nVertex3 = vec4(vertex - t3, 1.0);
	vec4 nNormal0 = vec4(normal, 0.0);
	vec4 nNormal1 = vec4(normal, 0.0);
	vec4 nNormal2 = vec4(normal, 0.0);
	vec4 nNormal3 = vec4(normal, 0.0);
	
	nVertex0 = ((boneMat0 * nVertex0) + vec4(t0, 0.0)) * weights.x;
	nVertex1 = ((boneMat1 * nVertex1) + vec4(t1, 0.0)) * weights.y;
	nVertex2 = ((boneMat2 * nVertex2) + vec4(t2, 0.0)) * weights.z;
	nVertex3 = ((boneMat3 * nVertex3) + vec4(t3, 0.0)) * weights.w;
	nNormal0 = (boneMat0 * nNormal0) * weights.x;
	nNormal1 = (boneMat1 * nNormal1) * weights.y;
	nNormal2 = (boneMat2 * nNormal2) * weights.z;
	nNormal3 = (boneMat3 * nNormal3) * weights.w;
	
	vec4 nVertex = vec4((nVertex0 + nVertex1 + nVertex2 + nVertex3).xyz, 1.0);
	vec4 nNormal = nNormal0 + nNormal1 + nNormal2 + nNormal3;
	
	gl_Position = (camera * vec4(nVertex.xyz + objectPos, 1.0));
	normal0 = nNormal.xyz;
	texCoord0 = UV;
	worldPos0 = nVertex.xyz;
	
	//gl_Position = (camera * vec4(vertex + objectPos, 1.0));
	//normal0 = normal;
	//texCoord0 = UV;
	//worldPos0 = vertex;
}