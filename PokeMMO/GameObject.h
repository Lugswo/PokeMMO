#pragma once

#include <string>
#include <vector>

class Component;

class GameObject
{
public:
  GameObject(const std::string& name);
  ~GameObject();

  void Update(float dt);

  template <typename T>
  void AddComponent(std::string str = "nothing")
  {
    T* comp = new T();
    comp->SetParent(this);
    if (str != "nothing")
    {
      comp->SetDepression(str);
    }
    comp->Init();
    components.push_back(comp);
  }

  void AddComponent(Component* c);

  Component* GetComponent(const std::string& str);

  const std::string& GetName() const
  {
    return name;
  }

  const std::string& GetFilename() const
  {
    return filename;
  }

  const std::vector<Component*> GetComponents() const
  {
    return components;
  }

  const bool GetSaved() const
  {
    return saved;
  }

  void SetSaved(bool b)
  {
    saved = b;
  }

  bool ShouldDelete()
  {
    return shouldDelete;
  }

  void SetDelete()
  {
    shouldDelete = true;
  }

private:
  std::vector<Component *> components;
  std::string name;

  bool saved, shouldDelete;

  std::string filename;
};
