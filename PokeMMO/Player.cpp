#include "Player.h"

#include <rttr/registration>

#include "InputManager.h"
#include "GameObjectFactory.h"

namespace Movement
{
  constexpr float SPEED = .5f;
};

Player::Player()
{
  SetComponentName("Player");
}

void Player::Init()
{
  ParseInit();
}

void Player::ParseInit()
{
  anim = GetComponent(Animation, parent);
  trans = GetComponent(Transform, parent);
  GameObjectFactory::GetInstance()->SetPlayerRef(parent);

  Tile* tile = new Tile();
  tile->SetPosition(glm::vec3(0, 0, 0));

  Tile* right = new Tile();
  right->SetPosition(glm::vec3(.15, 0, 0));

  tile->SetRight(right);
  right->SetLeft(tile);

  currentTile = tile;
  trans->SetPosition(glm::vec3(0, 0, 0));
}

void Player::Update(float dt)
{
  auto position = trans->GetPosition();

  if (!isMoving)
  {
    uint8_t move = 0; // bitwise storage for movement; left > right > up > down
    if (InputManager::GetInstance()->KeyDown(GLFW_KEY_LEFT))
    {
      if (currentTile->GetLeft())
      {
        currentTile = currentTile->GetLeft();
        isMoving = true;
      }
      move |= 0b1000;
    }
    if (InputManager::GetInstance()->KeyDown(GLFW_KEY_RIGHT))
    {
      if (currentTile->GetRight())
      {
        currentTile = currentTile->GetRight();
        isMoving = true;
      }
      move |= 0b0100;
    }
    if (InputManager::GetInstance()->KeyDown(GLFW_KEY_UP))
    {
      if (currentTile->GetUp())
      {
        currentTile = currentTile->GetUp();
        isMoving = true;
      }
      move |= 0b0010;
    }
    if (InputManager::GetInstance()->KeyDown(GLFW_KEY_DOWN))
    {
      if (currentTile->GetDown())
      {
        currentTile = currentTile->GetDown();
        isMoving = true;
      }
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
  }
  else
  {
    glm::vec3 dir = position - currentTile->GetPosition();
    if (glm::length(dir) != 0)
    {
      dir = glm::normalize(dir);
      glm::vec3 move = dir * Movement::SPEED * dt;
      position -= move;

      dir = currentTile->GetPosition() - position;
      if (glm::length(dir) < glm::length(move))
      {
        position = currentTile->GetPosition();
        isMoving = false;
      }
    }
    else
    {
      isMoving = false;
    }
  }

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