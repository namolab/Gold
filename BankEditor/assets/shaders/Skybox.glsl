// Basic Skybox Shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in int a_EntityID;


uniform mat4 u_ViewProjection;

out vec3 v_TexCoord;
out flat int v_EntityID;

void main()
{
	v_TexCoord = a_Position;
	v_EntityID=a_EntityID;
	gl_Position=u_ViewProjection* vec4(a_Position,1.0);	
}


#type fragment
#version 450 core

layout(location=0) out vec4 color;
layout(location=1) out int color2;


in vec3 v_TexCoord;
in flat int v_EntityID;

uniform samplerCube u_CubeMap;
uniform samplerCube u_CubeMap2;

uniform float u_blendFactor;
uniform vec3 u_fogColor;

const float lowerLimit = 0.0;
const float upperLimit = 30.0;

const float levels = 10.0;

void main()
{
	vec4 texture1 =texture(u_CubeMap, v_TexCoord);
	vec4 texture2 = texture(u_CubeMap2, v_TexCoord);
    vec4 finalcolor = mix(texture1,texture2,u_blendFactor);


	float amount = (finalcolor.r + finalcolor.g + finalcolor.b) / 3.0;
	amount = floor(amount * levels) /levels;
	finalcolor.rgb = amount * u_fogColor;


	float factor = (v_TexCoord.y - lowerLimit) / (upperLimit - lowerLimit);
	factor = clamp(factor,0.0,1.0);
	color = mix(vec4(u_fogColor,1.0),finalcolor,factor);

	color2 = v_EntityID;
}