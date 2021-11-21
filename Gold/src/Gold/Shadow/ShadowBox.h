#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include "Gold/Renderer/EditorCamera.h"

namespace Gold {

	class ShadowBox 
	{
	public:
		float Offset = 10.0f;
		glm::vec4 Up = { 0.0f, 1.0f, 0.0f, 0.0f};
		glm::vec4 Forward = { 0.0f, 0.0f, -1.0f, 0.0f};
		float Shadow_Distance = 100;

		float minX, maxX;
		float minY, maxY;
		float minZ, maxZ;
		glm::mat4 lightViewMatrix;
		EditorCamera cam;

		float farHeight, farWidth, nearHeight, nearWidth;

		/**
		 * Creates a new shadow box and calculates some initial values relating to
		 * the camera's view frustum, namely the width and height of the near plane
		 * and (possibly adjusted) far plane.
		 *
		 * @param lightViewMatrix
		 *            - basically the "view matrix" of the light. Can be used to
		 *            transform a point from world space into "light" space (i.e.
		 *            changes a point's coordinates from being in relation to the
		 *            world's axis to being in terms of the light's local axis).
		 * @param camera
		 *            - the in-game camera.
		 */
		ShadowBox() = default;
		ShadowBox(glm::mat4 lightViewMatrix, EditorCamera camera);

		/**
		 * Updates the bounds of the shadow box based on the light direction and the
		 * camera's view frustum, to make sure that the box covers the smallest area
		 * possible while still ensuring that everything inside the camera's view
		 * (within a certain range) will cast shadows.
		 */
		void update();

		/**
		 * Calculates the center of the "view cuboid" in light space first, and then
		 * converts this to world space using the inverse light's view matrix.
		 *
		 * @return The center of the "view cuboid" in world space.
		 */
		glm::vec3 getCenter();

		/**
		 * @return The width of the "view cuboid" (orthographic projection area).
		 */
		float getWidth() 
		{
			return maxX - minX;
		}

		/**
		 * @return The height of the "view cuboid" (orthographic projection area).
		 */
		float getHeight() 
		{
			return maxY - minY;
		}

		/**
		 * @return The length of the "view cuboid" (orthographic projection area).
		 */
		float getLength()
		{
			return maxZ - minZ;
		}

		/**
		 * Calculates the position of the vertex at each corner of the view frustum
		 * in light space (8 vertices in total, so this returns 8 positions).
		 *
		 * @param rotation
		 *            - camera's rotation.
		 * @param forwardVector
		 *            - the direction that the camera is aiming, and thus the
		 *            direction of the frustum.
		 * @param centerNear
		 *            - the center point of the frustum's near plane.
		 * @param centerFar
		 *            - the center point of the frustum's (possibly adjusted) far
		 *            plane.
		 * @return The positions of the vertices of the frustum in light space.
		 */
		std::vector<glm::vec4> calculateFrustumVertices(glm::mat4 rotation, glm::vec3 forwardVector, glm::vec3 centerNear, glm::vec3 centerFar);
		

		/**
		 * Calculates one of the corner vertices of the view frustum in world space
		 * and converts it to light space.
		 *
		 * @param startPoint
		 *            - the starting center point on the view frustum.
		 * @param direction
		 *            - the direction of the corner from the start point.
		 * @param width
		 *            - the distance of the corner from the start point.
		 * @return - The relevant corner vertex of the view frustum in light space.
		 */
		glm::vec4 calculateLightSpaceFrustumCorner(glm::vec3 startPoint, glm::vec3 direction, float width);


		/**
		 * @return The rotation of the camera represented as a matrix.
		 */
		glm::mat4 calculateCameraRotationMatrix();


		/**
		 * Calculates the width and height of the near and far planes of the
		 * camera's view frustum. However, this doesn't have to use the "actual" far
		 * plane of the view frustum. It can use a shortened view frustum if desired
		 * by bringing the far-plane closer, which would increase shadow resolution
		 * but means that distant objects wouldn't cast shadows.
		 */
		void calculateWidthsAndHeights();


		/**
		 * @return The aspect ratio of the display (width:height ratio).
		 */
		 float getAspectRatio() 
		 {
			return (float)1280/ (float)720;
		 }
	};
}