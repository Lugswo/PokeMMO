#pragma once

#include "Component.h"
#include "Transform.h"
#include "Animation.h"

class Player : public Component
{
public:
  void Init() override;
  void ParseInit() override;
  void Update(float dt) override;
  void Shutdown() override;

private:
  Transform* trans;
  Animation* anim;

  RTTR_ENABLE(Component)
};