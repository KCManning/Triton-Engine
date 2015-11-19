#version 400

const float specularPower = 8.0;
const float specularIntensity = 1.0;
const vec4 ambientLight = vec4(0.1, 0.1, 0.1, 1.0);
const int MAX_POINTLIGHTS = 5;
const float pointLightRange = 5.0;

in vec2 texCoord0;
in vec3 normal0;
in vec3 worldPos0;

uniform sampler2D diffuse;
uniform vec3 eyePos;

struct PointLight
{
	vec3 color;
	vec3 atten;
	float intensity;
	vec3 position;
};

uniform PointLight pointLights[MAX_POINTLIGHTS];

vec4 resaturate(vec4 color)
{
	for(int i = 0; i < 3; ++i)
	{
		color[0] = min(1.0, color[0]);
		color[0] = max(0.0, color[0]);
	}
	color[3] = 1.0;
	
	return color;
}

vec4 calcPointLight(PointLight ptLight, vec3 normal)
{
	vec3 lightDirection = worldPos0 - ptLight.position;
	float distanceToPoint = length(lightDirection);
	
	lightDirection = normalize(lightDirection);
	
	float diffuseFactor = dot(-lightDirection, normal);
	
	vec4 diffuseColor = mix(vec4(0.0, 0.0, 0.0, 0.0), vec4(ptLight.color, 1.0) * ptLight.intensity * diffuseFactor, step(0.0, diffuseFactor));
	
	vec3 directionToEye = normalize(eyePos - worldPos0);
	vec3 reflectDirection = normalize(reflect(lightDirection, normal));
	
	float specularFactor = dot(directionToEye, reflectDirection);
	specularFactor = pow(specularFactor, specularPower);
		
	vec4 specularColor = mix(vec4(0.0, 0.0, 0.0, 0.0), vec4(ptLight.color, 1.0) * specularIntensity * specularFactor, step(0.0, specularFactor));
	
	vec4 ptLcolor = max(diffuseColor + specularColor, vec4(0.0, 0.0, 0.0, 0.0));
	
	float attenuation = ptLight.atten.x + ptLight.atten.y * distanceToPoint + ptLight.atten.z * distanceToPoint * distanceToPoint + 0.0001;
	
	return mix(vec4(0.0, 0.0, 0.0, 0.0), ptLcolor / attenuation, step(distanceToPoint, pointLightRange));
}

void main()
{
	vec4 totalLight = ambientLight;
	vec4 textureColor = texture2D(diffuse, texCoord0);
	
	for(int i = 0; i < MAX_POINTLIGHTS; ++i)
	{
		totalLight += mix(vec4(0.0, 0.0, 0.0, 0.0), calcPointLight(pointLights[i], normal0), step(0.0, pointLights[i].intensity));
	}
	
	gl_FragColor = textureColor * totalLight;
	
	//gl_FragColor = texture2D(diffuse, texCoord0);
}