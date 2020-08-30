#shader vertex
#version 410 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;
layout (location = 2) in vec3 a_Normal;
layout (location = 3) in vec3 a_Tangent;
layout (location = 4) in vec3 a_Bitangent;

out vec3 v_Position;
out vec2 v_TexCoord;
out vec3 v_Normal;
out mat3 v_TBN;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main()
{
	v_Position = (u_Transform * vec4(a_Position, 1.0)).xyz;
	v_TexCoord = a_TexCoord;

	mat3 normalMatrix = transpose(inverse(mat3(u_Transform)));
	
	vec3 T = normalize(normalMatrix * a_Tangent);
	vec3 B = normalize(normalMatrix * a_Bitangent);
	vec3 N = normalize(normalMatrix * a_Normal);
	v_TBN = mat3(T, B, N);

	v_Normal = normalize(normalMatrix * a_Normal);

	gl_Position = u_ViewProjection * vec4(v_Position, 1.0);
}

#shader fragment
#version 410 core

struct Material
{
	vec3 Albedo;
	float Roughness;
	float Metalness;

	sampler2D AlbedoMap;
	sampler2D NormalMap;
	sampler2D RoughnessMap;
	sampler2D MetalnessMap;
};

struct Light
{
	vec3 Direction;
	vec3 Radiance;
};

const float PI = 3.14159265359;

out vec4 color;

in vec3 v_Position;
in vec2 v_TexCoord;
in vec3 v_Normal;
in mat3 v_TBN;

uniform Light u_Light;
uniform Material u_Material;
uniform vec3 u_CameraPosition;

uniform bool u_UseAlbedoMap;
uniform bool u_UseNormalMap;
uniform bool u_UseRoughnessMap;
uniform bool u_UseMetalnessMap;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}
vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
} 

void main()
{

	vec3 albedo = u_UseAlbedoMap ? texture(u_Material.AlbedoMap, v_TexCoord).rgb : u_Material.Albedo;
	float roughness = u_UseRoughnessMap ? texture(u_Material.RoughnessMap, v_TexCoord).r : u_Material.Roughness;
	float metalness = u_UseMetalnessMap ? texture(u_Material.MetalnessMap, v_TexCoord).r : u_Material.Metalness;

	vec3 normal = normalize(v_Normal);
	if(u_UseNormalMap)
	{
		normal = normalize(texture(u_Material.NormalMap, v_TexCoord).rgb * 2.0 - 1.0);
		normal = normalize(v_TBN * normal);
	}	

	vec3 view = normalize(u_CameraPosition - v_Position);

	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metalness);

	vec3 Lo = vec3(0.0);

	// calculate radiance
	vec3 L = normalize(-u_Light.Direction);
	vec3 H = normalize(view + L);
	//float dist = length(u_Light.Position - v_Position);
	//float attenuation = 1.0 / (dist * dist);
	vec3 radiance = u_Light.Radiance;

	// brdf
	float NDF = DistributionGGX(normal, H, roughness);
	float G = GeometrySmith(normal, view, L, roughness);
	vec3 F = FresnelSchlick(max(dot(H, view), 0.0), F0);

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metalness;

	vec3 numerator = NDF * G * F;
	float denominator = 4.0 * max(dot(normal, view), 0.0) * max(dot(normal, L), 0.0);
	vec3 specular = numerator / max(denominator, 0.001);

	float NdotL = max(dot(normal, L), 0.0);
	Lo += (kD * albedo / PI + specular) * radiance * NdotL;

	vec3 ambient = vec3(0.03) * albedo;
	vec3 finalColor = ambient + Lo;

	finalColor = finalColor / (finalColor + vec3(1.0));
	finalColor = pow(finalColor, vec3(1.0 / 2.2));

	color = vec4(finalColor, 1.0);

}
