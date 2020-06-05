#include "Camera.h"

#include <gtc/matrix_transform.hpp>

#include "GameObjectFactory.h"
#include "Transform.h"

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
  auto player = GameObjectFactory::GetInstance()->GetPlayer();
  if (player)
  {
    position = GetComponent(Transform, player)->GetPosition();
    position.z = 3;
  }
  view = glm::lookAt(position, position + jumbo, glm::vec3(0, 1, 0));
}

void Camera::Shutdown()
{

}
