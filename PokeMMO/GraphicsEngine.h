#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLFW_INCLUDE_NONE

#include <vector>

#include "System.h"
#include "Window.h"

class Sprite;

class GraphicsEngine : public System
{
public:
  void Init() override;
  void Update(float dt) override;
  void Shutdown() override;

  void AddToQueue(const Sprite* s);

  static GraphicsEngine* GetInstance();

  bool Closed();

private:
  GraphicsEngine() {};
  ~GraphicsEngine();

  void Draw();
  void DrawSprite(const Sprite*);

  std::vector<const Sprite*> drawQueue;

  static GraphicsEngine* instance;
  Window* window;
};
