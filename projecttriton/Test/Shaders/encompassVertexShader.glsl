#version 400

// vertex attributes
attribute vec3 vertex;
attribute vec2 UV;
attribute vec3 normal;
attribute vec4 tangent;
//attribute vec4 weights; // make sure these are in order of bone parentage, max 4 bones can influence a single vertex
//attribute vec4 groups; // make sure these are in order of bone parentage

//const unsigned int MAX_BONE_COUNT = 80;

// scene uniforms
//uniform mat4 WVP; //world view projection
//uniform vec3 offsets[MAX_BONE_COUNT]; // bone position, x, y, z order, ??? turn to vec4 and have last element be scale
//uniform vec4 rotations[MAX_BONE_COUNT]; // bone rotation w, x, y, z order

// outputs
//varying vec4 outVertex;
varying vec2 outUV;
//varying vec4 outNormal;
//varying vec4 outTangent;
//varying vec4 outBitangent;

//vec4 conjugate(vec4 q)
//{
//	return vec4(q.w, -q.x, -q.y, -q.z);
//}

void main()	
{	
	vec3 vertex0 = vec3(0.0, 0.0, 0.0);
	vec3 normal0 = vec3(0.0, 0.0, 0.0);
	vec3 tangent0 = vec3(0.0, 0.0, 0.0);
	
	for(int i = 0; i < 4; ++i)
	{
		vec4 conjugation = conjugate(rotations[groups[i]]);
		vertex0 += (offsets[groups[i]] + (conjugation * (vertex - offsets[groups[i]]) * rotations[groups[i]].wxyz)) * weights[i];
		normal0 += (conjugation * normal * rotations[groups[i]].wxyz) * weights[i];
		tangent0 += (conjugation * tangent.xyz * rotations[groups[i]].wxyz) * weights[i];
	}
	
	outUV = UV;
	outNormal = WVP * vec4(normal0, 0.0);
	outTangent = WVP * vec4(tangent0, 0.0);
	outBitangent = WVP * vec4(tangent.w * cross(normal0, tangent0), 0.0);
	outVertex = WVP * vec4(vertex0, 1.0);
	gl_Position = outVertex;
	
	//temp vert shader
	gl_Position = vec4(vertex, 1.0);
}