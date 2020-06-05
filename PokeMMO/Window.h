#pragma once

#include <GLFW/glfw3.h>

class Window
{
public:
  Window(unsigned w, unsigned h);
  ~Window();

  void Update();
  GLFWwindow* GetWindow();

private:
  GLFWwindow* window;
};