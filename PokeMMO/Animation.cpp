#include "Animation.h"

#include "InputManager.h"
#include <rttr/registration>
#include "Logger.h"

Animation::Animation()
{
  SetComponentName("Animation");

  row = col = 1;
  frameTime = .3f;

  currFrame = 0;
}

std::vector<float> Animation::CalculateUV()
{

  unsigned r = frameOrderCurrent[currFrame] / col;


  unsigned c = frameOrderCurrent[currFrame] - (r * col);

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
  if (++currFrame > endFrame)
  {

    currFrame = 0;//startRow * col;
  }

  auto uvs = CalculateUV();
  sprite->ChangeUV(uvs);
}

std::vector<int> Animation::UncompFrameOrder(const std::vector<int>& inVal)
{
  auto retVal = std::vector<int>();
  for (int i = 0; i < inVal.size(); i++)
  {
    if (inVal[i] == -1)
    {
      if (inVal.size() < i + 2)
      {
        L::Log(TL::WARN, "Compressed animation frame sequence is not correct: -1 is not followed by 2 valid numbers");
        break;
      }
      else
      {
        assert(inVal[i + 1] <= inVal[i + 2] &&
          inVal[i + 1] >= 0 &&
          inVal[i + 2] >= 0);
        for (int j = inVal[i + 1]; j <= inVal[i + 2]; j++)
        {
          retVal.push_back(j);
        }
        i += 2;
      }
    }
    else
    {
      retVal.push_back(inVal[i]);
    }
  }

  return retVal;
}

void Animation::SetFrameOrderComp(const std::string& key, const std::vector<int>& inVal)
{
  if (frameOrderCompMap.count(key) == 0 && frameOrderMap.count(key) == 0)
  {
    frameOrderCompMap.emplace(key, inVal);
    frameOrderMap.emplace(key, UncompFrameOrder(inVal));
  }
  else
  {
    L::Log(TL::WARN, "Animation frame order added for key that already exists");
  }
}

void Animation::SetFrameOrderComp(const std::map<std::string, std::vector<int>>& compMap)
{
  for (auto& mapVal : compMap)
  {
    frameOrderMap.emplace(mapVal.first, UncompFrameOrder(mapVal.second));
    if (frameOrderCompMap.count(mapVal.first) == 0) // add if not already in comp map
    {
      frameOrderCompMap.insert(mapVal);
    }
  }
}

void Animation::SetAnim(const std::string& key)
{
  if (frameOrderMap.count(key) == 1)
  {
    currentAnimKey = key;
    frameOrderCurrent = frameOrderMap[key];
    endFrame = (unsigned)(frameOrderCurrent.size() - 1);
    ResetAnimation();
  }
  else
  {
    L::Log(TL::WARN, "Animation frame order for given key does not exist");
  }
}

void Animation::SetAnim()
{
  SetAnim(defaultAnimKey);
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
  ParseInit();
  endFrame = 0;
  defaultAnimKey = "";}

void Animation::ParseInit()
{
  sprite = GetComponent(Sprite, parent);
  currTime = frameTime;

  width = sprite->GetWidth();
  height = sprite->GetHeight();

  fWidth = width / col;
  fHeight = height / row;

  Transform* trans = GetComponent(Transform, parent);
  trans->SetTextureScale((float)fWidth / (float)fHeight);


  currFrame = 0;

  if (defaultAnimKey == "")
  {
    defaultAnimKey = "default";
    SetFrameOrderComp(defaultAnimKey, { 0 });
  }

  if (frameOrderCompMap.size() == 0)
  {
    SetFrameOrderComp(defaultAnimKey, { 0 });
  }
  else
  {
    SetFrameOrderComp(frameOrderCompMap);
  }
  SetAnim();

  auto uvs = CalculateUV();
  sprite->ChangeUV(uvs);
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

      .property("frameTime", &Animation::frameTime)
      .property("defaultAnimKey", &Animation::defaultAnimKey)
      .property("frameOrderCompMap", &Animation::frameOrderCompMap)
      ;
}
