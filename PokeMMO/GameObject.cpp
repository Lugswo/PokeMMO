#include "GameObject.h"

#include "Component.h"
#include "GameObjectFactory.h"

#define GetComponent GetComponent

GameObject::GameObject(const std::string& n)
{
  name = n;
  filename = n;
  saved = showEditor = true;
  shouldDelete = false;
}

GameObject::GameObject(const GameObject& o)
{
  for (auto c : o.components)
  {
    rttr::type t = rttr::type::get_by_name(c->GetComponentName());
    rttr::variant* toAdd = new rttr::variant();
    
    *toAdd = t.create({ toAdd });
    Component* componentPtr = toAdd->get_value<Component*>();

    AddComponent(componentPtr);
  }

  name = o.name;
  filename = o.filename;
  saved = o.saved;
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
  for (auto itr = components.begin(); itr != components.end();)
  {
    if ((*itr)->GetDelete())
    {
      itr = components.erase(itr);
    }
    else
    {
      ++itr;
    }
  }

  for (auto c : components)
  {
    c->Update(dt);
  }
}

void GameObject::EditorName()
{
  std::string origName = name;
  unsigned num = 1;
  auto objs = GameObjectFactory::GetInstance()->GetAllObjects();

  bool unique = false;

  while (!unique)
  {
    unique = true;

    for (auto& obj : objs)
    {
      if (obj->GetName() == name && obj != this)
      {
        name = origName;
        name += std::to_string(num++);
        unique = false;
        break;
      }
    }
  }
}

void GameObject::AddComponent(Component* c)
{
  c->SetParent(this);
  c->ParseInit();
  components.push_back(c);
}

Component* GameObject::GetComponent(const std::string& str) const
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