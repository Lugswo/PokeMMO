#pragma once

#include <glm.hpp>

#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Sprite.h"

class Tile : public Component
{
public:
  Tile();
  ~Tile();

  void Init() override;
  void ParseInit() override;

  const Transform* GetTransform() const;
  const glm::vec3& GetPosition() const;

  Tile* GetUp() const;
  void SetUp(Tile*);

  Tile* GetRight() const;
  void SetRight(Tile*);

  Tile* GetLeft() const;
  void SetLeft(Tile*);

  Tile* GetDown() const;
  void SetDown(Tile*);

private:
  Transform* trans;
  Tile* up, * down, * left, * right;
};

//class Serializable()
//{
//  void Serialize();
//}
//
//class Nonserializable : Serializable
//{
//  int guy;
//};