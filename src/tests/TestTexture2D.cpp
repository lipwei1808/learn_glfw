#include <GL/glew.h>
#include <memory>

#include "../vendor/imgui/imgui.h"
#include "../Renderer.h"
#include "../VertexArray.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../VertexBufferLayout.h"
#include "../Shader.h"
#include "../Texture.h"


#include "TestTexture2D.h"

test::TestTexture2D::TestTexture2D()
    : m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0),
    m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 520.0f, -1.0f, 1.0f)),
    m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))) {
  float positions[] = {
    -50, -50, 0, 0,
    50, -50, 1, 0,
    50, 50, 1, 1,
    -50, 50, 0, 1
  };

  unsigned int indices[] = {
    0, 1, 2,
    0, 2, 3
  };

  // Setup buffers for the positions
  m_VAO = std::make_unique<VertexArray>();
  m_VB = std::make_unique<VertexBuffer>(positions, sizeof(positions));
  VertexBufferLayout layout;
  layout.Push<float>(2);
  layout.Push<float>(2);
  m_VAO->AddBuffer(*m_VB, layout);
  m_IB = std::make_unique<IndexBuffer>(indices, 6);

  // Setup shader
  m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
  m_Shader->Bind();
  m_Shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

  // Setup texture
  m_Texture = std::make_unique<Texture>("res/textures/ChernoLogo2.png");
  m_Texture->Bind();
  m_Shader->SetUniform1i("u_Texture", 0);
}
test::TestTexture2D::~TestTexture2D() {}

void test::TestTexture2D::OnUpdate(float deltaTime) {}

void test::TestTexture2D::OnRender() {
  GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
  GLCall(glClear(GL_COLOR_BUFFER_BIT));

  Renderer renderer;

  {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
    glm::mat4 mvp = m_Proj * m_View * model;
    m_Shader->Bind();
    m_Shader->SetUniformMat4f("u_MVP", mvp);
    renderer.Draw(*m_VAO, *m_IB, *m_Shader);
  }
  {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
    glm::mat4 mvp = m_Proj * m_View * model;
    m_Shader->Bind();
    m_Shader->SetUniformMat4f("u_MVP", mvp);
    renderer.Draw(*m_VAO, *m_IB, *m_Shader);
  }
}

void test::TestTexture2D::OnImGuiRender() {
  ImGui::SliderFloat3("Translate A", &m_TranslationA.x, 0.0f, 960.0f);
  ImGui::SliderFloat3("Translate B", &m_TranslationB.x, 0.0f, 960.0f);
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}