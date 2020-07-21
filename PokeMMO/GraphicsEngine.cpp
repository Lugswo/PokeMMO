#include "GraphicsEngine.h"

#include <iostream>
#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>

#include "Sprite.h"
#include "stb_image.h"
#include "Logger.h"
#include "InputManager.h"

GraphicsEngine* GraphicsEngine::instance = nullptr;

namespace Graphics
{
  const unsigned WIDTH = 1920;
  const unsigned HEIGHT = 1080;
}

GraphicsEngine* GraphicsEngine::GetInstance()
{
  if (!instance)
  {
    instance = new GraphicsEngine();
  }

  return instance;
}

Window* GraphicsEngine::GetWindow()
{
  return window;
}

bool GraphicsEngine::Closed()
{
  return glfwWindowShouldClose(window->GetWindow());
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (action == GLFW_PRESS)
    InputManager::GetInstance()->PushKey(key);
  else if (action == GLFW_RELEASE)
    InputManager::GetInstance()->RelKey(key);
}

void GraphicsEngine::Init()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = new Window(Graphics::WIDTH, Graphics::HEIGHT);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    L::Log(TL::FATAL, "GLAD failed to initialize!");
    throw std::runtime_error("GLAD not initialized");
  }

  glfwSetKeyCallback(window->GetWindow(), key_callback);

  stbi_set_flip_vertically_on_load(true);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GraphicsEngine::Update(float dt)
{
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  Draw();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  glfwSwapBuffers(window->GetWindow());
}

void GraphicsEngine::AddToQueue(const Sprite* s)
{
  drawQueue.push_back(s);
}

void GraphicsEngine::DrawSprite(const Sprite* s)
{
  const Shader* shader = s->GetShader();
  shader->Use();

  const Transform* transform = s->GetTransform();

  if (transform)
  {
    s->GetShader()->Uniform(s->GetTransform()->GetMatrix(), "transform");
    shader->Uniform(0, "tex");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, s->GetTexture()->GetTexture());

    glBindVertexArray(s->GetVAO());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s->GetEBO());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
}

void GraphicsEngine::Draw()
{
  for (auto& s : drawQueue)
  {
    DrawSprite(s);
  }

  drawQueue.clear();
}

void GraphicsEngine::Shutdown()
{
  delete instance;
}

GraphicsEngine::~GraphicsEngine()
{
  glfwTerminate();
}