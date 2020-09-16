#shader vertex
#version 410 core

layout (location = 0) in vec3 a_Position;

out vec3 v_Position;

uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	v_Position = a_Position;

	mat4 rotationMatrix = mat4(mat3(u_View));

	vec4 clipPos = u_Projection * rotationMatrix * vec4(a_Position, 1.0);

	gl_Position = clipPos.xyww;
}

#shader fragment
#version 410 core

out vec4 color;
in vec3 v_Position;

uniform samplerCube u_SkyBox;
uniform float u_Exposure;


void main()
{
	const float gamma = 2.0;
	vec3 envColor = texture(u_SkyBox, v_Position).rgb;
    
    //envColor = envColor / (envColor + vec3(1.0));
	vec3 mapped = vec3(1.0) - exp(-envColor * u_Exposure);
	mapped = pow(mapped, vec3(1.0 / gamma));
    //envColor = pow(envColor, vec3(1.0/2.2)); 
  
    color = vec4(mapped, 1.0);
}
