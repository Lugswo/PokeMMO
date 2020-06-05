#include "Texture.h"

#include "stb_image.h"
#include "Logger.h"

Texture::Texture(const std::string& filepath)
{
  this->filepath = filepath;
  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_2D, texID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  int width, height, nrChannels;
  unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);

  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    aspect = (float)width / (float)height;
    this->width = (unsigned)width;
    this->height = (unsigned)height;
  }
  else
  {
    L::Log(TL::ERR, "Failed to load texture " + filepath);
  }

  stbi_image_free(data);
}

Texture::~Texture()
{
  glDeleteTextures(1, &texID);
}