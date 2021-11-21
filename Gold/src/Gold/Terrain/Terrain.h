#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include "Gold/Renderer/Texture.h"

namespace Gold {

	class Terrain
	{
	public:
		Terrain() = default;

		float GetSize()
		{
			return Size;
		}

		void generateTerrain();
		float GetHeight(int x, int z, Ref<Texture2D> image);
		glm::vec3 CalNormal(int x, int z, Ref<Texture2D> image);

	public:
		float Size = 800.0f;
		float Max_Height = 40.0f;
		float Max_PIXEL_COLOR = 256.0f * 256.0f * 256.0f;

		std::vector<glm::vec4> TerrainVertices;
		std::vector<glm::vec2> TerrainTexCoords;
		std::vector<glm::vec3> TerrainNormals;
		std::vector<uint32_t> TerrainIndices;
	};
}
