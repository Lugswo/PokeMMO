#include "Transform.h"

#include <gtc/matrix_transform.hpp>
#include <rttr/registration>

Transform::Transform()
{
  SetComponentName("Transform");
}

void Transform::Init()
{
  mat = glm::mat4(1.f);
  scale = glm::vec3(1.f);
  rot = 0.f;
  pos = glm::vec3(0.f);

  ParseInit();
}

void Transform::ParseInit()
{
  tScale = glm::vec3(1.f);
}

void Transform::Shutdown()
{

}

const glm::mat4& Transform::GetMatrix()
{
  if (dirty)
  {
    mat = glm::mat4(1.f);
    glm::mat4 t = glm::mat4(1.f), r = glm::mat4(1.f), s = glm::mat4(1.f);
    t = glm::translate(t, pos);
    r = glm::rotate(r, rot, glm::vec3(0, 0, 1));
    s = glm::scale(s, scale);
    s = glm::scale(s, tScale);

    mat = t * r * s;

    dirty = false;
  }

  return mat;
}

RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<Transform>("Transform")
        .constructor<>()
        (

        )
      // register directly a member object pointer; mark it as 'private'
      .property("pos", &Transform::GetPosition, &Transform::SetPosition)
      .property("scale", &Transform::GetScale, &Transform::SetScale)
      .property("rotation", &Transform::GetRotation, &Transform::SetRotation)
      //.property_readonly("children", &node::get_children) // a read-only property; so not set possible
      //.method("set_visible", &node::set_visible)
      //(
      //    default_arguments(true),              // the default value for 'cascade'
      //    parameter_names("visible", "cascade") // provide the names of the parameter; optional, but might be useful for clients
      //)
      //.method("render", &node::render)
      ;
}
