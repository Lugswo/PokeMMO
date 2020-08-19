#include "ShaderManager.h"
#include "Camera.h"

#include <gtc/matrix_transform.hpp>

ShaderManager* ShaderManager::instance;

ShaderManager* ShaderManager::GetInstance()
{
  if (!instance)
  {
    instance = new ShaderManager();
  }

  return instance;
}

void ShaderManager::Init()
{
  persp = glm::mat4(1.f);

  persp = glm::perspective(45.f, 1920.f / 1080.f, .1f, 100.f);
}

void ShaderManager::Update(float dt)
{
  auto view = Camera::GetInstance()->GetCamera();

  for (auto& sh : shaders)
  {
    sh.second->Uniform(view, "view");
    sh.second->Uniform(persp, "persp");
  }
}

void ShaderManager::Shutdown()
{
  for (auto& sh : shaders)
  {
    delete sh.second;
  }
  shaders.clear();
}

const Shader* ShaderManager::GetShader(const std::string& name)
{
  auto sh = shaders.find(name);

  if (sh != shaders.end())
    return sh->second;
  else
    return nullptr;
}

const Shader* ShaderManager::AddShader(const std::string& vs, const std::string& fs, const std::string& name)
{
  Shader* sh = new Shader(vs, fs, name);
  shaders.insert(std::make_pair(name, sh));

  return sh;
}
