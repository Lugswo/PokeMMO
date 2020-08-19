#include "Camera.h"

#include <gtc/matrix_transform.hpp>

#include "GameObjectFactory.h"
#include "Transform.h"

#include "Editor.h"
#include "InputManager.h"

Camera* Camera::instance;

void Camera::Init()
{
  position = glm::vec3(0.f, 0, 3);
  jumbo = glm::vec3(0.f, 0, -2);
}

Camera* Camera::GetInstance()
{
  if (!instance)
  {
    instance = new Camera();
  }

  return instance;
}

void Camera::Update(float dt)
{
  if (!Editor::GetInstance())
  {
    auto player = GameObjectFactory::GetInstance()->GetPlayer();
    if (player)
    {
      position = GetComponent(Transform, player)->GetPosition();
      position.z = 3;
    }
  }
  else
  {
    if (InputManager::GetInstance()->KeyDown(GLFW_KEY_W))
    {
      position.y += .05f;
    }
    if (InputManager::GetInstance()->KeyDown(GLFW_KEY_A))
    {
      position.x -= .05f;
    }
    if (InputManager::GetInstance()->KeyDown(GLFW_KEY_S))
    {
      position.y -= .05f;
    }
    if (InputManager::GetInstance()->KeyDown(GLFW_KEY_D))
    {
      position.x += .05f;
    }
  }

  view = glm::lookAt(position, position + jumbo, glm::vec3(0, 1, 0));
}

void Camera::Shutdown()
{

}
