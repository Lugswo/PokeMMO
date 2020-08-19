#pragma once

#include <imgui.h>

#include <string>
#include <rttr/variant.h>

#include "System.h"
#include "GameObject.h"
#include "FileExplorer.hpp"
#include "Tileset.h"

class Editor : public System
{
public:
  void Init() override;
  void Update(float dt) override;
  void Shutdown() override;

  static Editor* GetInstance();

  std::vector<std::vector<GameObject*>>& GetPlacedTiles()
  {
    return placedTiles;
  }

  void ResetTiles();

private:
  void SetVariable(const rttr::property&, Component*);

  void Outliner();
  void TileEditor();
  void MenuBar();

  void GetCursorPos();

  static Editor* instance;

  bool createObject, loadPrefab, tile, saveMap;

  std::vector<std::string> objPaths;
  std::vector<std::string> componentTypes;

  std::vector<std::vector<GameObject*>> placedTiles;
  int xoff, yoff;

  glm::vec2 tileU, tileV;
  std::string currTileset, mapName;

  FileExplorer file;

  Tileset* tileset;

  glm::vec3 cursorPos;
};