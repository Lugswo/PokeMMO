#pragma once

#include <string>
#include <glm.hpp>

class Shader
{
public:
  Shader(const std::string& vs, const std::string& fs, const std::string& name);
  Shader(const std::string& n, const std::string& name);
  ~Shader();

  void Use() const;

  void Uniform(const glm::mat4& mat, const std::string& name) const;
  void Uniform(const int i, const std::string& name) const;
  void Uniform(const unsigned u, const std::string& name) const;
  void Uniform(const float f, const std::string& name) const;
  void Uniform(const glm::vec3 &v, const std::string& name) const;

private:
  std::string name;

  unsigned id;
};