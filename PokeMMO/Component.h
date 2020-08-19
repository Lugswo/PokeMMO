#pragma once

#include "GameObject.h"

#include <rttr/type>

#define GetComponent(name, obj) ((name *)obj->GetComponent(#name))

class Component
{
public:
  virtual void Init();
  virtual void ParseInit();
  virtual void Update(float dt);
  virtual void Shutdown();

  virtual void SetDepression(const std::string&) {};

  GameObject* GetParent() const
  {
    return parent;
  }

  void SetParent(GameObject* obj)
  {
    parent = obj;
  }

  const std::string& GetComponentName() const
  {
    return compName;
  }

  const std::string& GetName() const
  {
    return name;
  }

  void SetComponentName(const std::string& str);
  void SetName(const std::string& str);

  bool GetDelete()
  {
    return shouldDelete;
  }

  void SetDelete()
  {
    shouldDelete = true;
  }

protected:
  GameObject* parent;
  std::string compName;
  std::string name;

private:
  bool shouldDelete;
  RTTR_ENABLE()
};