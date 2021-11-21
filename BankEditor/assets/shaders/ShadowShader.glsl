// Basic Texture Shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;
layout(location = 5) in vec3 a_Normal;
layout(location = 6) in vec3 a_Tangent;
layout(location = 7) in int a_EntityID;

uniform mat4 u_ViewProjection;

out flat int v_EntityID;

void main()
{

	v_EntityID=a_EntityID;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);

}


#type fragment
#version 450 core

layout(location=0) out vec4 color;
layout(location=1) out int color2;

in flat int v_EntityID;

void main()
{
	color = vec4(1.0);	
	color2=v_EntityID;
}