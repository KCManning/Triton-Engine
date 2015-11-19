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
uniform vec3 bonePos[MAX_BONES];
uniform vec3 scales[MAX_BONES];

out vec2 texCoord0;
out vec3 normal0;
out vec3 worldPos0;
out vec3 eyePos;

vec4 conjugate(vec4 quat)
{
	return vec4(-quat.x, -quat.y, -quat.z, quat.w);
}

// this is just here in case i want to format stuff
vec4 format(vec4 quat)
{
	return vec4(quat.x, quat.y, quat.z, quat.w);
}

mat4 quatToMatrix(vec4 q0, vec3 t0, vec3 s0)
{
	return mat4(
	s0.x*(1.0 - 2.0*q0.y*q0.y - 2.0*q0.z*q0.z), 	2.0*q0.x*q0.y - 2.0*q0.z*q0.w, 				2.0*q0.x*q0.z + 2.0*q0.y*q0.w, 				0.0,
	2.0*q0.x*q0.y + 2.0*q0.z*q0.w, 				s0.y*(1.0 - 2.0*q0.x*q0.x - 2.0*q0.z*q0.z), 	2.0*q0.y*q0.z - 2.0*q0.x*q0.w, 				0.0,
	2.0*q0.x*q0.z - 2.0*q0.y*q0.w, 				2.0*q0.y*q0.z + 2.0*q0.x*q0.w, 				s0.z*(1.0 - 2.0*q0.x*q0.x - 2.0*q0.y*q0.y), 	0.0,
	t0.x, 									t0.y, 									t0.z, 									1.0);
}

void main()
{
	int index[4];
	index[0] = int(weight_indices.x);
	index[1] = int(weight_indices.y);
	index[2] = int(weight_indices.z);
	index[3] = int(weight_indices.w);
	
	float weight[4];
	weight[0] = weights.x;
	weight[1] = weights.y;
	weight[2] = weights.z;
	weight[3] = weights.w;
	
	mat4 boneMat[MAX_BONES];
	
	for (int i = 0; i < MAX_BONES; ++i)
	{
		boneMat[i] = quatToMatrix(format(rotations[i]), offsets[i], scales[i]);
	}
	
	vec4 nVertex[4];
	vec4 nNormal[4];
	vec4 nVertex_f = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 nNormal_f = vec4(0.0, 0.0, 0.0, 0.0);
	
	for(int i = 0; i < 4; ++i)
	{
		nVertex[i] = vec4(vertex, 1.0);
		nNormal[i] = vec4(normal, 0.0);
		
		vec4 bone_offset = vec4(bonePos[index[i]], 0.0);
		nVertex[i] -= bone_offset;
		nVertex[i] = boneMat[index[i]] * nVertex[i];
		nNormal[i] = boneMat[index[i]] * nNormal[i];
		nVertex[i] += bone_offset;
		
		int pIndex = parentIndices[index[i]];
		// might want to change this
		while(pIndex > -1)//pIndex would be -1 for any bone without a parent and -2 for bones that don't exist
		{
			vec4 bone_offset = vec4(bonePos[pIndex], 0.0);
			nVertex[i] -= bone_offset;
			nVertex[i] = boneMat[pIndex] * nVertex[i];
			nNormal[i] = boneMat[pIndex] * nNormal[i];
			nVertex[i] += bone_offset;
			pIndex = parentIndices[pIndex];
		}
		
		nVertex_f += nVertex[i] * weight[i];
		nNormal_f += nNormal[i] * weight[i];
	}
	
	gl_Position = (camera * vec4(nVertex_f.xyz + objectPos, 1.0));
	normal0 = nNormal_f.xyz;
	texCoord0 = UV;
	worldPos0 = nVertex_f.xyz;
}