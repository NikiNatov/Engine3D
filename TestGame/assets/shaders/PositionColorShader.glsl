#shader vertex
#version 410 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Color;

out vec3 v_Position;
out vec3 v_Color;

uniform mat4 u_ViewProjection;

void main()
{
	v_Position = a_Position;
	v_Color = a_Color;

	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#shader fragment
#version 410 core

out vec4 color;

in vec3 v_Position;
in vec3 v_Color;

void main()
{
	//color = vec4(v_Position * 0.5 + 0.5, 1.0);
	color = vec4(v_Color, 1.0);
}
