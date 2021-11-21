#include "Bank2D.h"
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Bank2D::Bank2D()
	: Layer("Bank2D"), m_CameraController(1280.0f / 720.0f), m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
{
}

void Bank2D::OnAttach()
{
	//m_CheckerboardTexture = Gold::Texture2D::Create("assets/textures/Checkerboard.png");


	//// Particle Setting Init here
	//m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	//m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	//m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	//m_Particle.LifeTime = 1.0f;
	//m_Particle.Velocity = { 0.0f, 0.0f };
	//m_Particle.VelocityVariation = { 3.0f, 1.0f };
	//m_Particle.Position = { 0.0f, 0.0f };
}

void Bank2D::OnDetach()
{

}

void Bank2D::OnUpdate(Gold::Timestep ts)
{
	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	Gold::Renderer2D::ResetStats();
	{
		Gold::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Gold::RenderCommand::Clear();
	}

	{
		static float rotation = 0.0f;
		rotation += ts * 50.0f;

		Gold::Renderer2D::BeginScene(m_CameraController.GetCamera(),0.03f);
		Gold::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0,0,1 }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Gold::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Gold::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_SquareColor);
	//	Gold::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckerboardTexture, 10.0f);
	//	Gold::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, { 0,0,1 }, m_CheckerboardTexture, 20.0f);
		Gold::Renderer2D::EndScene();

		Gold::Renderer2D::BeginScene(m_CameraController.GetCamera(), 0.03f);
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				Gold::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}
		Gold::Renderer2D::EndScene();
	}


	////Particle System Use Method////
	/*if (Gold::Input::IsMouseButtonPressed(Gold::Mouse::ButtonLeft))
	{
		auto [x, y] = Gold::Input::GetMousePosition();
		auto width = Gold::Application::Get().GetWindow().GetWidth();
		auto height = Gold::Application::Get().GetWindow().GetHeight();

		auto bounds = m_CameraController.GetBounds();
		auto pos = m_CameraController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_Particle.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 5; i++)
			m_ParticleSystem.Emit(m_Particle);
	}

	m_ParticleSystem.OnUpdate(ts);
	m_ParticleSystem.OnRender(m_CameraController.GetCamera());*/
}

void Bank2D::OnImGuiRender()
{
	ImGui::Begin("Settings");

	auto stats = Gold::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Bank2D::OnEvent(Gold::Event& e)
{
	m_CameraController.OnEvent(e);
}