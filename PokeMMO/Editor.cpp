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
  obj = nullptr;
}

void Editor::Update(float dt)
{
  ImGui::Begin("Demo window");

  char buf[100] = { 0 };
  
  if (obj)
  {
    strcpy(buf, obj->GetName().c_str());
  }

  if (ImGui::InputText("Object", buf, 100, ImGuiInputTextFlags_EnterReturnsTrue))
  {
    GameObject* o = GameObjectFactory::GetInstance()->FindObject(buf);

    if (o)
    {
      obj = o;
    }
  }

  if (obj)
  {
    Animation* anim = GetComponent(Animation, obj);
    int r = anim->GetRows();
    if (ImGui::InputInt("Rows", &r))
    {
      anim->SetRows(r);
    }

    int c = anim->GetCols();
    if (ImGui::InputInt("Columns", &c))
    {
      anim->SetCols(c);
    }

    int n = anim->GetNumFrames();
    if (ImGui::InputInt("Frame Count", &n))
    {
      anim->SetNumFrames(n);
    }

    float t = anim->GetFrameTime();
    if (ImGui::InputFloat("Frame Timer", &t))
    {
      anim->SetFrameTime(t);
    }

    int start = anim->GetStartRow();
    if (ImGui::InputInt("Start Row", &start))
    {
      anim->SetStartRow(start);
    }

    if (ImGui::Button("Save"))
    {
      Serializer::Serialize(obj);
    }

    //for (auto& c : obj->GetComponents())
    //{
    //  if (ImGui::TreeNode(c->GetName().c_str()))
    //  {
    //    rttr::type t = rttr::type::get_by_name(c->GetComponentName());

    //    //for (auto& p : t.get_properties())
    //    //{
    //    //  
    //    //}

    //    ImGui::TreePop();
    //  }
    //}
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
