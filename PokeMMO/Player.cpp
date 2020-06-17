#include "Player.h"

#include <rttr/registration>

#include "InputManager.h"
#include "GameObjectFactory.h"

namespace Movement
{
  constexpr float SPEED = 1.f;
};

void Player::Init()
{
  SetName("Player");

  trans = GetComponent(Transform, parent);
  GameObjectFactory::GetInstance()->SetPlayerRef(parent);
}

void Player::ParseInit()
{
  SetName("Player");

  trans = GetComponent(Transform, parent);
  GameObjectFactory::GetInstance()->SetPlayerRef(parent);
}

void Player::Update(float dt)
{
  auto anim = GetComponent(Animation, parent);
  auto position = trans->GetPosition();
  uint8_t move = 0; // bitwise storage for movement; left > right > up > down
  if (InputManager::GetInstance()->KeyDown(GLFW_KEY_LEFT))
  {
    position.x -= Movement::SPEED * dt;
    move |= 0b1000;
  }
  if (InputManager::GetInstance()->KeyDown(GLFW_KEY_RIGHT))
  {
    position.x += Movement::SPEED * dt;
    move |= 0b0100;
  }
  if (InputManager::GetInstance()->KeyDown(GLFW_KEY_UP))
  {
    position.y += Movement::SPEED * dt;
    move |= 0b0010;
  }
  if (InputManager::GetInstance()->KeyDown(GLFW_KEY_DOWN))
  {
    position.y -= Movement::SPEED * dt;
    move |= 0b0001;
  }

  // get previous animation state and map to bit flags
  std::string prevAnim = anim->GetAnim();
  uint8_t prevMove = 0;
  if (prevAnim == "move_left") prevMove = 0b1000;
  else if (prevAnim == "move_right") prevMove = 0b0100;
  else if (prevAnim == "move_up") prevMove = 0b0010;
  else if (prevAnim == "move_down") prevMove = 0b0001;
  else prevMove = 0b0000;

  // if conflicting dirs are active, deactivate both
  if ((move & 0b1100) == 0b1100) move &= 0b0011;
  if ((move & 0b0011) == 0b0011) move &= 0b1100;

  // set anim if changed
  if (move & prevMove) {} // nothing changed since last move is still held
  else if (move & 0b1000) anim->SetAnim("move_left");
  else if (move & 0b0100) anim->SetAnim("move_right");
  else if (move & 0b0010) anim->SetAnim("move_up");
  else if (move & 0b0001) anim->SetAnim("move_down");
  else if (prevMove & 0b1000) anim->SetAnim("face_left");
  else if (prevMove & 0b0100) anim->SetAnim("face_right");
  else if (prevMove & 0b0010) anim->SetAnim("face_up");
  else if (prevMove & 0b0001) anim->SetAnim("face_down");
  else {}; // nothing changed since no move now or prev

  trans->SetPosition(position);
}

void Player::Shutdown()
{
  GameObjectFactory::GetInstance()->UnsetPlayerRef();
}

RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<Player>("Player")
        .constructor<>()
        (

        )
      ;
}