#include "InputManager.h"

#include <GLFW/glfw3.h>

#include "Logger.h"

InputManager* InputManager::instance;

void InputManager::Init()
{
  for (int i = 0; i < GLFW_KEY_MENU; ++i)
  {
    keyboard[i] = Key();
  }

  for (int i = 0; i < GLFW_MOUSE_BUTTON_8; ++i)
  {
    mouse[i] = Key();
  }

  L::Log(TRACE_LEVEL::INFO, "Input manager successfully initiated.");
}

void InputManager::Update(float dt)
{
  int size = static_cast<int>(keysToUpdate.size());
  for (int i = 0; i < size; ++i)
  {
    keysToUpdate[i]->p = keysToUpdate[i]->r = false;
  }
  keysToUpdate.clear();
}

void InputManager::Shutdown()
{

}

bool InputManager::KeyPress(int key)
{
  return keyboard[key].p;
}

//bool InputManager::KeyPress(int key, int key2)
//{
//
//}

bool InputManager::KeyDown(int key)
{
  return keyboard[key].d;
}

//bool InputManager::KeyDown(int key, int key2)
//{
//
//}
//
//bool InputManager::KeyUp(int key)
//{
//
//}
//
//bool InputManager::KeyUp(int key, int key2)
//{
//
//}

bool InputManager::KeyRelease(int key)
{
  return keyboard[key].r;
}

//bool InputManager::KeyRelease(int key, int key2)
//{
//
//}

bool InputManager::MouseClick(int button)
{
  return mouse[button].p;
}

bool InputManager::MouseDown(int button)
{
  return mouse[button].d;
}

//bool InputManager::MouseUp(int button)
//{
//  return 
//}

bool InputManager::MouseRelease(int button)
{
  return mouse[button].r;
}

void InputManager::PushKey(int key)
{
  keyboard[key].p = keyboard[key].d = true;
  keyboard[key].u = false;
  keysToUpdate.push_back(keyboard + key);
}

void InputManager::RelKey(int key)
{
  keyboard[key].r = keyboard[key].u = true;
  keyboard[key].d = false;
  keysToUpdate.push_back(keyboard + key);
}

void InputManager::PushButton(int button)
{
  mouse[button].p = mouse[button].d = true;
  mouse[button].u = false;
  keysToUpdate.push_back(mouse + button);
}

void InputManager::RelButton(int button)
{
  mouse[button].r = mouse[button].u = true;
  mouse[button].d = false;
  keysToUpdate.push_back(mouse + button);
}