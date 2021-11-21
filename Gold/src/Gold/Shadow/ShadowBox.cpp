#include "hzpch.h"
#include "ShadowBox.h"

namespace Gold {

	ShadowBox::ShadowBox(glm::mat4 lightViewMatrix, EditorCamera camera)
	{
		lightViewMatrix = lightViewMatrix;
		cam = camera;
		calculateWidthsAndHeights();
	}
	
	void ShadowBox::update()
	{
		glm::mat4 rotation = calculateCameraRotationMatrix();
		glm::vec3 forwardVector = rotation * Forward;
		glm::vec3 toFar = forwardVector;
		toFar = toFar * Shadow_Distance;
		glm::vec3 toNear = forwardVector;
		toNear = toNear * 0.1f;
		glm::vec3 centerNear = toNear + cam.GetPosition();
		glm::vec3 centerFar = toFar + cam.GetPosition();

		std::vector<glm::vec4> points = calculateFrustumVertices(rotation, forwardVector, centerNear,
			centerFar);
	
		bool first = true;
		for (auto point : points) 
		{
			if (first) {
				minX = point.x;
				maxX = point.x;
				minY = point.y;
				maxY = point.y;
				minZ = point.z;
				maxZ = point.z;
				first = false;
				continue;
			}
			if (point.x > maxX) {
				maxX = point.x;
			}
			else if (point.x < minX) {
				minX = point.x;
			}
			if (point.y > maxY) {
				maxY = point.y;
			}
			else if (point.y < minY) {
				minY = point.y;
			}
			if (point.z > maxZ) {
				maxZ = point.z;
			}
			else if (point.z < minZ) {
				minZ = point.z;
			}
		}
		maxZ += Offset;
	}

	glm::vec3 ShadowBox::getCenter()
	{
		float x = (minX + maxX) / 2.0f;
		float y = (minY + maxY) / 2.0f;
		float z = (minZ + maxZ) / 2.0f;
		glm::vec4 cen = glm::vec4(x, y, z, 1);
		glm::mat4 invertedLight = glm::inverse(lightViewMatrix);

		return invertedLight * cen;
	}


	std::vector<glm::vec4> ShadowBox::calculateFrustumVertices(glm::mat4 rotation, glm::vec3 forwardVector,glm::vec3 centerNear, glm::vec3 centerFar)
	{
		glm::vec3  upVector = rotation * Up;
		glm::vec3  rightVector = glm::cross(forwardVector, upVector);
		glm::vec3  downVector = glm::vec3(-upVector.x, -upVector.y, -upVector.z );
		glm::vec3  leftVector = glm::vec3(-rightVector.x, -rightVector.y, -rightVector.z );
		glm::vec3  farTop = centerFar + glm::vec3(upVector.x* farHeight, upVector.y* farHeight, upVector.z* farHeight);			
		glm::vec3  farBottom = centerFar + glm::vec3(downVector.x * farHeight,downVector.y * farHeight, downVector.z * farHeight);
		glm::vec3  nearTop = centerNear + glm::vec3(upVector.x * nearHeight, upVector.y * nearHeight, upVector.z * nearHeight);
		glm::vec3  nearBottom = centerNear + glm::vec3(downVector.x * nearHeight, downVector.y * nearHeight, downVector.z * nearHeight);
			

		std::vector<glm::vec4> points;
		points.push_back(calculateLightSpaceFrustumCorner(farTop, rightVector, farWidth));
		points.push_back(calculateLightSpaceFrustumCorner(farTop, leftVector, farWidth));
		points.push_back(calculateLightSpaceFrustumCorner(farBottom, rightVector, farWidth));
		points.push_back(calculateLightSpaceFrustumCorner(farBottom, leftVector, farWidth));
		points.push_back(calculateLightSpaceFrustumCorner(nearTop, rightVector, nearWidth));
		points.push_back(calculateLightSpaceFrustumCorner(nearTop, leftVector, nearWidth));
		points.push_back(calculateLightSpaceFrustumCorner(nearBottom, rightVector, nearWidth));
		points.push_back(calculateLightSpaceFrustumCorner(nearBottom, leftVector, nearWidth));
		return points;
	}

	glm::vec4 ShadowBox::calculateLightSpaceFrustumCorner(glm::vec3 startPoint, glm::vec3 direction, float width)
	{
		glm::vec3 point = startPoint + glm::vec3(direction.x * width, direction.y * width, direction.z * width);
		glm::vec4 point4f = glm::vec4(point.x, point.y, point.z, 1.0f);
		point4f = lightViewMatrix * point4f;
		return point4f;
	}

	glm::mat4 ShadowBox::calculateCameraRotationMatrix()
	{
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(cam.GetYaw()), { 0.0f,1.0f,0.0f }) *
			glm::rotate(glm::mat4(1.0f), glm::radians(cam.GetPitch()), { 1.0f,0.0f,0.0f });


		return rotation;
	}

	void ShadowBox::calculateWidthsAndHeights()
	{
		farWidth = (float)(Shadow_Distance* glm::tan(glm::radians(70.0f)));
		nearWidth = (float)(0.1f* glm::tan(glm::radians(70.0f)));
		farHeight = farWidth / getAspectRatio();
		nearHeight = nearWidth / getAspectRatio();
	}

}