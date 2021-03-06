#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

// TEMPORARY #ToFix REMOVE 
#include <gl/GL.h>;

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	m_SquareVA = Denocat::VertexArray::Create();

	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	Denocat::Ref<Denocat::VertexBuffer> squareVB;
	squareVB.reset(Denocat::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	squareVB->SetLayout({
		{ Denocat::ShaderDataType::Float3, "a_Position" }
		});
	m_SquareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Denocat::Ref<Denocat::IndexBuffer> squareIB;
	squareIB.reset(Denocat::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	m_SquareVA->SetIndexBuffer(squareIB);

	m_FlatColorShader = Denocat::Shader::Create("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Denocat::Timestep ts)
{
	float FPS = 1.0f / ts;
	VS_TRACE(FPS);
	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	Denocat::RenderCommand::SetClearColor({ 0.1f, 0.2f, 0.1f, 1 });
	Denocat::RenderCommand::Clear();

	Denocat::Renderer::BeginScene(m_CameraController.GetCamera());

	// TODO: Add Shader::SetMat4 and Shader::SetFloat4
	std::dynamic_pointer_cast<Denocat::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<Denocat::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

	Denocat::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Denocat::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();

	ImGui::Begin("File settings");
	ImGui::Text("Hi");
	ImGui::End();
}

void Sandbox2D::OnEvent(Denocat::Event& e)
{
	m_CameraController.OnEvent(e);
}