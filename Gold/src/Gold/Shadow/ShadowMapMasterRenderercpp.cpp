#include "hzpch.h"
#include "ShadowMapMasterRenderer.h"

namespace Gold 
{

	ShadowMapMasterRenderer::ShadowMapMasterRenderer(EditorCamera Camera)
	{
		shadowBox = ShadowBox(lightViewMatrix, Camera);
	}

	glm::mat4 ShadowMapMasterRenderer::createOffset()
	{
		glm::mat4 offset = glm::translate(glm::mat4(1.0f), { 0.5f, 0.5f, 0.5f })*
			glm::scale(glm::mat4(1.0f), { 0.5f, 0.5f, 0.5f });

		return offset;
	}

	void ShadowMapMasterRenderer::render(Light sun) 
	{
		shadowBox.update();
		glm::vec3 sunPosition = sun.GetPosition();
		glm::vec3 lightDirection = glm::vec3(-sunPosition.x, -sunPosition.y, -sunPosition.z);
		prepare(lightDirection, shadowBox);
		//finish();
	}

	glm::mat4 ShadowMapMasterRenderer::getToShadowMapSpaceMatrix() 
	{
		return offset * projectionViewMatrix;
	}

	void ShadowMapMasterRenderer::prepare(glm::vec3 lightDirection, ShadowBox box)
	{
		updateOrthoProjectionMatrix(box.getWidth(), box.getHeight(), box.getLength());
		updateLightViewMatrix(lightDirection, box.getCenter());
		projectionMatrix = projectionMatrix * lightViewMatrix;
	}

	void ShadowMapMasterRenderer::updateLightViewMatrix(glm::vec3 direction, glm::vec3 center) 
	{
		direction = glm::normalize(direction);
		center = -center;
		lightViewMatrix = glm::mat4(1.0f);

		float pitch = (float)glm::acos(glm::vec2(direction.x,direction.z ).length());
		lightViewMatrix = glm::rotate(lightViewMatrix, pitch, { 1.0f, 0.0f, 0.0f });
		float yaw = (float)glm::degrees(((float)glm::atan(direction.x / direction.z)));
		yaw = direction.z > 0 ? yaw - 180 : yaw;
		lightViewMatrix = glm::rotate(lightViewMatrix, -glm::radians(yaw), { 0.0f, 1.0f, 0.0f });
		lightViewMatrix = glm::translate(lightViewMatrix, center);
	}

	void ShadowMapMasterRenderer::updateOrthoProjectionMatrix(float width, float height, float length)
	{
		projectionMatrix = glm::mat4(1.0f);
		projectionMatrix[0][0] = 2.0f / width;
		projectionMatrix[1][1] = 2.0f / height;
		projectionMatrix[2][2] = -2.0f / length;
		projectionMatrix[3][3] = 1.0f;
	}
}
