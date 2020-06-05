#pragma once

#include <map>

#include "Shader.h"
#include "System.h"

class ShaderManager : public System
{
public:
  void Init() override;
  void Update(float dt) override;
  void Shutdown() override;

  static ShaderManager* GetInstance();

  const Shader* GetShader(const std::string& name);
  const Shader* AddShader(const std::string& vs, const std::string& fs, const std::string& name);

private:
  ShaderManager() {};
  ~ShaderManager() {};
  std::map<std::string, Shader*> shaders;

  static ShaderManager* instance;
};