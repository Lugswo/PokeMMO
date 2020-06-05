#pragma once

#include "Sprite.h"

#include <rttr/registration_friend.h>

class Animation : public Component
{
public:
  void Init() override;
  void Update(float dt) override;
  void Shutdown() override;

  unsigned GetRows() const
  {
    return row;
  }

  void SetRows(unsigned r)
  {
    row = r;
    numFrames = row * col;

    fHeight = (float)height / (float)row;

    float aspect = (float)fWidth / (float)fHeight;
    sprite->SetAspect(aspect);

    ResetAnimation();
  }

  unsigned GetCols() const
  {
    return col;
  }

  void SetCols(unsigned r)
  {
    col = r;
    numFrames = row * col;

    fWidth = (float)width / (float)col;

    float aspect = (float)fWidth / (float)fHeight;
    sprite->SetAspect(aspect);

    ResetAnimation();
  }

  unsigned GetNumFrames() const
  {
    return numFrames;
  }

  void SetNumFrames(unsigned r)
  {
    numFrames = r;
  }

  float GetFrameTime() const
  {
    return frameTime;
  }

  void SetFrameTime(float t)
  {
    frameTime = t;
  }

private:
  void ResetAnimation();
  std::vector<float> CalculateUV();
  void NextFrame();

  unsigned row, col, numFrames, currFrame;

  float frameTime, currTime;
  unsigned width, height, fWidth, fHeight;

  Sprite* sprite;

  RTTR_ENABLE(Component)
  RTTR_REGISTRATION_FRIEND
};