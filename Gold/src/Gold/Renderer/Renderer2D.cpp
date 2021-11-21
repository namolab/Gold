#include "hzpch.h"
#include "Gold/Renderer/Renderer2D.h"

#include "Gold/Renderer/VertexArray.h"
#include "Gold/Renderer/Shader.h"
#include "Gold/Renderer/RenderCommand.h"
#include "Gold/Shadow/ShadowBox.h"
#include "Gold/Shadow/ShadowMapMasterRenderer.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glad/glad.h>
namespace Gold {

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
		glm::vec3 Normal;
		glm::vec3 Tangent;
		int EntityID;
	};

	struct SkyVertex
	{
		glm::vec3 Position;
		int EntityID;
	};

	struct ShadowVertex
	{
		glm::vec3 Position;
		int EntityID;
	};


	struct QuadIndex
	{
		uint32_t Index;
	};

	struct Renderer2DData
	{
		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;

		static const uint32_t MaxTextureSlots = 10; // TODO: RenderCaps

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<IndexBuffer> QuadIndexBuffer;

		Ref<VertexArray> TerrainVertexArray;
		Ref<VertexBuffer> TerrainVertexBuffer;
		Ref<IndexBuffer> TerrainIndexBuffer;

		Ref<VertexArray> SkyVertexArray;
		Ref<VertexBuffer> SkyVertexBuffer;

		Ref<VertexArray> ShadowVertexArray;
		Ref<VertexBuffer> ShadowVertexBuffer;
		Ref<IndexBuffer> ShadowIndexBuffer;

		Ref<Texture2D> WhiteTexture;
		Ref<Texture2D> TerrainTexture;
		Ref<Texture2D> SkyTexture;
		Ref<Texture2D> SkyTexture2;


		Ref<Texture2D> BaseTextrue;
		Ref<Texture2D> NormalTextrue;

		Ref<Texture2D> ShadowTextrue;

		uint32_t QuadIndexCount = 0;
		uint32_t offset = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		QuadVertex* TerrainVertexBufferBase = nullptr;
		QuadVertex* TerrainVertexBufferPtr = nullptr;

		SkyVertex* SkyVertexBufferBase = nullptr;
		SkyVertex* SkyVertexBufferPtr = nullptr;

		ShadowVertex* ShadowVertexBufferBase = nullptr;
		ShadowVertex* ShadowVertexBufferPtr = nullptr;


		QuadIndex* QuadIndexBufferBase = nullptr;
		QuadIndex* QuadIndexBufferPtr = nullptr;

		QuadIndex* TerrainIndexBufferBase = nullptr;
		QuadIndex* TerrainIndexBufferPtr = nullptr;

		QuadIndex* ShadowIndexBufferBase = nullptr;
		QuadIndex* ShadowIndexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 4; // 0 = white texture  1 = Terrain texture  2, 3 SKy 5 Shadow

		glm::vec4 QuadVertexPositions[4];
		glm::vec3 Normal2DVector;
		Renderer2D::Statistics Stats;

		ShaderLibrary m_ShaderLibrary;

		ShadowMapMasterRenderer ShadowRenderer;

		float SkyRotation = 0.0f;
		int time = 0.0f;
		float blendfactor;
	};

	struct VertexInfo
	{
		std::vector<glm::vec3> Tangents;
		glm::vec3 averageTangent = glm::vec3(0.0f);

		void addTangent(glm::vec3 tangent)
		{
			Tangents.push_back(tangent);
		}

		void CalaverageTangents() 
		{
			if (Tangents.empty()) 
			{
				return;
			}
			for (auto tangent : Tangents)
			{
				averageTangent += tangent;
			}
			averageTangent = glm::normalize(averageTangent);
		}
	};

	struct Mode3Dlnfo
	{
		std::vector<glm::vec4> Vertex;
		std::vector<uint32_t> Index;
		std::vector<glm::vec2> TextureCoords;
		std::vector<glm::vec3> Normals;
		std::vector<glm::vec3> Tangents;
		glm::mat4 ModelTransform;
	};

	std::vector<std::string> SkyFaces
	{
		"assets/textures/3D/Skybox_Images/0.jpg",
		"assets/textures/3D/Skybox_Images/1.jpg",
		"assets/textures/3D/Skybox_Images/2.jpg",
		"assets/textures/3D/Skybox_Images/3.jpg",
		"assets/textures/3D/Skybox_Images/4.jpg",
		"assets/textures/3D/Skybox_Images/5.jpg"
	};

	std::vector<std::string> SkyFaces2
	{
		"assets/textures/3D/Night_Images/0.png",
		"assets/textures/3D/Night_Images/1.png",
		"assets/textures/3D/Night_Images/2.png",
		"assets/textures/3D/Night_Images/3.png",
		"assets/textures/3D/Night_Images/4.png",
		"assets/textures/3D/Night_Images/5.png"
	};

	std::vector<glm::vec3> SkyBoxVertexs
	{
		{-500.0f,  500.0f, -500.0f},
		{-500.0f, -500.0f, -500.0f},
		{ 500.0f, -500.0f, -500.0f},
		{ 500.0f, -500.0f, -500.0f},
		{ 500.0f,  500.0f, -500.0f},
		{-500.0f,  500.0f, -500.0f},

		{-500.0f, -500.0f,  500.0f},
		{-500.0f, -500.0f, -500.0f},
		{-500.0f,  500.0f, -500.0f},
		{-500.0f,  500.0f, -500.0f},
		{-500.0f,  500.0f,  500.0f},
		{-500.0f, -500.0f,  500.0f},

		{ 500.0f, -500.0f, -500.0f},
		{ 500.0f, -500.0f,  500.0f},
		{ 500.0f,  500.0f,  500.0f},
		{ 500.0f,  500.0f,  500.0f},
		{ 500.0f,  500.0f, -500.0f},
		{ 500.0f, -500.0f, -500.0f},

		{-500.0f, -500.0f,  500.0f},
		{-500.0f,  500.0f,  500.0f},
		{ 500.0f,  500.0f,  500.0f},
		{ 500.0f,  500.0f,  500.0f},
		{ 500.0f, -500.0f,  500.0f},
		{-500.0f, -500.0f,  500.0f},

		{-500.0f,  500.0f, -500.0f},
		{ 500.0f,  500.0f, -500.0f},
		{ 500.0f,  500.0f,  500.0f},
		{ 500.0f,  500.0f,  500.0f},
		{-500.0f,  500.0f,  500.0f},
		{-500.0f,  500.0f, -500.0f},

		{-500.0f, -500.0f, -500.0f},
		{-500.0f, -500.0f,  500.0f},
		{ 500.0f, -500.0f, -500.0f},
		{ 500.0f, -500.0f, -500.0f},
		{-500.0f, -500.0f,  500.0f},
		{ 500.0f, -500.0f,  500.0f}
	};

	static Renderer2DData s_Data;
	static std::unordered_map<std::string,Mode3Dlnfo> s_Model3DInfo;
	Light Sun;
	Terrain BottomTerrain;
	EditorCamera MainRendercamera;
	std::vector<std::string> ShadowList;

	glm::mat4 lightViewMatrix = glm::mat4(1.0f);

	void Renderer2D::Init()
	{
		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));

		s_Data.QuadVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"},
			{ShaderDataType::Float2, "a_TexCoord"},
			{ShaderDataType::Float, "a_TexIndex"},
			{ShaderDataType::Float, "a_TilingFactor"},
			{ShaderDataType::Float3, "a_Normal"},
			{ShaderDataType::Float3, "a_Tangent"},
			{ShaderDataType::Int,	"a_EntityID"}
		});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		s_Data.QuadIndexBuffer = IndexBuffer::Create(s_Data.MaxIndices);

		s_Data.QuadVertexArray->SetIndexBuffer(s_Data.QuadIndexBuffer);
		s_Data.QuadIndexBufferBase = new QuadIndex[s_Data.MaxIndices];

		s_Data.WhiteTexture = Texture2D::Create(1, 1,false);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		//////////////////////////////////Edit Program//////////////////////
		auto TextureShader = s_Data.m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
		s_Data.m_ShaderLibrary.Load("assets/shaders/Skybox.glsl");
		auto TerrainShader = s_Data.m_ShaderLibrary.Load("assets/shaders/Terrain.glsl");
		auto TextureNormalShader = s_Data.m_ShaderLibrary.Load("assets/shaders/NormalTexture.glsl");
		//s_Data.m_ShaderLibrary.Load("assets/shaders/ShadowShader.glsl");
		////////////////////////////////////////////////////////////////////

		TextureShader->Bind();
		TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		TerrainShader->Bind();
		TerrainShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		TextureNormalShader->Bind();
		TextureNormalShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		// Set first texture slot to 0
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };


		glm::vec3 QuadVertexPositions3Vector = { -0.5f, -0.5f, 0.0f };
		glm::vec3 QuadVertexPositions3Vector2 = { 0.5f, -0.5f, 0.0f };
		glm::vec3 QuadVertexPositions3Vector3 = { 0.5f,  0.5f, 0.0f };

		s_Data.Normal2DVector= -glm::cross( (QuadVertexPositions3Vector2 - QuadVertexPositions3Vector), (QuadVertexPositions3Vector3 - QuadVertexPositions3Vector));


		Sun.SetPosition({ 1000000.0f,1500000.0f,1000000.0f });
		//Sun.SetPosition({ 0.0f,0.0f,-20.0f });
		Sun.SetColor({ 1.3f,1.3f,1.3f });

		s_Data.m_ShaderLibrary.Get("Texture")->Bind();
		s_Data.m_ShaderLibrary.Get("Texture")->SetFloat3("u_lightPosition", Sun.GetPosition());
		s_Data.m_ShaderLibrary.Get("Texture")->SetFloat3("u_lightColor", Sun.GetColor());
		s_Data.m_ShaderLibrary.Get("Texture")->SetFloat("u_shineDamper", 10);
		s_Data.m_ShaderLibrary.Get("Texture")->SetFloat("u_reflectivity", 1);
		s_Data.m_ShaderLibrary.Get("Texture")->SetFloat3("u_skycolor", { 0.5444f, 0.62f, 0.69f });

		s_Data.m_ShaderLibrary.Get("Terrain")->Bind();
		s_Data.m_ShaderLibrary.Get("Terrain")->SetFloat3("u_lightPosition", Sun.GetPosition());
		s_Data.m_ShaderLibrary.Get("Terrain")->SetFloat3("u_lightColor", Sun.GetColor());
		s_Data.m_ShaderLibrary.Get("Terrain")->SetFloat("u_shineDamper", 10);
		s_Data.m_ShaderLibrary.Get("Terrain")->SetFloat("u_reflectivity", 1);
		s_Data.m_ShaderLibrary.Get("Terrain")->SetFloat3("u_skycolor", { 0.5444f, 0.62f, 0.69f });

		s_Data.m_ShaderLibrary.Get("NormalTexture")->Bind();
		s_Data.m_ShaderLibrary.Get("NormalTexture")->SetFloat3("u_lightPosition", Sun.GetPosition());
		s_Data.m_ShaderLibrary.Get("NormalTexture")->SetFloat3("u_lightColor", Sun.GetColor());
		s_Data.m_ShaderLibrary.Get("NormalTexture")->SetFloat("u_shineDamper", 10);
		s_Data.m_ShaderLibrary.Get("NormalTexture")->SetFloat("u_reflectivity", 0.5);
		s_Data.m_ShaderLibrary.Get("NormalTexture")->SetFloat3("u_skycolor", { 0.5444f, 0.62f, 0.69f });

		s_Data.TerrainTexture = Texture2D::Create("assets/textures/3D/grass.png", false, false);
		s_Data.TextureSlots[1] = s_Data.TerrainTexture;

		s_Data.SkyTexture = Texture2D::Create(SkyFaces);
		s_Data.TextureSlots[2] = s_Data.SkyTexture;

		s_Data.SkyTexture2 = Texture2D::Create(SkyFaces2);
		s_Data.TextureSlots[3] = s_Data.SkyTexture2;

		//s_Data.ShadowTextrue = Texture2D::Create(2048, 2048,true);
		//s_Data.TextureSlots[4] = s_Data.ShadowTextrue;

		s_Data.BaseTextrue = Texture2D::Create("assets/textures/3D/barrel.png", false, false);
		s_Data.NormalTextrue = Texture2D::Create("assets/textures/3D/barrelNormal.png",false, false);

		s_Data.m_ShaderLibrary.Get("Skybox")->Bind();
		s_Data.m_ShaderLibrary.Get("Skybox")->SetInt("u_CubeMap", 2);
		s_Data.m_ShaderLibrary.Get("Skybox")->SetInt("u_CubeMap2", 3);
		s_Data.m_ShaderLibrary.Get("Skybox")->SetFloat3("u_fogColor", { 0.5444f, 0.62f, 0.69f });

		//s_Data.ShadowRenderer = ShadowMapMasterRenderer(MainRendercamera);

		MakeTerrain();
		MakeSky(); 
		//MakeShadow();
	}

	void Renderer2D::Shutdown()
	{
		delete[] s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const EditorCamera& camera, const float ts)
	{
		glm::mat4 viewProj = camera.GetViewProjection();
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 Proj = camera.GetProjection();

		s_Data.m_ShaderLibrary.Get("Texture")->Bind();
		s_Data.m_ShaderLibrary.Get("Texture")->SetMat4("u_ViewProjection", viewProj);
		s_Data.m_ShaderLibrary.Get("Texture")->SetMat4("u_View", view);

		s_Data.m_ShaderLibrary.Get("Terrain")->Bind();
		s_Data.m_ShaderLibrary.Get("Terrain")->SetMat4("u_ViewProjection", viewProj);
		s_Data.m_ShaderLibrary.Get("Terrain")->SetMat4("u_View", view);

		s_Data.m_ShaderLibrary.Get("NormalTexture")->Bind();
		s_Data.m_ShaderLibrary.Get("NormalTexture")->SetMat4("u_ViewProjection", viewProj);
		s_Data.m_ShaderLibrary.Get("NormalTexture")->SetMat4("u_View", view);

		view[3][0] = 0;
		view[3][1] = 0;
		view[3][2] = 0;

		s_Data.SkyRotation += 1.0f * ts;
		view = glm::rotate(view, glm::radians(s_Data.SkyRotation), { 0.0f, 1.0f, 0.0f });

		viewProj = Proj * view;

		s_Data.m_ShaderLibrary.Get("Skybox")->Bind();
		s_Data.m_ShaderLibrary.Get("Skybox")->SetMat4("u_ViewProjection", viewProj);

		bindTextures(ts);
		s_Data.m_ShaderLibrary.Get("Skybox")->SetFloat("u_blendFactor", s_Data.blendfactor);

		MainRendercamera = camera;
		StartBatch();
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform, const float ts)
	{
		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);
		glm::mat4 view = glm::inverse(transform);
		glm::mat4 Proj = camera.GetProjection();

		s_Data.m_ShaderLibrary.Get("Texture")->Bind();
		s_Data.m_ShaderLibrary.Get("Texture")->SetMat4("u_ViewProjection", viewProj);
		s_Data.m_ShaderLibrary.Get("Texture")->SetMat4("u_View", glm::inverse(transform));

		s_Data.m_ShaderLibrary.Get("Terrain")->Bind();
		s_Data.m_ShaderLibrary.Get("Terrain")->SetMat4("u_ViewProjection", viewProj);
		s_Data.m_ShaderLibrary.Get("Terrain")->SetMat4("u_View", glm::inverse(transform));

		s_Data.m_ShaderLibrary.Get("NormalTexture")->Bind();
		s_Data.m_ShaderLibrary.Get("NormalTexture")->SetMat4("u_ViewProjection", viewProj);
		s_Data.m_ShaderLibrary.Get("NormalTexture")->SetMat4("u_View", glm::inverse(transform));

		view[3][0] = 0;
		view[3][1] = 0;
		view[3][2] = 0;

		s_Data.SkyRotation += 1.0f * ts;
		view = glm::rotate(view, glm::radians(s_Data.SkyRotation), { 0.0f, 1.0f, 0.0f });

		viewProj = Proj * view;

		s_Data.m_ShaderLibrary.Get("Skybox")->Bind();
		s_Data.m_ShaderLibrary.Get("Skybox")->SetMat4("u_ViewProjection", viewProj);

		bindTextures(ts);
		s_Data.m_ShaderLibrary.Get("Skybox")->SetFloat("u_blendFactor", s_Data.blendfactor);

		StartBatch();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera, const float ts)
	{
		glm::mat4 viewProj = camera.GetViewProjectionMatrix();
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 Proj = camera.GetProjectionMatrix();

		s_Data.m_ShaderLibrary.Get("Texture")->Bind();
		s_Data.m_ShaderLibrary.Get("Texture")->SetMat4("u_ViewProjection", viewProj);
		s_Data.m_ShaderLibrary.Get("Texture")->SetMat4("u_View", view);


		s_Data.m_ShaderLibrary.Get("Terrain")->Bind();
		s_Data.m_ShaderLibrary.Get("Terrain")->SetMat4("u_ViewProjection", viewProj);
		s_Data.m_ShaderLibrary.Get("Terrain")->SetMat4("u_View", view);



		s_Data.m_ShaderLibrary.Get("NormalTexture")->Bind();
		s_Data.m_ShaderLibrary.Get("NormalTexture")->SetMat4("u_ViewProjection", viewProj);
		s_Data.m_ShaderLibrary.Get("NormalTexture")->SetMat4("u_View", view);

		view[3][0] = 0;
		view[3][1] = 0;
		view[3][2] = 0;

		s_Data.SkyRotation += 1.0f * ts;
		view = glm::rotate(view, glm::radians(s_Data.SkyRotation), { 0.0f, 1.0f, 0.0f });

		viewProj = Proj * view;

		s_Data.m_ShaderLibrary.Get("Skybox")->Bind();
		s_Data.m_ShaderLibrary.Get("Skybox")->SetMat4("u_ViewProjection", viewProj);

		bindTextures(ts);
		s_Data.m_ShaderLibrary.Get("Skybox")->SetFloat("u_blendFactor", s_Data.blendfactor);

		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		Flush();
	}

	void Renderer2D::Flush()
	{
		if (s_Data.QuadIndexCount == 0)
			return; // Nothing to draw

		s_Data.QuadVertexArray->Bind();

		uint32_t VertexdataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, VertexdataSize);

		uint32_t IndexdataSize = (uint8_t*)s_Data.QuadIndexBufferPtr - (uint8_t*)s_Data.QuadIndexBufferBase;
		s_Data.QuadIndexBuffer->SetData(s_Data.QuadIndexBufferBase, IndexdataSize);

		// Bind textures
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);

		s_Data.Stats.DrawCalls++;

		//DrawShadow();
	}

	void Renderer2D::DrawTerrain()
	{
		s_Data.TerrainVertexArray->Bind();
		s_Data.TerrainVertexBuffer->Bind();

		uint32_t VertexdataSize = (uint8_t*)s_Data.TerrainVertexBufferPtr - (uint8_t*)s_Data.TerrainVertexBufferBase;
		s_Data.TerrainVertexBuffer->SetData(s_Data.TerrainVertexBufferBase, VertexdataSize);

		uint32_t IndexdataSize = (uint8_t*)s_Data.TerrainIndexBufferPtr - (uint8_t*)s_Data.TerrainIndexBufferBase;
		s_Data.TerrainIndexBuffer->SetData(s_Data.TerrainIndexBufferBase, IndexdataSize);

		s_Data.TextureSlots[1]->Bind(1);

		RenderCommand::DrawIndexed(s_Data.TerrainVertexArray, BottomTerrain.TerrainIndices.size());
	}

	void Renderer2D::DrawSky()
	{
		s_Data.SkyVertexArray->Bind();
		s_Data.SkyVertexBuffer->Bind();

		uint32_t VertexdataSize = (uint8_t*)s_Data.SkyVertexBufferPtr - (uint8_t*)s_Data.SkyVertexBufferBase;
		s_Data.SkyVertexBuffer->SetData(s_Data.SkyVertexBufferBase, VertexdataSize);

		s_Data.TextureSlots[2]->Bind(2);
		s_Data.TextureSlots[3]->Bind(3);

		RenderCommand::DrawIndexedSky(36);
	}

	void Renderer2D::DrawShadow()
	{
		/*s_Data.TextureSlots[4]->unbindFramebuffer();
		s_Data.TextureSlots[4]->resetviewport();

		s_Data.QuadVertexArray->ShadowBind();*/

		glm::mat4 viewProj = MainRendercamera.GetViewProjection();

		s_Data.m_ShaderLibrary.Get("ShadowShader")->Bind();
		s_Data.m_ShaderLibrary.Get("ShadowShader")->SetMat4("u_ViewProjection", viewProj);

		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);


		/*s_Data.ShadowVertexArray->Bind();
		s_Data.ShadowVertexBuffer->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { 0.0f,0.0f,0.0f });

		s_Data.m_ShaderLibrary.Get("ShadowShader")->Bind();
		s_Data.m_ShaderLibrary.Get("ShadowShader")->SetMat4("u_ViewProjection", glm::mat4(1.0f));

		for (auto Paths : ShadowList)
		{
			for (int i = 0; i < s_Model3DInfo[Paths].Vertex.size(); i++)
			{
				s_Data.ShadowVertexBufferPtr->Position = transform * s_Model3DInfo[Paths].Vertex[i];
				s_Data.ShadowVertexBufferPtr->EntityID = -1;
				s_Data.ShadowVertexBufferPtr++;
			}

			for (int i = 0; i < s_Model3DInfo[Paths].Index.size(); i++)
			{
				s_Data.ShadowIndexBufferPtr->Index = s_Model3DInfo[Paths].Index[i];
				s_Data.ShadowIndexBufferPtr++;
			}

			uint32_t VertexdataSize = (uint8_t*)s_Data.ShadowVertexBufferPtr - (uint8_t*)s_Data.ShadowVertexBufferBase;
			s_Data.ShadowVertexBuffer->SetData(s_Data.ShadowVertexBufferBase, VertexdataSize);

			uint32_t IndexdataSize = (uint8_t*)s_Data.ShadowIndexBufferPtr - (uint8_t*)s_Data.ShadowIndexBufferBase;
			s_Data.ShadowIndexBuffer->SetData(s_Data.ShadowIndexBufferBase, IndexdataSize);

			s_Data.TextureSlots[4]->Bind(4);

			RenderCommand::DrawIndexed(s_Data.ShadowVertexArray, s_Model3DInfo[Paths].Index.size());

			s_Data.ShadowVertexBufferPtr = s_Data.ShadowVertexBufferBase;
			s_Data.ShadowIndexBufferPtr = s_Data.ShadowIndexBufferBase;
		}*/



		//s_Data.m_ShaderLibrary.Get("ShadowShader")->Bind();
		//s_Data.m_ShaderLibrary.Get("ShadowShader")->SetMat4("u_ViewProjection", glm::mat4(1.0f));

		//s_Data.QuadVertexBuffer->SetLayout({
		//	{ShaderDataType::Float3, "a_Position"}
		//	});
		//s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer,true);

		//uint32_t VertexdataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		//s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, VertexdataSize);


		//// Bind textures
		//s_Data.TextureSlots[4]->Bind(4);

		//RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, subtexture, tilingFactor, tintColor);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float tilingFactor, const glm::vec4& tintColor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, subtexture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec3& direction, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, direction, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec3& direction, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), direction)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec3& direction, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, direction, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec3& direction, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), direction)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec3& direction, const Ref<SubTexture2D>& subtexture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, direction, subtexture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec3& direction, const Ref<SubTexture2D>& subtexture, float tilingFactor, const glm::vec4& tintColor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), direction)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, subtexture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{

		constexpr size_t quadVertexCount = 4;
		const float textureIndex = 0.0f; // White Texture
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		const float tilingFactor = 1.0f;


		s_Data.m_ShaderLibrary.Get("Texture")->Bind();
		s_Data.m_ShaderLibrary.Get("Texture")->SetMat4("u_Transform", transform);
		s_Data.m_ShaderLibrary.Get("Texture")->SetFloat("u_FakeLighting", false);

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform*s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->Normal = s_Data.Normal2DVector;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		Index2DSetting();

		s_Data.Stats.QuadCount++;

		NextBatch();
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor, int entityID)
	{

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		s_Data.m_ShaderLibrary.Get("Texture")->Bind();
		s_Data.m_ShaderLibrary.Get("Texture")->SetMat4("u_Transform", transform);
		s_Data.m_ShaderLibrary.Get("Texture")->SetFloat("u_FakeLighting", false);

		float textureIndex;

		textureIndex = (float)s_Data.TextureSlotIndex;
		s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
		s_Data.TextureSlotIndex++;
		
		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform*s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->Normal = s_Data.Normal2DVector;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		Index2DSetting();

		s_Data.Stats.QuadCount++;
	
		NextBatch();
	}

	void Renderer2D::DrawQuadTerrain(float tilingFactor, const glm::vec4& tintColor, int entityID)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { 0.0f,0.0f,0.0f});

		s_Data.m_ShaderLibrary.Get("Terrain")->Bind();
		s_Data.m_ShaderLibrary.Get("Terrain")->SetMat4("u_Transform", transform);
		s_Data.m_ShaderLibrary.Get("Terrain")->SetFloat("u_FakeLighting", false);

		DrawTerrain();
	}

	void Renderer2D::DrawQuadSky(float tilingFactor, const glm::vec4& tintColor, int entityID)
	{
		s_Data.m_ShaderLibrary.Get("Skybox")->Bind();
		DrawSky();	
	}
	
	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subtexture, float tilingFactor, const glm::vec4& tintColor, int entityID)
	{

		constexpr size_t quadVertexCount = 4;
		const glm::vec2* textureCoords = subtexture->GetTexCoords();
		const Ref<Texture2D> texture = subtexture->GetTexture();

		s_Data.m_ShaderLibrary.Get("Texture")->Bind();
		s_Data.m_ShaderLibrary.Get("Texture")->SetMat4("u_Transform", transform);
		s_Data.m_ShaderLibrary.Get("Texture")->SetFloat("u_FakeLighting", false);

		float textureIndex;

		textureIndex = (float)s_Data.TextureSlotIndex;
		s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
		s_Data.TextureSlotIndex++;

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform*s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->Normal = s_Data.Normal2DVector;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		Index2DSetting();
		
		s_Data.Stats.QuadCount++;

		NextBatch();
	}


	void Renderer2D::DrawQuad(const glm::vec3& position, const std::string& pathString, bool FakeLight, int entityID)
	{
		//s_Data.TextureSlots[4]->renderdepth();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);

		s_Data.m_ShaderLibrary.Get("Texture")->Bind();
		s_Data.m_ShaderLibrary.Get("Texture")->SetMat4("u_Transform", transform);
		s_Data.m_ShaderLibrary.Get("Texture")->SetFloat("u_FakeLighting", FakeLight);

		LoadObj(pathString, false);

		s_Model3DInfo[pathString].ModelTransform = transform;

		float textureIndex = 0.0f;
		const float tilingFactor = 1.0f;

		textureIndex = (float)s_Data.TextureSlotIndex;
		s_Data.TextureSlots[s_Data.TextureSlotIndex] = s_Data.BaseTextrue;
		s_Data.TextureSlotIndex++;

		for (size_t i = 0; i < s_Model3DInfo[pathString].Vertex.size(); i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform* s_Model3DInfo[pathString].Vertex[i];
			s_Data.QuadVertexBufferPtr->Color = glm::vec4(1.0f);
			s_Data.QuadVertexBufferPtr->TexCoord = s_Model3DInfo[pathString].TextureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->Normal = s_Model3DInfo[pathString].Normals[i];
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		for (size_t i = 0; i < s_Model3DInfo[pathString].Index.size(); i++)
		{
			s_Data.QuadIndexBufferPtr->Index = s_Model3DInfo[pathString].Index[i];
			s_Data.QuadIndexBufferPtr++;
		}

		s_Data.QuadIndexCount += s_Model3DInfo[pathString].Index.size();

		NextBatch();
	}

	void Renderer2D::DrawQuadNormal(const glm::vec3& position, const std::string& pathString, bool FakeLight, int entityID)
	{

		//s_Data.TextureSlots[4]->renderdepth();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);

		s_Data.m_ShaderLibrary.Get("NormalTexture")->Bind();
		s_Data.m_ShaderLibrary.Get("NormalTexture")->SetMat4("u_Transform", transform);
		s_Data.m_ShaderLibrary.Get("NormalTexture")->SetFloat("u_FakeLighting", FakeLight);

		LoadObj(pathString, true);

		s_Model3DInfo[pathString].ModelTransform = transform;

		float textureIndex = 0.0f;
		const float tilingFactor = 1.0f;

		textureIndex = (float)s_Data.TextureSlotIndex;
		s_Data.TextureSlots[s_Data.TextureSlotIndex] = s_Data.BaseTextrue;
		s_Data.TextureSlotIndex++;

		s_Data.TextureSlots[s_Data.TextureSlotIndex] = s_Data.NormalTextrue;
		s_Data.TextureSlotIndex++;

		for (size_t i = 0; i < s_Model3DInfo[pathString].Vertex.size(); i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Model3DInfo[pathString].Vertex[i];
			s_Data.QuadVertexBufferPtr->Color = glm::vec4(1.0f);
			s_Data.QuadVertexBufferPtr->TexCoord = s_Model3DInfo[pathString].TextureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->Normal = s_Model3DInfo[pathString].Normals[i];
			s_Data.QuadVertexBufferPtr->Tangent = s_Model3DInfo[pathString].Tangents[i];
			s_Data.QuadVertexBufferPtr->EntityID = -1;
			s_Data.QuadVertexBufferPtr++;
		}

		for (size_t i = 0; i < s_Model3DInfo[pathString].Index.size(); i++)
		{
			s_Data.QuadIndexBufferPtr->Index = s_Model3DInfo[pathString].Index[i];
			s_Data.QuadIndexBufferPtr++;
		}

		s_Data.QuadIndexCount += s_Model3DInfo[pathString].Index.size();

		NextBatch();
	}

	void Renderer2D::LoadObj(const std::string& pathString, bool NormalFlag)
	{
		if (s_Model3DInfo.count(pathString) == 0)
		{
			Assimp::Importer imp;
			const auto pScene = imp.ReadFile(pathString.c_str(),
				aiProcess_Triangulate |
				aiProcess_JoinIdenticalVertices |
				aiProcess_ConvertToLeftHanded |
				aiProcess_GenNormals |
				aiProcess_GenSmoothNormals |
				aiProcess_CalcTangentSpace
			);
			if (pScene == nullptr)
			{
				GD_CORE_ASSERT(false, "Load 3D Model Error");
			}

			const auto pMesh = pScene->mMeshes[0];

			std::vector<glm::vec4> TempVertex;
			std::vector<uint32_t> TempIndex;
			std::vector<glm::vec2> TempTexCoords;
			std::vector<glm::vec3> Normals;
			std::vector<glm::vec3> Tangents;
			std::vector<VertexInfo> InfoVertex;

			InfoVertex.resize(pMesh->mNumVertices);

			for (size_t i = 0; i < pMesh->mNumVertices; i++)
			{
				glm::vec3 tempVertex;
				tempVertex = (*reinterpret_cast<glm::vec3*>(&pMesh->mVertices[i]));
				TempVertex.push_back({ tempVertex.x, tempVertex.y,tempVertex.z, 1.0f });
				TempTexCoords.push_back(*reinterpret_cast<glm::vec2*>(&pMesh->mTextureCoords[0][i]));
				Normals.push_back(*reinterpret_cast<glm::vec3*>(&pMesh->mNormals[i]));
			}

			for (size_t i = 0; i < pMesh->mNumFaces; i++)
			{
				const auto& face = pMesh->mFaces[i];
				TempIndex.push_back(face.mIndices[0]);
				TempIndex.push_back(face.mIndices[1]);
				TempIndex.push_back(face.mIndices[2]);

				if (NormalFlag)
				{
					std::vector<glm::vec3> CalVertex;
					std::vector<glm::vec2> CalTexCoords;

					CalVertex.push_back({ TempVertex[face.mIndices[0]].x,TempVertex[face.mIndices[0]].y,TempVertex[face.mIndices[0]].z });
					CalVertex.push_back({ TempVertex[face.mIndices[1]].x,TempVertex[face.mIndices[1]].y,TempVertex[face.mIndices[1]].z });
					CalVertex.push_back({ TempVertex[face.mIndices[2]].x,TempVertex[face.mIndices[2]].y,TempVertex[face.mIndices[2]].z });


					CalTexCoords.push_back(TempTexCoords[face.mIndices[0]]);
					CalTexCoords.push_back(TempTexCoords[face.mIndices[1]]);
					CalTexCoords.push_back(TempTexCoords[face.mIndices[2]]);


					InfoVertex[face.mIndices[0]].addTangent(CalculateTangents(CalVertex, CalTexCoords));
					InfoVertex[face.mIndices[1]].addTangent(CalculateTangents(CalVertex, CalTexCoords));
					InfoVertex[face.mIndices[2]].addTangent(CalculateTangents(CalVertex, CalTexCoords));
				}
			}

			if (NormalFlag)
			{
				for (int i = 0; i< pMesh->mNumVertices;i++)
				{
					InfoVertex[i].CalaverageTangents();
					Tangents.push_back(InfoVertex[i].averageTangent);
				}
			}

			Mode3Dlnfo ModelInf = { TempVertex,TempIndex,TempTexCoords,Normals,Tangents };

			s_Model3DInfo.emplace(pathString, ModelInf);
			ShadowList.push_back(pathString);
		}
	}


	glm::vec3 Renderer2D::CalculateTangents(std::vector<glm::vec3> Vertex, std::vector<glm::vec2> TexCoord)
	{
		glm::vec3 delatPos1 = Vertex[1] - Vertex[0];
		glm::vec3 delatPos2 = Vertex[2] - Vertex[0];

		glm::vec2  deltaUv1 = TexCoord[1] - TexCoord[0];
		glm::vec2  deltaUv2 = TexCoord[2] - TexCoord[0];

		float r = 1.0f / (deltaUv1.x * deltaUv2.y - deltaUv1.y * deltaUv2.x);

		delatPos1 = delatPos1 * deltaUv2.y;
		delatPos2 = delatPos2 * deltaUv1.y;
			
		glm::vec3 tangent = (delatPos1 - delatPos2)* r;
			
		return tangent;
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID)
	{
		if (src.Texture!=nullptr)
			DrawQuad(transform, src.Texture, src.tilingFactor, src.Color, entityID);
		else
			DrawQuad(transform, src.Color, entityID);
	}

	void Renderer2D::Index2DSetting()
	{
		s_Data.QuadIndexBufferPtr->Index = s_Data.offset + 0;
		s_Data.QuadIndexBufferPtr++;
		s_Data.QuadIndexBufferPtr->Index = s_Data.offset + 1;
		s_Data.QuadIndexBufferPtr++;
		s_Data.QuadIndexBufferPtr->Index = s_Data.offset + 2;
		s_Data.QuadIndexBufferPtr++;
		s_Data.QuadIndexBufferPtr->Index = s_Data.offset + 2;
		s_Data.QuadIndexBufferPtr++;
		s_Data.QuadIndexBufferPtr->Index = s_Data.offset + 3;
		s_Data.QuadIndexBufferPtr++;
		s_Data.QuadIndexBufferPtr->Index = s_Data.offset + 0;
		s_Data.QuadIndexBufferPtr++;

		s_Data.QuadIndexCount += 6;
		s_Data.offset += 4;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void Renderer2D::StartBatch()
	{
		s_Data.QuadIndexCount = 0;
		s_Data.offset = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.QuadIndexBufferPtr = s_Data.QuadIndexBufferBase;
		//s_Data.ShadowVertexBufferPtr = s_Data.ShadowVertexBufferBase;
		s_Data.TextureSlotIndex = 4;
	}


	void Renderer2D::MakeTerrain()
	{
		/////////////////////////////////////////////// Terrain ///////////////////////////////////////////////////
		BottomTerrain.generateTerrain();

		s_Data.TerrainVertexArray = VertexArray::Create();
		s_Data.TerrainVertexBuffer = VertexBuffer::Create(BottomTerrain.TerrainVertices.size() * sizeof(QuadVertex));

		s_Data.TerrainVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"},
			{ShaderDataType::Float2, "a_TexCoord"},
			{ShaderDataType::Float, "a_TexIndex"},
			{ShaderDataType::Float, "a_TilingFactor"},
			{ShaderDataType::Float3, "a_Normal"},
			{ShaderDataType::Float3, "a_Tangent"},
			{ShaderDataType::Int,	"a_EntityID"}
		});


		s_Data.TerrainVertexArray->AddVertexBuffer(s_Data.TerrainVertexBuffer);

		s_Data.TerrainVertexBufferBase = new QuadVertex[BottomTerrain.TerrainVertices.size()];
		s_Data.TerrainVertexBufferPtr = s_Data.TerrainVertexBufferBase;

		s_Data.TerrainIndexBuffer = IndexBuffer::Create(BottomTerrain.TerrainIndices.size());
		s_Data.TerrainVertexArray->SetIndexBuffer(s_Data.TerrainIndexBuffer);

		s_Data.TerrainIndexBufferBase = new QuadIndex[BottomTerrain.TerrainIndices.size()];
		s_Data.TerrainIndexBufferPtr = s_Data.TerrainIndexBufferBase;


		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { -400.0f,50.0f,-1.0 * BottomTerrain.GetSize()+400.0f });

		for (size_t i = 0; i < BottomTerrain.TerrainVertices.size(); i++)
		{
			s_Data.TerrainVertexBufferPtr->Position = transform * BottomTerrain.TerrainVertices[i];
			s_Data.TerrainVertexBufferPtr->Color = glm::vec4(1.0f);
			s_Data.TerrainVertexBufferPtr->TexCoord = BottomTerrain.TerrainTexCoords[i];
			s_Data.TerrainVertexBufferPtr->TexIndex = 1.0f;
			s_Data.TerrainVertexBufferPtr->TilingFactor = 1.0f;
			s_Data.TerrainVertexBufferPtr->Normal = BottomTerrain.TerrainNormals[i];
			s_Data.TerrainVertexBufferPtr->EntityID = -1.0f;
			s_Data.TerrainVertexBufferPtr++;
		}

		for (size_t i = 0; i < BottomTerrain.TerrainIndices.size(); i++)
		{
			s_Data.TerrainIndexBufferPtr->Index = BottomTerrain.TerrainIndices[i];
			s_Data.TerrainIndexBufferPtr++;
		}
	}

	void Renderer2D::MakeSky()
	{
		/////////////////////////////////////////////// Terrain ///////////////////////////////////////////////////

		s_Data.SkyVertexArray = VertexArray::Create();
		s_Data.SkyVertexBuffer = VertexBuffer::Create(36 * sizeof(SkyVertex));

		s_Data.SkyVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Int,	"a_EntityID"}
		});

		s_Data.SkyVertexArray->AddVertexBuffer(s_Data.SkyVertexBuffer);
		s_Data.SkyVertexBufferBase = new SkyVertex[36];
		s_Data.SkyVertexBufferPtr = s_Data.SkyVertexBufferBase;

		for (int i = 0; i < 36; i++)
		{
			s_Data.SkyVertexBufferPtr->Position = SkyBoxVertexs[i];
			s_Data.SkyVertexBufferPtr->EntityID = -1;
			s_Data.SkyVertexBufferPtr++;
		}
	}

	void Renderer2D::MakeShadow()
	{
	/*	s_Data.ShadowVertexArray = VertexArray::Create();
		s_Data.ShadowVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(ShadowVertex));

		s_Data.ShadowVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Int,	"a_EntityID"}
		});

		s_Data.ShadowVertexArray->AddVertexBuffer(s_Data.ShadowVertexBuffer);

		s_Data.ShadowVertexBufferBase = new ShadowVertex[s_Data.MaxVertices];
		s_Data.ShadowVertexBufferPtr = s_Data.ShadowVertexBufferBase;

		s_Data.ShadowIndexBuffer = IndexBuffer::Create(s_Data.MaxIndices);

		s_Data.ShadowVertexArray->SetIndexBuffer(s_Data.ShadowIndexBuffer);
		s_Data.ShadowIndexBufferBase = new QuadIndex[s_Data.MaxIndices];
		s_Data.ShadowIndexBufferPtr = s_Data.ShadowIndexBufferBase;*/
	}

	void Renderer2D::bindTextures(float ts) 
	{
		s_Data.time += ts * 1000;
		s_Data.time = s_Data.time % 24000;

		if (s_Data.time >= 0 && s_Data.time < 5000)
		{
			s_Data.TextureSlots[2] = s_Data.SkyTexture2;
			s_Data.TextureSlots[3] = s_Data.SkyTexture2;
			s_Data.blendfactor = (s_Data.time - 0) / (5000 - 0);
		}
		else if (s_Data.time >= 5000 && s_Data.time < 8000) 
		{
			s_Data.TextureSlots[2] = s_Data.SkyTexture2;
			s_Data.TextureSlots[3] = s_Data.SkyTexture;

			s_Data.blendfactor = (s_Data.time - 5000) / (8000 - 5000);
		}
		else if (s_Data.time >= 8000 && s_Data.time < 21000) 
		{
			s_Data.TextureSlots[2] = s_Data.SkyTexture;
			s_Data.TextureSlots[3] = s_Data.SkyTexture;

			s_Data.blendfactor = (s_Data.time - 8000) / (21000 - 8000);
		}
		else
		{
			s_Data.TextureSlots[2] = s_Data.SkyTexture;
			s_Data.TextureSlots[3] = s_Data.SkyTexture2;

			s_Data.blendfactor = (s_Data.time - 21000) / (24000 - 21000);
		}
	}
}