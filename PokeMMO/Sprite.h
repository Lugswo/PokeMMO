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
  void Init() override;
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

  void ChangeUV(const std::vector<float>& uvs);

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

  bool draw;

  const Shader* shader;
  Texture* tex;

  Transform* transform;

  std::string filepath;

  unsigned width, height;

  RTTR_ENABLE(Component)
  RTTR_REGISTRATION_FRIEND
};