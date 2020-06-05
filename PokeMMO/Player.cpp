#include "Player.h"

#include "InputManager.h"
#include "GameObjectFactory.h"

namespace Movement
{
  constexpr float SPEED = 1.f;
};

void Player::Init()
{
  compName = "Player";

  trans = GetComponent(Transform, parent);
  GameObjectFactory::GetInstance()->SetPlayerRef(parent);
}

void Player::Update(float dt)
{
  auto position = trans->GetPosition();
  if (InputManager::GetInstance()->KeyDown(GLFW_KEY_LEFT))
  {
    position.x -= Movement::SPEED * dt;
  }
  if (InputManager::GetInstance()->KeyDown(GLFW_KEY_RIGHT))
  {
    position.x += Movement::SPEED * dt;
  }
  if (InputManager::GetInstance()->KeyDown(GLFW_KEY_UP))
  {
    position.y += Movement::SPEED * dt;
  }
  if (InputManager::GetInstance()->KeyDown(GLFW_KEY_DOWN))
  {
    position.y -= Movement::SPEED * dt;
  }

  trans->SetPosition(position);
}

void Player::Shutdown()
{
  GameObjectFactory::GetInstance()->UnsetPlayerRef();
}