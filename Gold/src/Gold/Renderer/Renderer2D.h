#pragma once

#include "Gold/Renderer/OrthographicCamera.h"

#include "Gold/Renderer/Texture.h"
#include "Gold/Renderer/SubTexture2D.h"

#include "Gold/Renderer/Camera.h"
#include "Gold/Renderer/EditorCamera.h"
#include "Gold/Scene/Components.h"
#include "Gold/Renderer/Light.h"
#include "Gold/Renderer/Shader.h"
#include "Gold/Terrain/Terrain.h"

namespace Gold {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform, const float ts);
		static void BeginScene(const EditorCamera& camera, const float ts);
		static void BeginScene(const OrthographicCamera& camera, const float ts); // TODO: Remove

		static void EndScene();
		static void Flush();
		static void DrawTerrain();
		static void DrawSky();
		static void DrawShadow();
		//Primitives

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		//direction 값에는 (0,0,1} 이면 Z 축 기준으로 회전 {0,1,0} 이면 Y축 기준으로 ...  

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec3& direction, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec3& direction, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec3& direction, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec3& direction, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec3& direction, const Ref<SubTexture2D>& subtexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec3& direction, const Ref<SubTexture2D >& subtexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int entityID = -1);
		static void DrawQuadTerrain(float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int entityID = -1);
		static void DrawQuadSky(float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subtexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int entityID = -1);
		static void DrawQuad(const glm::vec3& position, const std::string& pathString, bool FakeLight = false, int entityID = -1);
		static void DrawQuadNormal(const glm::vec3& position, const std::string& pathString,bool FakeLight = false, int entityID = -1);


		static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID);

		static void Index2DSetting();

		//Stats
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};

		static void ResetStats();
		static Statistics GetStats();

	private:
		static void StartBatch();
		static void NextBatch();

		static void MakeTerrain();
		static void MakeSky();
		static void MakeShadow();

		static void LoadObj(const std::string& pathString, bool NormalFlag);
		static glm::vec3 CalculateTangents(std::vector<glm::vec3> Vertex, std::vector<glm::vec2> TexCoord);

		static void bindTextures(float ts);
	};
}