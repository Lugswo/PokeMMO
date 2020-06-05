#include "Window.h"

#include "Logger.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  // update projection matrix later
  glViewport(0, 0, width, height);
}

GLFWwindow* Window::GetWindow()
{
  return window;
}

Window::Window(unsigned w, unsigned h)
{
  window = glfwCreateWindow(w, h, "PokeMMO", NULL, NULL);
  if (window == NULL)
  {
    L::Log(TL::FATAL, "Failed to create GLFW window");
    glfwTerminate();
  }
  glfwMakeContextCurrent(window);

  glViewport(0, 0, w, h);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void Window::Update()
{

}