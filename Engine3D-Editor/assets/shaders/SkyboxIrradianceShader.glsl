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

uniform samplerCube u_SkyBox;

const float PI = 3.14159265359;

void main()
{
	vec3 normal = normalize(v_Position);
	vec3 irradiance = vec3(0.0);
    
	vec3 up    = vec3(0.0, 1.0, 0.0);
	vec3 right = cross(up, normal);
	up         = cross(normal, right);

	float sampleDelta = 0.025;
	float nrSamples = 0.0; 
	for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
	{
		for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
		{
			// spherical to cartesian (in tangent space)
			vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
			// tangent space to world
			vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal; 

			irradiance += texture(u_SkyBox, sampleVec).rgb * cos(theta) * sin(theta);
			nrSamples++;
		}
	}
	irradiance = PI * irradiance * (1.0 / float(nrSamples));

    color = vec4(irradiance, 1.0);
}
