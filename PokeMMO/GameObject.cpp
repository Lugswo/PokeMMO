#include "GameObject.h"

#include "Component.h"

#define GetComponent GetComponent

GameObject::GameObject(const std::string& n)
{
  name = n;
  filename = n;
}

GameObject::~GameObject()
{
  for (auto c : components)
  {
    delete c;
  }
  components.clear();
}

void GameObject::Update(float dt)
{
  for (auto c : components)
  {
    c->Update(dt);
  }
}

void GameObject::AddComponent(Component* c)
{
  c->SetParent(this);
  c->ParseInit();
  components.push_back(c);
}

Component* GameObject::GetComponent(const std::string& str)
{
  for (auto c : components)
  {
    if (c->GetComponentName() == str)
    {
      return c;
    }
  }

  return nullptr;
}