#include "TextureManager.h"

#include "Logger.h"
#include "Texture.h"

TextureManager* TextureManager::instance;

void TextureManager::Init()
{

}

void TextureManager::Update(float dt)
{

}

void TextureManager::Shutdown()
{
  for (auto itr = textures.begin(); itr != textures.end(); ++itr)
  {
    delete itr->second;
  }
}

void TextureManager::AddTexture(const std::string& filepath, Texture* t)
{
  textures.insert(std::make_pair(filepath, t));
}



Texture* TextureManager::GetTexture(const std::string& filepath)
{
  auto itr = textures.find(filepath);
  if (itr != textures.end())
  {
    return itr->second;
  }
  else
  {
    Texture* tex = new Texture(filepath);
    AddTexture(filepath, tex);
    return tex;
  }
}

TextureManager* TextureManager::GetInstance()
{
  if (!instance)
  {
    instance = new TextureManager();
  }

  return instance;
}
