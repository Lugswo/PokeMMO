#pragma once

#include <imgui.h>

#include <string>
#include <rttr/variant.h>

#include "System.h"
#include "GameObject.h"
#include "FileExplorer.hpp"

class Editor : public System
{
public:
  void Init() override;
  void Update(float dt) override;
  void Shutdown() override;

  static Editor* GetInstance();

private:
  void SetVariable(const rttr::property&, Component*);

  void MenuBar();

  static Editor* instance;

  bool createObject, loadPrefab;

  std::vector<std::string> objPaths;
  std::vector<std::string> componentTypes;

  FileExplorer file;
};