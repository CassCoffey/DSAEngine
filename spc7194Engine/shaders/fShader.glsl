#version 430

in vec2 FragUV;
in vec3 FragPos;
in vec3 Normal; 
uniform sampler2D myTexture;
uniform vec3 lightPos;
uniform vec3 viewPos;

float near = 0.1f;
float far = 10.0f;
float ambientStrength = 0.1f;
float specularStrength = 0.5f;
vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0;
	return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;
	
	vec3 result = (ambient + diffuse + specular);

	float depth = LinearizeDepth(gl_FragCoord.z) / far; 
	gl_FragColor = texture(myTexture, FragUV);
	gl_FragDepth = depth;
}