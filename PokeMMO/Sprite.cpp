#include "Sprite.h"

#include <rttr/registration.h>

#include "GraphicsEngine.h"
#include "ShaderManager.h"
#include "TextureManager.h"

Sprite::Sprite()
{
  SetComponentName("Sprite");
  u = glm::vec2(0);
  v = glm::vec2(1);
}

void Sprite::SetupBuffers()
{
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STREAM_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
}

void Sprite::Init()
{
  ParseInit();
}

void Sprite::ParseInit()
{
  draw = true;

  // texture coord attribute
  //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  //glEnableVertexAttribArray(2);

  SetupBuffers();

    //  TODO: replace this with shaderName
  shader = ShaderManager::GetInstance()->GetShader("Normal");

  if (!tex)
    SetTexture();
  else
  {
    transform = GetComponent(Transform, parent);
    transform->SetTextureScale(tex->GetAspect());

    width = tex->GetWidth();
    height = tex->GetHeight();
  }
}

void Sprite::Update(float dt)
{
  if (draw)
  {
    GraphicsEngine::GetInstance()->AddToQueue(this);
  }
}

void Sprite::Shutdown()
{
  
}

void Sprite::ChangeUV(const std::vector<float>& uvs)
{
  vertices[3] = uvs[0];
  vertices[4] = uvs[1];

  float right, top;
  right = uvs[0];
  top = uvs[1];

  v.x = right;
  v.y = top;

  vertices[8] = uvs[2];
  vertices[9] = uvs[3];

  vertices[13] = uvs[4];
  vertices[14] = uvs[5];

  float left, bot;
  left = uvs[4];
  bot = uvs[5];

  u.x = left;
  u.y = bot;

  vertices[18] = uvs[6];
  vertices[19] = uvs[7];

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

  float w, h;
  w = width * (right - left);
  h = height * (top - bot);
  transform->SetTextureScale(w / h);
}

void Sprite::ChangeUV(const glm::vec2& u, const glm::vec2& v)
{
  vertices[3] = v.x;
  vertices[4] = v.y;

  vertices[8] = v.x;
  vertices[9] = u.y;

  vertices[13] = u.x;
  vertices[14] = u.y;

  vertices[18] = u.x;
  vertices[19] = v.y;

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

  float w, h;
  w = width * (v.x - u.x);
  h = height * (v.y - u.y);
  transform->SetTextureScale(w / h);
}

void Sprite::SetTexture()
{
  tex = TextureManager::GetInstance()->GetTexture(filepath);

  if (parent)
  {
    transform = GetComponent(Transform, parent);
    transform->SetTextureScale(tex->GetAspect());

    width = tex->GetWidth();
    height = tex->GetHeight();
  }
}

RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<Sprite>("Sprite")
        .constructor<>()
        (

        )
      .property("filepath", &Sprite::GetFilepath, &Sprite::SetFilepath)
      .property("u", &Sprite::GetU, &Sprite::SetU)
      .property("v", &Sprite::GetV, &Sprite::SetV)
      ;
}