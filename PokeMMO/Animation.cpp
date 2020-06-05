#include "Animation.h"

#include "InputManager.h"
#include <rttr/registration>

std::vector<float> Animation::CalculateUV()
{
  unsigned r = currFrame / col;

  unsigned c = currFrame - (r * col);

  //float tru = (float)(c + 1) / (float)col;
  //float trv = (float)r / (float)row;

  //float bru = (float)(c + 1) / (float)col;
  //float brv = (float)(r + 1) / (float)row;

  //float blu = (float)c / (float)col;
  //float blv = (float)(r + 1) / (float)row;

  //float tlu = (float)c / (float)col;
  //float tlv = (float)r / (float)row;

  std::vector<float> uvs;

  uvs.push_back((float)(c + 1) / (float)col);
  uvs.push_back((float)(row - r) / (float)row);
  uvs.push_back((float)(c + 1) / (float)col);
  uvs.push_back((float)(row - r - 1) / (float)row);
  uvs.push_back((float)c / (float)col);
  uvs.push_back((float)(row - r - 1) / (float)row);
  uvs.push_back((float)c / (float)col);
  uvs.push_back((float)(row - r) / (float)row);

  return uvs;
}

void Animation::NextFrame()
{
  if (++currFrame >= numFrames)
  {
    currFrame = 0;
  }

  auto uvs = CalculateUV();
  sprite->ChangeUV(uvs);
}

void Animation::ResetAnimation()
{
  currFrame = 0;
  currTime = frameTime;

  auto uvs = CalculateUV();

  sprite->ChangeUV(uvs);
}

void Animation::Init()
{
  compName = "Animation";
  sprite = GetComponent(Sprite, parent);

  row = col = 1;
  frameTime = .3f;
  currTime = frameTime;

  width = sprite->GetWidth();
  height = sprite->GetHeight();

  fWidth = width;
  fHeight = height;

  numFrames = row * col;
  currFrame = 0;
}

void Animation::Update(float dt)
{
  if (InputManager::GetInstance()->KeyPress(GLFW_KEY_0))
  {
    ResetAnimation();
  }

  currTime -= dt;
  if (currTime <= 0.f)
  {
    currTime = frameTime;
    NextFrame();
  }
}

void Animation::Shutdown()
{

}

RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<Animation>("Animation")
        .constructor<>()
        (

        )
      // register directly a member object pointer; mark it as 'private'
      .property("row", &Animation::row)
      .property("col", &Animation::col)
      .property("numFrames", &Animation::numFrames)
      .property("frameTime", &Animation::frameTime)

      //.property_readonly("children", &node::get_children) // a read-only property; so not set possible
      //.method("set_visible", &node::set_visible)
      //(
      //    default_arguments(true),              // the default value for 'cascade'
      //    parameter_names("visible", "cascade") // provide the names of the parameter; optional, but might be useful for clients
      //)
      //.method("render", &node::render)
      ;
}
