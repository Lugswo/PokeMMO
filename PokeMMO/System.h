#pragma once

class System
{
public:
  System() {};
  ~System() {};

  virtual void Init() {};
  virtual void Update(float dt) {};
  virtual void Shutdown() {};

private:
};