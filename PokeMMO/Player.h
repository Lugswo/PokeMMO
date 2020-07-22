#pragma once

#include "Component.h"
#include "Transform.h"
#include "Animation.h"
#include "Tile.h"

class Player : public Component
{
public:
  Player();
  void Init() override;
  void ParseInit() override;
  void Update(float dt) override;
  void Shutdown() override;

private:
  Transform* trans;
  Animation* anim;

  Tile* currentTile;

  bool isMoving;

  RTTR_ENABLE(Component)
};