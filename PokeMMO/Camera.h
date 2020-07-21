#pragma once

#include <glm.hpp>

#include "System.h"

class Camera : public System
{
public:
  void Init() override;
  void Update(float dt) override;
  void Shutdown() override;

  const glm::mat4& GetCamera() const
  {
    return view;
  }

  static Camera* GetInstance();

  const glm::vec3& GetPosition() const
  {
    return position;
  }

private:
  Camera() {};
  ~Camera() {};

  static Camera* instance;

  glm::mat4 view;
  glm::vec3 position, jumbo;
};
