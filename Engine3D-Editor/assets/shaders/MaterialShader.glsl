#shader vertex
#version 410 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;
layout (location = 2) in vec3 a_Normal;

out vec3 v_Position;
out vec2 v_TexCoord;
out vec3 v_Normal;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main()
{
	v_Position = (u_Transform * vec4(a_Position, 1.0f)).xyz;
	v_TexCoord = a_TexCoord;
	v_Normal = normalize((u_Transform * vec4(a_Normal, 0.0)).xyz);

	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#shader fragment
#version 410 core

struct Material
{
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	float Shininess;
	float Transparency;

	sampler2D DiffuseTexture;
	sampler2D SpecularTexture;
	sampler2D NormalMap;
};

struct Light
{
	vec3 Direction;

	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
};

out vec4 color;

in vec3 v_Position;
in vec2 v_TexCoord;
in vec3 v_Normal;

uniform Material u_Material;
uniform Light u_Light;
uniform vec3 u_CameraPosition;

uniform bool u_UseDiffuseTexture;
uniform bool u_UseSpecularTexture;
uniform bool u_UseNormalMap;

void main()
{

	// Diffuse light
	vec3 posToLight = normalize(-u_Light.Direction);
	float diffuseIntensity = max(dot(posToLight, v_Normal), 0);
	

	// Specular light
	vec3 viewDir = normalize(u_CameraPosition - v_Position);
	vec3 reflectedRay = normalize(reflect(-posToLight, v_Normal));
	float specularIntensity = pow(max(dot(viewDir, reflectedRay), 0.0), u_Material.Shininess);

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	if(u_UseDiffuseTexture == true)
	{
		ambient = u_Light.Ambient * texture(u_Material.DiffuseTexture, v_TexCoord).xyz;
		diffuse = u_Light.Diffuse * texture(u_Material.DiffuseTexture, v_TexCoord).xyz * diffuseIntensity;
	}
	else
	{
		ambient = u_Light.Ambient * u_Material.Ambient;
		diffuse = u_Light.Diffuse * u_Material.Diffuse * diffuseIntensity;
	}

	if(u_UseSpecularTexture == true)
	{
		specular = u_Light.Specular * texture(u_Material.SpecularTexture, v_TexCoord).xyz * specularIntensity;
	}
	else
	{
		specular = u_Light.Specular * u_Material.Specular * specularIntensity;
	}
	
    
	vec3 finalColor = ambient + diffuse + specular;
	color = vec4(finalColor, max(u_Material.Transparency, 0.0));
}
