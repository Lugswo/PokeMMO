#include "Sprite.h"

#include <rttr/registration.h>

#include "GraphicsEngine.h"
#include "ShaderManager.h"

void Sprite::Init()
{
  SetName("Sprite");
  draw = true;

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
  //// texture coord attribute
  //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  //glEnableVertexAttribArray(2);

  shader = ShaderManager::GetInstance()->GetShader("Normal");

  tex = new Texture(filepath);

  transform = GetComponent(Transform, parent);

  transform->SetTextureScale(tex->GetAspect());

  width = tex->GetWidth();
  height = tex->GetHeight();
}

void Sprite::ParseInit()
{
  SetName("Sprite");
  draw = true;

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
  //// texture coord attribute
  //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  //glEnableVertexAttribArray(2);

  shader = ShaderManager::GetInstance()->GetShader("Normal");

  tex = new Texture(filepath);

  transform = GetComponent(Transform, parent);

  transform->SetTextureScale(tex->GetAspect());

  width = tex->GetWidth();
  height = tex->GetHeight();
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

  vertices[8] = uvs[2];
  vertices[9] = uvs[3];

  vertices[13] = uvs[4];
  vertices[14] = uvs[5];

  vertices[18] = uvs[6];
  vertices[19] = uvs[7];

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
}

RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<Sprite>("Sprite")
        .constructor<>()
        (

        )
      .property("filepath", &Sprite::filepath)
      ;
}