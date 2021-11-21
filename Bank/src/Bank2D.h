#pragma once

#include <Gold.h>

class Bank2D : public Gold::Layer
{
public:
	Bank2D();
	virtual ~Bank2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Gold::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Gold::Event& e) override;
private:
	Gold::OrthographicCameraController m_CameraController;

	// Temp
	Gold::Ref<Gold::VertexArray> m_SquareVA;
	Gold::Ref<Gold::Shader> m_FlatColorShader;

	Gold::Ref<Gold::Texture2D> m_CheckerboardTexture;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};