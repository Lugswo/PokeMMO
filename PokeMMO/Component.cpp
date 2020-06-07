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

void Component::SetName(const std::string& str)
{
  compName = str;
  name = str;
}