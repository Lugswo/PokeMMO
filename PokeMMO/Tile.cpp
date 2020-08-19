#include "Tile.h"

#include "GameObjectFactory.h"
#include "Sprite.h"

//Tile::Tile(const std::string& filepath, const glm::vec2& u, const glm::vec2& v, const glm::vec3& pos)
//{
//  up = down = left = right = nullptr;
//
//  Transform* trans = GetComponent(Transform, parent);
//  trans->SetPosition(pos);
//
//  Sprite* sprite = GetComponent(Sprite, parent);
//  sprite->SetFilepath(filepath);
//
//  std::vector<float> uvs;
//  uvs.push_back(v.x);
//  uvs.push_back(v.y);
//  uvs.push_back(v.x);
//  uvs.push_back(u.y);
//  uvs.push_back(u.x);
//  uvs.push_back(u.y);
//  uvs.push_back(u.x);
//  uvs.push_back(v.y);
//
//  sprite->ChangeUV(uvs);
//}

Tile::Tile()
{
  SetComponentName("Tile");
}

void Tile::Init()
{
  ParseInit();
}

void Tile::ParseInit()
{
  trans = GetComponent(Transform, parent);
}

const Transform* Tile::GetTransform() const
{
  return trans;
}

const glm::vec3& Tile::GetPosition() const
{
  return trans->GetPosition();
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
