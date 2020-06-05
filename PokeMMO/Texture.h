#pragma once

#include <glad/glad.h>
#include <string>

class Texture
{
public:
  Texture(const std::string& filepath);
  ~Texture();

  GLuint GetTexture() const
  {
    return texID;
  }
  
  void SetAspect(float newa)
  {
    aspect = newa;
  }

  float GetAspect() const
  {
    return aspect;
  }

  unsigned GetWidth() const
  {
    return width;
  }

  unsigned GetHeight() const
  {
    return height;
  }

private:
  GLuint texID;
  std::string filepath;

  float aspect;

  unsigned width, height;
};