#pragma once

#include <map>
#include <string>

#include "System.h"

class Texture;

class TextureManager : public System
{
public:
  void Init() override;
  void Update(float dt) override;
  void Shutdown() override;

  void AddTexture(const std::string&, Texture* t);
  Texture* GetTexture(const std::string&);

  static TextureManager* GetInstance();

private:
  TextureManager() {};
  ~TextureManager();

  std::map<const std::string, Texture*> textures;

  static TextureManager* instance;
};