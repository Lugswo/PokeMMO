#pragma once

#include <string>
#include <vector>

#include "Tile.h"
#include "Texture.h"

class Tileset
{
public:
  Tileset(const std::string&);
  ~Tileset();

  const std::vector<glm::vec2>& GetTiles() const
  {
    return tiles;
  }

  GLuint GetTexID()
  {
    return tst.GetTexture();
  }

private:
  std::vector<glm::vec2> tiles;
  Texture tst;
};