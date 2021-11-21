// Basic Texture Shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;
layout(location = 5) in vec3 a_Normal;
layout(location = 6) in int a_EntityID;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform mat4 u_View;
uniform vec3 u_lightPosition;


out vec4 v_Color;
out vec2 v_TexCoord;
out flat float v_TexIndex;
out float v_TilingFactor;
out vec3 v_Normal;
out vec3 v_LightVector;
out vec3 toCameraVector;

out flat int v_EntityID;

void main()
{
	v_Color=a_Color;
	v_TexCoord=a_TexCoord;
	v_TexIndex=a_TexIndex;
	v_TilingFactor=a_TilingFactor;

	v_Normal= (u_Transform* vec4(a_Normal,0.0)).xyz;
	v_LightVector=u_lightPosition - a_Position;
	toCameraVector = (inverse(u_View)* vec4(0.0,0.0,0.0,1.0)).xyz - a_Position.xyz;

	v_EntityID=a_EntityID;
	gl_Position=u_ViewProjection* vec4(a_Position,1.0);
}

#type fragment
#version 450 core

layout(location=0) out vec4 color;
layout(location=1) out int color2;

in vec4 v_Color;
in vec2 v_TexCoord;
in flat float v_TexIndex;
in float v_TilingFactor;
in vec3 v_Normal;
in vec3 v_LightVector;
in vec3 toCameraVector;
in flat int v_EntityID;

uniform sampler2D u_Textures[6];
uniform vec3 u_lightColor;
uniform float u_shineDamper;
uniform float u_reflectivity;

void main()
{
	vec3 unitNormal =normalize(v_Normal);
	vec3 unitLightVector = normalize(v_LightVector);

	float nDotl = dot(unitNormal,unitLightVector);
	float brightness = max(nDotl,0.15);
	vec3 diffuse = brightness* u_lightColor;

	vec3 unitVectorToCamera = normalize(toCameraVector);
	vec3 lightDirection = -unitLightVector;
	vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);

	float specularFactor = dot(reflectedLightDirection, unitVectorToCamera);
	specularFactor= max(specularFactor,0.0);
	float dampedFactor = pow(specularFactor, u_shineDamper);
	vec3 finalSpecular = dampedFactor* u_reflectivity* u_lightColor;

	vec4 texColor = v_Color;
	switch(int(v_TexIndex))
	{
		case 0: texColor *= texture(u_Textures[0], v_TexCoord * v_TilingFactor); break;
		case 1: texColor *= texture(u_Textures[1], v_TexCoord * v_TilingFactor); break;
		case 2: texColor *= texture(u_Textures[2], v_TexCoord * v_TilingFactor); break;
		case 3: texColor *= texture(u_Textures[3], v_TexCoord * v_TilingFactor); break;
		case 4: texColor *= texture(u_Textures[4], v_TexCoord * v_TilingFactor); break;
		case 5: texColor *= texture(u_Textures[5], v_TexCoord * v_TilingFactor); break;
	}
	color = vec4(diffuse,1.0)* texColor+ vec4(finalSpecular,1.0) ;
	color2=v_EntityID;
}