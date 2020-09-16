#shader vertex
#version 410 core

layout (location = 0) in vec3 a_Position;

out vec3 v_Position;

uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	v_Position = a_Position;

	gl_Position = u_Projection * u_View * vec4(a_Position, 1.0);
}

#shader fragment
#version 410 core

out vec4 color;
in vec3 v_Position;

uniform sampler2D u_HDRMap;

const vec2 invAtan = vec2(0.1591, 0.3183);

vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{
	vec2 texCoord = SampleSphericalMap(normalize(v_Position));
	color = vec4(texture(u_HDRMap, texCoord).rgb, 1.0);
}
