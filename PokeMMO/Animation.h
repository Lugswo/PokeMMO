#pragma once

#include "Sprite.h"

#include <rttr/registration_friend.h>
#include <map>

class Animation : public Component
{
public:
  Animation();

  void Init() override;
  void ParseInit() override;
  void Update(float dt) override;
  void Shutdown() override;

  unsigned GetRows() const
  {
    return row;
  }

  void SetRows(unsigned r)
  {
    row = r;

    fHeight = (unsigned)((float)height / (float)row);

    float aspect = (float)fWidth / (float)fHeight;
    sprite->SetAspect(aspect);
    // endFrame = numFrames + (startRow * col);

    ResetAnimation();
  }

  unsigned GetCols() const
  {
    return col;
  }

  void SetCols(unsigned r)
  {
    col = r;

    fWidth = (unsigned)((float)width / (float)col);

    float aspect = (float)fWidth / (float)fHeight;
    sprite->SetAspect(aspect);
    // endFrame = numFrames + (startRow * col);

    ResetAnimation();
  }

  float GetFrameTime() const
  {
    return frameTime;
  }

  void SetFrameTime(float t)
  {
    frameTime = t;
  }

  static std::vector<int> UncompFrameOrder(const std::vector<int>& inVal);

  void SetFrameOrderComp(const std::string& key, const std::vector<int>& inVal); // input is frame order in compressed form (uncompressed will result in same though)

  void SetFrameOrderComp(const std::map<std::string, std::vector<int>>& compMap);

  std::string GetAnim() const { return currentAnimKey; }

  void SetAnim(const std::string& key);

  void SetAnim();

private:
  void ResetAnimation();
  std::vector<float> CalculateUV();
  void NextFrame();

  unsigned row, col, currFrame, endFrame;

  float frameTime, currTime;
  unsigned width, height, fWidth, fHeight;

  std::map<std::string, std::vector<int>> frameOrderMap; // literal number list of frames
  std::map<std::string, std::vector<int>> frameOrderCompMap; // compressed version with intervals, -1: marker for interval between next 2
                                                             // ex: 2 4 -1 3 7 expands to 2 4 3 4 5 6 7
  
  std::string currentAnimKey; // current key to frameOrderMap
  std::string defaultAnimKey; // default animation key
  std::vector<int> frameOrderCurrent; // current frames

  Sprite* sprite;

  RTTR_ENABLE(Component)
  RTTR_REGISTRATION_FRIEND
};