#include "Component.h"

void Component::Init()
{
  name = compName;
}

void Component::ParseInit()
{
}

void Component::Update(float dt)
{

}

void Component::Shutdown()
{

}

void Component::SetComponentName(const std::string& str)
{
  compName = str;

  if (name == "")
  {
    name = str;
  }
}

void Component::SetName(const std::string& str)
{
  name = str;
}