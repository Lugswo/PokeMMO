#include "GameObjectFactory.h"

#include "Serializer.h"

GameObjectFactory* GameObjectFactory::instance;

GameObject* GameObjectFactory::ParseObject(const std::string& filepath)
{
  GameObject* obj = Serializer::Parse(filepath);

  if (obj)
    return obj;

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