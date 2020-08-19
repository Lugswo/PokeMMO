#pragma once

#include <vector>
#include <GLFW/glfw3.h>

#include "System.h"

class InputManager : public System
{
public:
  void Init() override;
  void Update(float dt) override;
  void Shutdown() override;

  bool KeyPress(int key);
  bool KeyPress(int key, int key2);
  bool KeyDown(int key);
  bool KeyDown(int key, int key2);
  bool KeyUp(int key);
  bool KeyUp(int key, int key2);
  bool KeyRelease(int key);
  bool KeyRelease(int key, int key2);

  bool MouseClick(int button);
  bool MouseDown(int button);
  bool MouseUp(int button);
  bool MouseRelease(int button);

  void PushKey(int key);
  void RelKey(int key);

  void PushButton(int button);
  void RelButton(int button);

  static InputManager* GetInstance()
  {
    if (!instance)
    {
      instance = new InputManager();
    }

    return instance;
  }

private:
  class Key
  {
  public:
    Key()
    {
      r = d = p = false;
      u = true;
    }
    bool r, p, u, d;
  };

  InputManager() {};
  ~InputManager() {};

  static InputManager* instance;

  Key keyboard[GLFW_KEY_MENU];
  Key mouse[GLFW_MOUSE_BUTTON_8];

  std::vector<Key *> keysToUpdate;
};