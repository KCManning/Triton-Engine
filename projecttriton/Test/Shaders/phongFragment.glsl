#version 400

in vec2 texCoord0;
in vec3 normal0;
in vec3 worldPos0;

uniform sampler2D diffuse;

void main()
{
	// vec4 textureColor = texture2D(diffuse, texCoord0);
	
	//gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	gl_FragColor = texture2D(diffuse, texCoord0);
}