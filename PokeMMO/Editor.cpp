#include "Editor.h"

#include "GameObjectFactory.h"
#include "Component.h"
#include "Animation.h"
#include "Serializer.h"

#pragma warning(disable : 4996)

Editor* Editor::instance;

Editor* Editor::GetInstance()
{
  if (!instance)
  {
    instance = new Editor();
  }

  return instance;
}

void Editor::Init()
{

}

void Editor::Update(float dt)
{
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(ImVec2(500, 1080));
  ImGui::Begin("Outliner");

  GameObjectFactory* gof = GameObjectFactory::GetInstance();

  auto objects = gof->GetInstance()->GetAllObjects();

  for (auto& obj : objects)
  {
    if (ImGui::TreeNode(obj->GetName().c_str()))
    {
      for (auto& c : obj->GetComponents())
      {
        if (ImGui::TreeNode(c->GetName().c_str()))
        {
          char buf[128];
          strcpy(buf, c->GetName().c_str());
          if (ImGui::InputText("Name", buf, 128, ImGuiInputTextFlags_EnterReturnsTrue))
          {
            c->SetName(buf);
          }

          rttr::type t = rttr::type::get_by_name(c->GetComponentName());

          for (auto& p : t.get_properties())
          {
            ImGui::Text(p.get_name().to_string().c_str());
          }

          ImGui::TreePop();
        }
      }
      ImGui::TreePop();
    }
  }
  
  ImGui::End();
}

void Editor::SetVariable(rttr::variant& r)
{
  //if (r.is_type<int>())
  //{
  //  r.get_type().get_name();
  //  int i = comp[p.get_name().to_string().c_str()].GetInt();
  //  p.set_value(cVar, i);
  //}
  //else if (r.is_type<float>())
  //{
  //  float i = comp[p.get_name().to_string().c_str()].GetFloat();
  //  p.set_value(cVar, i);
  //}
  //else if (r.is_type<bool>())
  //{
  //  bool i = comp[p.get_name().to_string().c_str()].GetBool();
  //  p.set_value(cVar, i);
  //}
  //else if (r.is_type<double>())
  //{
  //  double i = comp[p.get_name().to_string().c_str()].GetDouble();
  //  p.set_value(cVar, i);
  //}
  //else if (r.is_type<unsigned>())
  //{
  //  unsigned i = comp[p.get_name().to_string().c_str()].GetUint();
  //  p.set_value(cVar, i);
  //}
  //else if (r.is_type<glm::vec3>())
  //{
  //  glm::vec3 v;
  //  unsigned count = 0;
  //  std::string name = p.get_name().to_string();
  //  const char* c = name.c_str();

  //  for (auto& f : comp[c].GetArray())
  //  {
  //    v[count++] = f.GetFloat();
  //  }

  //  p.set_value(cVar, v);
  //}
  //else if (r.is_type<glm::vec2>())
  //{
  //  auto arr = comp[p.get_name().to_string().c_str()].GetArray();
  //  glm::vec3 v;
  //  unsigned count = 0;

  //  for (auto& f : arr)
  //  {
  //    v[count] = f.GetFloat();
  //  }

  //  p.set_value(cVar, v);
  //}
  //else if (r.is_type<std::string>())
  //{
  //  std::string i = comp[p.get_name().to_string().c_str()].GetString();
  //  p.set_value(cVar, i);
  //}
}

void Editor::Shutdown()
{

}
