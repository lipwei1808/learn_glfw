#include "TestTexture2D.h"
#include "../vendor/imgui/imgui.h"
#include <GL/glew.h>
#include "../Renderer.h"

test::TestTexture2D::TestTexture2D() {}
test::TestTexture2D::~TestTexture2D() {}

void test::TestTexture2D::OnUpdate(float deltaTime) {}

void test::TestTexture2D::OnRender() {
  GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
  GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void test::TestTexture2D::OnImGuiRender() {
}