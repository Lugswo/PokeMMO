#pragma once

#include <glm.hpp>

class Tile
{
public:
  Tile();
  ~Tile();

  const glm::vec3& GetPosition() const;
  void SetPosition(const glm::vec3&);

  Tile* GetUp() const;
  void SetUp(Tile*);

  Tile* GetRight() const;
  void SetRight(Tile*);

  Tile* GetLeft() const;
  void SetLeft(Tile*);

  Tile* GetDown() const;
  void SetDown(Tile*);

private:
  glm::vec3 position;
  Tile* up, * down, * left, * right;
};