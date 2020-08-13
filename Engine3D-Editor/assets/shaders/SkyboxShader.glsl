#shader vertex
#version 410 core

layout (location = 0) in vec3 a_Position;

out vec3 v_TexCoord;

uniform mat4 u_ViewProjection;

void main()
{
	v_TexCoord = a_Position;

	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#shader fragment
#version 410 core

out vec4 color;

in vec3 v_TexCoord;

uniform samplerCube u_Skybox;

void main()
{
	color = texture(u_Skybox, v_TexCoord);
}
