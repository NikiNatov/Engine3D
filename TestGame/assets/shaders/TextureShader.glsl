#shader vertex
#version 410 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;

out vec3 v_Position;
out vec2 v_TexCoord;

uniform mat4 u_ViewProjection;

void main()
{
	v_Position = a_Position;
	v_TexCoord = a_TexCoord;

	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#shader fragment
#version 410 core

out vec4 color;

in vec3 v_Position;
in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main()
{
    //color = vec4(v_TexCoord, 0.0, 1.0);
	color = texture(u_Texture, v_TexCoord);
}
