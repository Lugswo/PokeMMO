#include "GameObjectFactory.h"

#include "Logger.h"
#include "Serializer.h"

GameObjectFactory* GameObjectFactory::instance;

GameObject* GameObjectFactory::ParseObject(const std::string& filepath)
{
  GameObject* obj = Serializer::Parse(filepath);

  if (obj)
  {
    objects.push_back(obj);
    return obj;
  }

  L::Log(TL::ERR, "GameObject " + filepath + " failed to load!");

  return nullptr;
}

GameObjectFactory* GameObjectFactory::GetInstance()
{
  if (!instance)
  {
    instance = new GameObjectFactory();
  }

  return instance;
}

GameObject* GameObjectFactory::FindObject(const std::string& name)
{
  for (int i = 0; i < objects.size(); ++i)
  {
    if (objects[i]->GetName() == name)
    {
      return objects[i];
    }
  }
  return nullptr;
}

std::vector<const GameObject*> GameObjectFactory::GetObjectsConst() const
{
  std::vector<const GameObject*> constObjects;
  for (const GameObject* g : objects)
  {
    constObjects.emplace_back(g);
  }
  return constObjects;
}

void GameObjectFactory::Init()
{
  playerRef = nullptr;
}

void GameObjectFactory::Update(float dt)
{
  for (auto obj : objects)
  {
    obj->Update(dt);
  }
}

void GameObjectFactory::Shutdown()
{
  for (auto obj : objects)
  {
    delete obj;
  }
}