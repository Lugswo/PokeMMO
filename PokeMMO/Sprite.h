#pragma once

#include <glad/glad.h>

#include "Component.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"

#include <rttr/registration_friend.h>

class Sprite : public Component
{
public:
  Sprite();

  void Init() override;
  void ParseInit() override;
  void Update(float dt) override;
  void Shutdown() override;

  void SetDepression(const std::string& str) override
  {
    filepath = str;
  }

  const Shader* GetShader() const
  {
    return shader;
  }

  unsigned GetVAO() const
  {
    return VAO;
  }

  unsigned GetVBO() const
  {
    return VBO;
  }

  unsigned GetEBO() const
  {
    return EBO;
  }

  const Texture* GetTexture() const
  {
    return tex;
  }

  Transform* GetTransform() const
  {
    return transform;
  }

  const unsigned GetWidth() const
  {
    return width;
  }

  const unsigned GetHeight() const
  {
    return height;
  }

  void SetAspect(float a)
  {
    tex->SetAspect(a);
    transform->SetTextureScale(tex->GetAspect());
  }

  const std::string& GetFilepath() const
  {
    return filepath;
  }

  void SetFilepath(const std::string& file)
  {
    filepath = file;

    SetTexture();
  }

  void SetTexture();

  const glm::vec2& GetU() const
  {
    return u;
  }

  void SetU(const glm::vec2& uu)
  {
    u = uu;
    ChangeUV(u, v);
  }

  const glm::vec2& GetV() const
  {
    return v;
  }

  void SetV(const glm::vec2& vv)
  {
    v = vv;
    ChangeUV(u, v);
  }

  void ChangeUV(const std::vector<float>& uvs);
  void ChangeUV(const glm::vec2& u, const glm::vec2& v);

private:
  unsigned VAO, VBO, EBO;
  float vertices[20] = 
  {
    // positions            // texture coords
     1.0f,  1.0f, 0.0f,     1.0f, 1.0f,   // top right
     1.0f, -1.0f, 0.0f,     1.0f, 0.0f,   // bottom right
    -1.0f, -1.0f, 0.0f,     0.0f, 0.0f,   // bottom left
    -1.0f,  1.0f, 0.0f,     0.0f, 1.0f    // top left 
  };

  unsigned int indices[6] = 
  {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
  };

  void SetupBuffers();

  bool draw;

  const Shader* shader;
  std::string shaderName;
  Texture* tex;

  Transform* transform;

  std::string filepath;

  unsigned width, height;

  glm::vec2 u, v;

  RTTR_ENABLE(Component)
  RTTR_REGISTRATION_FRIEND
};