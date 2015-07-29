varying vec2 texCoord0;
varying vec3 normal0;

uniform sampler2D diffuse;
uniform vec4 matcolor;

void main()
{
	gl_FragColor = texture2D(diffuse, texCoord0) 
	* clamp(dot(vec3(0, 1, 0), normal0), 0.0, 1.0) //light is coming from the top of the camera
	* matcolor;
}