#include "hzpch.h"
#include "Terrain.h"

namespace Gold {

	void Terrain::generateTerrain()
	{
		Ref<Texture2D> TerrainHeightTexture = Texture2D::Create("assets/textures/3D/heightmap.png",true, false);
		int Vertex_Count = TerrainHeightTexture.get()->GetHeight();

		int count = Vertex_Count * Vertex_Count;

		TerrainVertices.reserve(count);
		TerrainNormals.reserve(count);
		TerrainTexCoords.reserve(count);
		TerrainIndices.reserve(6 * (Vertex_Count - 1) * (Vertex_Count - 1));

		int vertexPointer = 0;
		for (int i = 0; i < Vertex_Count; i++) 
		{
			for (int j = 0; j < Vertex_Count; j++) 
			{
				TerrainVertices.push_back({ (float)j / ((float)Vertex_Count - 1) * Size ,-GetHeight(j,i,TerrainHeightTexture),(float)i / ((float)Vertex_Count - 1) * Size ,1.0f });
				TerrainNormals.push_back(CalNormal(j, i, TerrainHeightTexture));
				TerrainTexCoords.push_back({ (float)j / ((float)Vertex_Count - 1),(float)i / ((float)Vertex_Count - 1) });
				vertexPointer++;
			}
		}
		int pointer = 0;
		for (int gz = 0; gz < Vertex_Count - 1; gz++) 
		{
			for (int gx = 0; gx < Vertex_Count - 1; gx++) 
			{
				uint32_t topLeft = (gz * Vertex_Count) + gx;
				uint32_t topRight = topLeft + 1;
				uint32_t bottomLeft = ((gz + 1) * Vertex_Count) + gx;
				uint32_t bottomRight = bottomLeft + 1;
				TerrainIndices.push_back(topLeft);
				TerrainIndices.push_back(bottomLeft);
				TerrainIndices.push_back(topRight);
				TerrainIndices.push_back(topRight);
				TerrainIndices.push_back(bottomLeft);
				TerrainIndices.push_back(bottomRight);
			}
		}
	}

	glm::vec3 Terrain::CalNormal(int x, int z, Ref<Texture2D> image)
	{
		float hL = GetHeight(x - 1, z, image);
		float hR = GetHeight(x + 1, z, image);
		float hD = GetHeight(x, z - 1, image);
		float hU = GetHeight(x, z + 1, image);

		glm::vec3 normal( hL - hR, 2.0f, hD - hU);
		normal = glm::normalize(normal);
		return normal;
	}

	float Terrain::GetHeight(int x, int z, Ref<Texture2D> image)
	{
		if (x < 0 || x >= image.get()->GetHeight() || z < 0 || z >= image.get()->GetHeight())
		{
			return 0.0f;
		}
		
		float height = image.get()->GetPixel(x, z);
		height += Max_PIXEL_COLOR / 2.0f;
		height /= Max_PIXEL_COLOR / 2.0f;
		height *= Max_Height;
		return height;
	}
}