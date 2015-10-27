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

vec4 conjugate(vec4 quat)
{
	return vec4(-quat.x, -quat.y, -quat.z, quat.w);
}

// this is just here in case i want to format stuff
vec4 format(vec4 quat)
{
	return vec4(quat.x, quat.y, quat.z, quat.w);
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
	
	vec4 q[4];
	//q[0] = format(rotations[index[0]]);
	//q[1] = format(rotations[index[1]]);
	//q[2] = format(rotations[index[2]]);
	//q[3] = format(rotations[index[3]]);
	vec3 t[4];
	//t[0] = offsets[index[0]];
	//t[1] = offsets[index[1]];
	//t[2] = offsets[index[2]];
	//t[3] = bonePos[index[3]];
	vec3 b[4];
	//b[0] = bonePos[index[0]];
	//b[1] = bonePos[index[1]];
	//b[2] = bonePos[index[2]];
	//b[3] = bonePos[index[3]];
	mat4 boneMat[4];
	
	for(int i = 0; i < 4; ++i)
	{
		boneMat[i] = quatToMatrix(format(rotations[index[i]]));
		t[i] = offsets[index[i]];
		b[i] = bonePos[index[i]];
	}
	
	vec4 nVertex0 = vec4(vertex - b[0]// - t[0]
	, 0.0);                          
	vec4 nVertex1 = vec4(vertex - b[1]// - t[1]
	, 0.0);                           
	vec4 nVertex2 = vec4(vertex - b[2]// - t[2]
	, 0.0);                           
	vec4 nVertex3 = vec4(vertex - b[3]// - t[3]
	, 0.0);
	vec4 nNormal0 = vec4(normal, 0.0);
	vec4 nNormal1 = vec4(normal, 0.0);
	vec4 nNormal2 = vec4(normal, 0.0);
	vec4 nNormal3 = vec4(normal, 0.0);
	
	nVertex0 = ((boneMat[0] * nVertex0) + vec4(b[0], 0.0)
	) * weights.x;
	nVertex1 = ((boneMat[1] * nVertex1) + vec4(b[1], 0.0)
	) * weights.y;
	nVertex2 = ((boneMat[2] * nVertex2) + vec4(b[2], 0.0)
	) * weights.z;
	nVertex3 = ((boneMat[3] * nVertex3) + vec4(b[3], 0.0)
	) * weights.w;
	nNormal0 = (boneMat[0] * nNormal0) * weights.x;
	nNormal1 = (boneMat[1] * nNormal1) * weights.y;
	nNormal2 = (boneMat[2] * nNormal2) * weights.z;
	nNormal3 = (boneMat[3] * nNormal3) * weights.w;
	
	vec4 nVertex_f = vec4((nVertex0 + nVertex1 + nVertex2 + nVertex3).xyz, 1.0);
	vec4 nNormal_f = nNormal0 + nNormal1 + nNormal2 + nNormal3;
	
	//version 1 above here
	
	//version 2 currently not working below here
	
	//mat4 boneMat[MAX_BONES];
	//
	//for (int i = 0; i < MAX_BONES; ++i)
	//{
	//	boneMat[i] = quatToMatrix(format(rotations[i]));
	//}
	//
	//vec4 nVertex[4];
	//vec4 nNormal[4];
	//vec4 nVertex_f = vec4(0.0, 0.0, 0.0, 0.0);
	//vec4 nNormal_f = vec4(0.0, 0.0, 0.0, 0.0);
	//
	//for(int i = 0; i < 4; ++i)
	//{
	//	int boneChain[MAX_BONES];
	//	boneChain[0] = index[i];
	//	int boneCount = 1;
	//	int pIndex = parentIndices[index[i]];
	//	
	//	for (int j = 0; j < MAX_BONES - 1; ++j)
	//	{
	//		boneChain[j + 1] = pIndex;
	//		boneCount += int(step(float(-1), float(pIndex)));
	//		pIndex = parentIndices[pIndex];
	//	}
	//	
	//	nVertex[i] = vec4(vertex, 0.0);
	//	nNormal[i] = vec4(normal, 0.0);
	//	
	//	for (int j = 0; j < MAX_BONES; ++j)
	//	{
	//		int cbIndex = MAX_BONES - j - 1; // current bone Index
	//		vec4 aBonePos = vec4(bonePos[boneChain[cbIndex]] + (offsets[boneChain[cbIndex]] * step(float(boneCount - 1), float(cbIndex))) * step(float(-1), float(boneChain[cbIndex])), 0.0);
	//		nVertex[i] -= aBonePos;
	//		nVertex[i] = boneMat[boneChain[cbIndex]] * nVertex[i];
	//		nNormal[i] = boneMat[boneChain[cbIndex]] * nNormal[i];
	//		nVertex[i] += aBonePos;
	//		nVertex[i] += vec4(offsets[boneChain[cbIndex]] * step(float(boneCount - 1), float(cbIndex)), 0.0);
	//	}
	//	
	//	nVertex_f += nVertex[i] * weight[i];
	//	nNormal_f += nNormal[i] * weight[i];
	//}
	
	gl_Position = (camera * vec4(nVertex_f.xyz + objectPos, 1.0));
	normal0 = nNormal_f.xyz;
	texCoord0 = UV;
	worldPos0 = nVertex_f.xyz;
	
	//gl_Position = (camera * vec4(vertex + objectPos, 1.0));
	//normal0 = normal;
	//texCoord0 = UV;
	//worldPos0 = vertex;
}