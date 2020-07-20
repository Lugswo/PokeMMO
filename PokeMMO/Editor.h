#pragma once

#include <imgui.h>

#include <string>
#include <rttr/variant.h>

#include "System.h"
#include "GameObject.h"

class Editor : public System
{
public:
  void Init() override;
  void Update(float dt) override;
  void Shutdown() override;

  static Editor* GetInstance();

private:
  void SetVariable(const rttr::property&, Component*);

  static Editor* instance;
};