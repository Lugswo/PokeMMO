#pragma once

#include <vector>

#include "GameObject.h"

class Map
{
public:
  Map();
  ~Map();

private:
  std::vector<GameObject*> tiles;
  std::vector<GameObject*> objects;
};