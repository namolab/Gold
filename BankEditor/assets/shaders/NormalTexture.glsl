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
uniform mat4 u_Transform;
uniform mat4 u_View;
uniform vec3 u_lightPosition;
uniform float u_FakeLighting;

out vec4 v_Color;
out vec2 v_TexCoord;
out flat float v_TexIndex;
out float v_TilingFactor;
out vec3 v_LightVector;
out vec3 v_ToCameraVector;
out float visibility;
out flat int v_EntityID;


const float density =0.0035;
const float gradient = 5.0;

void main()
{
	v_Color=a_Color;
	v_TexCoord=a_TexCoord;
	v_TexIndex=a_TexIndex;
	v_TilingFactor=a_TilingFactor;

	mat4 modelViewMatrix = u_View* u_Transform;
	vec4 positionRelativeToCam = u_View * vec4(a_Position,1.0);

	vec3 surfaceNormal = (modelViewMatrix * vec4(a_Normal,0.0)).xyz;

	vec3 norm = normalize(surfaceNormal);
	vec3 tang = normalize((modelViewMatrix * vec4(a_Tangent, 0.0)).xyz);
	vec3 bitang = normalize(cross(norm, tang));
	
	mat3 toTangentSpace = mat3(
		tang.x, bitang.x, norm.x,
		tang.y, bitang.y, norm.y,
		tang.z, bitang.z, norm.z
	);

	v_LightVector= toTangentSpace* (u_lightPosition - positionRelativeToCam.xyz);
	v_ToCameraVector = toTangentSpace * (-positionRelativeToCam.xyz);

	float distance = length(positionRelativeToCam.xyz);
	visibility = exp(-pow((distance* density),gradient));
	visibility = clamp(visibility, 0.0, 1.0);

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
in vec3 v_LightVector;
in vec3 v_ToCameraVector;
in float visibility;
in flat int v_EntityID;

uniform sampler2D u_Textures[10];

uniform vec3 u_lightColor;
uniform float u_shineDamper;
uniform float u_reflectivity;
uniform vec3 u_skycolor;

const float levels = 3.0;

void main()
{
	vec4 normalMapValue = 2.0 * texture(u_Textures[6], v_TexCoord, -1.0) - 1.0;
	vec3 unitNormal = normalize(normalMapValue.rgb);
	vec3 unitVectorToCamera = normalize(v_ToCameraVector);

	vec3 unitLightVector = normalize(v_LightVector);

	float nDotl = dot(unitNormal,unitLightVector);
	float brightness = max(nDotl,0.15);
	float level =floor(brightness*levels);
	brightness = level/levels;

	vec3 diffuse = brightness* u_lightColor;

	vec3 lightDirection = -unitLightVector;
	vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);

	float specularFactor = dot(reflectedLightDirection, unitVectorToCamera);
	specularFactor= max(specularFactor,0.0);
	float dampedFactor = pow(specularFactor, u_shineDamper);
	level =floor(dampedFactor*levels);
	dampedFactor = level/levels;

	vec3 finalSpecular = dampedFactor* u_reflectivity* u_lightColor;

	vec4 texColor;
	switch(int(v_TexIndex))
	{
		case 0: texColor = texture(u_Textures[0], v_TexCoord * v_TilingFactor); break;
		case 1: texColor = texture(u_Textures[1], v_TexCoord * v_TilingFactor); break;
		case 2: texColor = texture(u_Textures[2], v_TexCoord * v_TilingFactor); break;
		case 3: texColor = texture(u_Textures[3], v_TexCoord * v_TilingFactor); break;
		case 4: texColor = texture(u_Textures[4], v_TexCoord * v_TilingFactor); break;
		case 5: texColor = texture(u_Textures[5], v_TexCoord * v_TilingFactor); break;
		case 6: texColor = texture(u_Textures[6], v_TexCoord * v_TilingFactor); break;
		case 7: texColor = texture(u_Textures[7], v_TexCoord * v_TilingFactor); break;
		case 8: texColor = texture(u_Textures[8], v_TexCoord * v_TilingFactor); break;
		case 9: texColor = texture(u_Textures[9], v_TexCoord * v_TilingFactor); break;
	}

	if(texColor.a <0.5)
	{
		discard;
	}

	color = vec4(diffuse,1.0)* v_Color* texColor+ vec4(finalSpecular,1.0) ;
	color= mix(vec4(u_skycolor,1.0),color,visibility);

	color2=v_EntityID;
}