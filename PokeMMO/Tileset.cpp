#include "Tileset.h"

constexpr int TILE_DIM = 32;

Tileset::Tileset(const std::string& filepath) : tst(filepath)
{
  int w, h;
  w = tst.GetWidth();
  h = tst.GetHeight();

  int row, col;
  row = w / TILE_DIM;
  col = h / TILE_DIM;

  for (int i = 0; i < row; ++i)
  {
    for (int j = 0; j < col; ++j)
    {
      glm::vec2 p1, p2;
      float u, v;

      u = 1.f / col;
      v = 1.f / row;

      p1.y = (j + 1) * u;
      p1.x = i * v;

      p2.y = (j) * u;
      p2.x = ((i + 1) * v);

      tiles.push_back(p1);
      tiles.push_back(p2);
    }
  }
}

Tileset::~Tileset()
{

}
