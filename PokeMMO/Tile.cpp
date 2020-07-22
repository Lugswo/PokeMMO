#include "Tile.h"

Tile::Tile()
{
  up = down = left = right = nullptr;
}

const glm::vec3& Tile::GetPosition() const
{
  return position;
}

void Tile::SetPosition(const glm::vec3& p)
{
  position = p;
}

Tile* Tile::GetUp() const
{
  return up;
}

void Tile::SetUp(Tile* u)
{
  up = u;
}

Tile* Tile::GetRight() const
{
  return right;
}

void Tile::SetRight(Tile* r)
{
  right = r;
}

Tile* Tile::GetLeft() const
{
  return left;
}

void Tile::SetLeft(Tile* l)
{
  left = l;
}

Tile* Tile::GetDown() const
{
  return down;
}

void Tile::SetDown(Tile* d)
{
  down = d;
}
