#pragma once

#include <chrono>
#include <vector>
#include <iostream>

#include "System.h"

class Engine
{
public:
  static void Init();
  static void Update();
  static void Shutdown();

  static bool IsEditor()
  {
    return editor;
  }

private:
  Engine();
  ~Engine();

  template <typename T>
  static void AddSystem();

  static float dt;

  static std::chrono::time_point<std::chrono::high_resolution_clock> beginFrame;
  static std::vector<System*> systems;

  static bool running;

  constexpr static bool editor = true;
};

#define make_game int main() { Engine::Init(); Engine::Update(); Engine::Shutdown(); return 0; }