#pragma once

#include "Component.h"

#include <glm.hpp>
#include <rttr/registration_friend.h>

class Transform : public Component
{
public:
  void Init() override;
  void Shutdown() override;

  float GetRotation() const
  {
    return rot;
  }

  void SetRotation(float r)
  {
    rot = r;
    dirty = true;
  }

  const glm::vec3& GetScale() const
  {
    return scale;
  }

  void SetScale(const glm::vec3& s)
  {
    scale = s;
    dirty = true;
  }

  const glm::vec3& GetPosition() const
  {
    return pos;
  }

  void SetPosition(const glm::vec3& p)
  {
    pos = p;
    dirty = true;
  }

  void SetTextureScale(float s)
  {
    tScale.x = s;
    dirty = true;
  }

  const glm::mat4& GetMatrix();

private:
  glm::mat4 mat;

  float rot;
  glm::vec3 scale;
  glm::vec3 pos;
  glm::vec3 tScale;

  bool dirty;

  RTTR_ENABLE(Component)
  RTTR_REGISTRATION_FRIEND
};