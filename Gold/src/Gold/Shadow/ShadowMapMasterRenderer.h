#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include "Gold/Renderer/EditorCamera.h"
#include "ShadowBox.h"
#include "Gold/Renderer/Light.h"

namespace Gold {

	class ShadowMapMasterRenderer
	{

	public: 
		int Shadow_Map_Size = 2048;

		ShadowBox shadowBox;
		glm::mat4 projectionMatrix = glm::mat4(1.0f);
		glm::mat4 lightViewMatrix =  glm::mat4(1.0f);
		glm::mat4 projectionViewMatrix = glm::mat4(1.0f);
		glm::mat4 offset;

		/**
		 * Creates instances of the important objects needed for rendering the scene
		 * to the shadow map. This includes the {@link ShadowBox} which calculates
		 * the position and size of the "view cuboid", the simple renderer and
		 * shader program that are used to render objects to the shadow map, and the
		 * {@link ShadowFrameBuffer} to which the scene is rendered. The size of the
		 * shadow map is determined here.
		 *
		 * @param camera
		 *            - the camera being used in the scene.
		 */

		ShadowMapMasterRenderer() = default;
		ShadowMapMasterRenderer(EditorCamera Camera);
	
		/**
		 * Carries out the shadow render pass. This renders the entities to the
		 * shadow map. First the shadow box is updated to calculate the size and
		 * position of the "view cuboid". The light direction is assumed to be
		 * "-lightPosition" which will be fairly accurate assuming that the light is
		 * very far from the scene. It then prepares to render, renders the entities
		 * to the shadow map, and finishes rendering.
		 *
		 * @param entities
		 *            - the lists of entities to be rendered. Each list is
		 *            associated with the {@link TexturedModel} that all of the
		 *            entities in that list use.
		 * @param sun
		 *            - the light acting as the sun in the scene.
		 */
		void render(Light sun);


		/**
		 * This biased projection-view matrix is used to convert fragments into
		 * "shadow map space" when rendering the main render pass. It converts a
		 * world space position into a 2D coordinate on the shadow map. This is
		 * needed for the second part of shadow mapping.
		 *
		 * @return The to-shadow-map-space matrix.
		 */
		glm::mat4 getToShadowMapSpaceMatrix();


		/**
		 * Clean up the shader and FBO on closing.
		 */


		/**
		 * @return The ID of the shadow map texture. The ID will always stay the
		 *         same, even when the contents of the shadow map texture change
		 *         each frame.
		 */

		/**
		 * @return The light's "view" matrix.
		 */
		glm::mat4 getLightSpaceTransform() {
			return lightViewMatrix;
		}

		/**
		 * Prepare for the shadow render pass. This first updates the dimensions of
		 * the orthographic "view cuboid" based on the information that was
		 * calculated in the {@link SHadowBox} class. The light's "view" matrix is
		 * also calculated based on the light's direction and the center position of
		 * the "view cuboid" which was also calculated in the {@link ShadowBox}
		 * class. These two matrices are multiplied together to create the
		 * projection-view matrix. This matrix determines the size, position, and
		 * orientation of the "view cuboid" in the world. This method also binds the
		 * shadows FBO so that everything rendered after this gets rendered to the
		 * FBO. It also enables depth testing, and clears any data that is in the
		 * FBOs depth attachment from last frame. The simple shader program is also
		 * started.
		 *
		 * @param lightDirection
		 *            - the direction of the light rays coming from the sun.
		 * @param box
		 *            - the shadow box, which contains all the info about the
		 *            "view cuboid".
		 */
		void prepare(glm::vec3 lightDirection, ShadowBox box);

		/**
		 * Finish the shadow render pass. Stops the shader and unbinds the shadow
		 * FBO, so everything rendered after this point is rendered to the screen,
		 * rather than to the shadow FBO.
		 */

		/**
		 * Updates the "view" matrix of the light. This creates a view matrix which
		 * will line up the direction of the "view cuboid" with the direction of the
		 * light. The light itself has no position, so the "view" matrix is centered
		 * at the center of the "view cuboid". The created view matrix determines
		 * where and how the "view cuboid" is positioned in the world. The size of
		 * the view cuboid, however, is determined by the projection matrix.
		 *
		 * @param direction
		 *            - the light direction, and therefore the direction that the
		 *            "view cuboid" should be pointing.
		 * @param center
		 *            - the center of the "view cuboid" in world space.
		 */
		void updateLightViewMatrix(glm::vec3 direction, glm::vec3 center);

		/**
		 * Creates the orthographic projection matrix. This projection matrix
		 * basically sets the width, length and height of the "view cuboid", based
		 * on the values that were calculated in the {@link ShadowBox} class.
		 *
		 * @param width
		 *            - shadow box width.
		 * @param height
		 *            - shadow box height.
		 * @param length
		 *            - shadow box length.
		 */
		void updateOrthoProjectionMatrix(float width, float height, float length);

		/**
		 * Create the offset for part of the conversion to shadow map space. This
		 * conversion is necessary to convert from one coordinate system to the
		 * coordinate system that we can use to sample to shadow map.
		 *
		 * @return The offset as a matrix (so that it's easy to apply to other matrices).
		 */
		glm::mat4 createOffset();
	};
}