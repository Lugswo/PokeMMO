#pragma once

#include "Component.h"
#include "Transform.h"

class Player : public Component
{
public:
  void Init() override;
  void Update(float dt) override;
  void Shutdown() override;

private:
  Transform* trans;
};