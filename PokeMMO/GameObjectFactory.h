#pragma once

#include "System.h"
#include "GameObject.h"

class GameObjectFactory : public System
{
public:
  void Init() override;
  void Update(float dt) override;
  void Shutdown() override;

  void AddObject(GameObject* obj)
  {
    objects.push_back(obj);
  }

  GameObject* ParseObject(const std::string& filepath);

  static GameObjectFactory* GetInstance();

  GameObject* GetPlayer()
  {
    return playerRef;
  }

  void SetPlayerRef(GameObject* player)
  {
    playerRef = player;
  }

  void UnsetPlayerRef()
  {
    playerRef = nullptr;
  }

  GameObject* FindObject(const std::string& name);

  std::vector<GameObject*>& GetAllObjects();  
  const std::vector<const GameObject*> GetAllObjectsConst() const;

private:
  static GameObjectFactory* instance;
  std::vector<GameObject*> objects;

  GameObject* playerRef;
};