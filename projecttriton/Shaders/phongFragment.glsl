const int MAX_POINT_LIGHTS = 2;
const int MAX_SPOT_LIGHTS = 2;

in vec2 texCoord0;
in vec3 normal0;
in vec3 worldPos0;

uniform sampler2D diffuse;
uniform vec4 matcolor;
uniform vec3 eyePos;
uniform vec3 ambientLight;

uniform float specularIntensity;
uniform float specularPower;

struct BaseLight
{
	vec3 color;
	float intensity;
};

struct DirectionLight
{
	BaseLight base;
	vec3 direction;
};

struct Attenuation
{
	float constant;
	float linear;
	float exponent;
};

struct PointLight
{
	BaseLight base;
	Attenuation atten;
	vec3 position;
	float range;
};

struct SpotLight
{
	PointLight pointLight;
	vec3 direction;
	float cutoff;
	float exponent;
};

uniform DirectionLight directionLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

vec4 calcLight(BaseLight base, vec3 direction, vec3 normal)
{
	//technically Attenuation
	float diffuseFactor = dot(-direction, normal);
	
	vec4 diffuseColor = vec4(0, 0, 0, 0);
	vec4 specularColor = vec4(0, 0, 0, 0);
	
	if(diffuseFactor > 0)
	{
		diffuseColor = vec4(base.color, 1.0) * base.intensity * diffuseFactor;
		
		vec3 directionToEye = normalize(eyePos - worldPos0);
		vec3 reflectDirection = normalize(reflect(direction, normal0));
		
		float specularFactor = dot(directionToEye, reflectDirection);
		specularFactor = pow(specularFactor, specularPower);
		
		if(specularFactor > 0)
		{
			specularColor = vec4(base.color, 1.0) * specularIntensity * specularFactor;
		}
	}
	
	return diffuseColor + specularColor;
}

vec4 calcDirectionLight(DirectionLight dirLight, vec3 normal)
{
	return calcLight(dirLight.base, dirLight.direction, normal);
}

vec4 calcPointLight(PointLight ptLight, vec3 normal)
{
	vec3 lightDirection = worldPos0 - ptLight.position;
	float distanceToPoint = length(lightDirection);
	
	if(distanceToPoint > ptLight.range)
	{
		return vec4(0, 0, 0, 0);
	}
	
	lightDirection = normalize(lightDirection);
	
	vec4 ptLcolor = calcLight(ptLight.base, lightDirection, normal);
	
	float attenuation = ptLight.atten.constant + ptLight.atten.linear * distanceToPoint +
	ptLight.atten.exponent * distanceToPoint * distanceToPoint + 0.0001;
	
	return ptLcolor / attenuation;
}

vec4 calcSpotLight(SpotLight spotLight, vec3 normal) //is okay for looking down but no flashlight effect
{
	vec3 lightDirection = spotLight.pointLight.position - worldPos0;
	
	float distanceToPoint = length(lightDirection);
	
	float spotFactor = dot(lightDirection, spotLight.direction);
	
	float spotCenterToPoint = dot(lightDirection, normal) * tan(acos(spotFactor/(length(spotLight.direction)*distanceToPoint)) + acos(distanceToPoint)) - distanceToPoint * sin(acos(distanceToPoint));
	
	spotFactor = max(dot(normalize(spotLight.direction), normalize(-lightDirection)), 0.0);
	
	if(spotCenterToPoint > spotLight.pointLight.range)
	{
		return vec4(0, 0, 0, 0);
	}
	
	if(spotFactor < spotLight.cutoff)
	{
		return vec4(0, 0, 0, 0);
	}
	
	spotFactor = pow(spotFactor, spotLight.exponent);
	
	vec4 color = calcLight(spotLight.pointLight.base, spotLight.direction, normal);
	
	float attenuation0 = spotFactor/(spotLight.pointLight.atten.constant + spotLight.pointLight.atten.linear * spotCenterToPoint +
	spotLight.pointLight.atten.exponent * spotCenterToPoint * spotCenterToPoint + 0.0001);
	
	float attenuation1 = spotFactor/(spotLight.pointLight.atten.constant + spotLight.pointLight.atten.linear * distanceToPoint +
	spotLight.pointLight.atten.exponent * distanceToPoint * distanceToPoint + 0.0001);
	
	return color * attenuation0 * attenuation1;
}

void main()
{
	vec4 totalLight = vec4(ambientLight, 1);
	vec4 textureColor = texture2D(diffuse, texCoord0);
	vec4 color = matcolor;
	
	if(textureColor != vec4(0, 0, 0, 0))
	{
		color = matcolor * textureColor;
	}
	
	totalLight += calcDirectionLight(directionLight, normal0); 
	
	for(int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		if(pointLights[i].base.intensity > 0)
		{
			totalLight += calcPointLight(pointLights[i], normal0); 
		}
	}
	
	for(int i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		if(spotLights[i].pointLight.base.intensity > 0)
		{
			totalLight += calcSpotLight(spotLights[i], normal0); 
		}
	}
	
	gl_FragColor = color * totalLight;
}