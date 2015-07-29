#version 400

//varying vec4 outVertex;
varying vec2 outUV;
//varying vec4 outNormal;
//varying vec4 outTangent;
//varying vec4 outBitangent;

uniform sampler2D diffuse;

void main()
{
	// temp frag shader
	gl_FragColor = texture2D(diffuse, outUV);
}