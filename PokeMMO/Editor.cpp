#include "Editor.h"

#include "GameObjectFactory.h"
#include "Component.h"
#include "Animation.h"
#include "Serializer.h"
#include "InputManager.h"

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
    bool popped = false;
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1));
    if (obj->GetSaved())
    {
      ImGui::PopStyleColor();
      popped = true;
    }
    if (ImGui::TreeNode(obj->GetName().c_str()))
    {
      if (ImGui::BeginPopupContextItem("Object Menu"))
      {
        if (ImGui::Button("Save"))
        {
          Serializer::Serialize(obj);
          obj->SetSaved(true);
          ImGui::OpenPopup("Saved");
        }

        if (ImGui::BeginPopupModal("Saved", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
          ImGui::Text("Object saved successfully!");

          if (ImGui::Button("Ok"))
          {
            ImGui::CloseCurrentPopup();
          }

          if (InputManager::GetInstance()->KeyPress((GLFW_KEY_ENTER)))
          {
            ImGui::CloseCurrentPopup();
          }
          ImGui::EndPopup();
        }

        ImGui::EndPopup();
      }

      ImGui::SameLine();
      if (ImGui::SmallButton("Delete")) 
      {
        ImGui::OpenPopup("Delete?");
      }

      if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
      {
        ImGui::Text("Delete this object?");

        if (ImGui::Button("Yes"))
        {
          obj->SetDelete();
          ImGui::CloseCurrentPopup();
        }
        else if (ImGui::Button("No"))
        {
          ImGui::CloseCurrentPopup();
        }

        if (InputManager::GetInstance()->KeyPress((GLFW_KEY_ENTER)))
        {
          ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
      }

      for (auto& c : obj->GetComponents())
      {
        if (ImGui::TreeNode(c->GetName().c_str()))
        {
          char buf[128];
          strcpy(buf, c->GetName().c_str());
          if (ImGui::InputText("Name", buf, 128, ImGuiInputTextFlags_EnterReturnsTrue))
          {
            c->SetName(buf);
            obj->SetSaved(false);
          }

          rttr::type t = rttr::type::get_by_name(c->GetComponentName());

          for (auto& p : t.get_properties())
          {
            SetVariable(p, c);
          }

          ImGui::TreePop();
        }
      }
      ImGui::TreePop();
    }

    if (!popped)
    {
      ImGui::PopStyleColor();
    }
  }

  ImGui::End();
}

template <typename T>
void SetValue(const rttr::property& p, Component* c, T value)
{
  GameObject* obj = c->GetParent();
  obj->SetSaved(false);
  p.set_value(c, value);
}

void Editor::SetVariable(const rttr::property& p, Component* c)
{
  rttr::variant r = p.get_value(c);
  if (r.is_type<int>())
  {
    int i = r.get_value<int>();
    if (ImGui::InputInt(p.get_name().to_string().c_str(), &i))
    {
      SetValue(p, c, i);
    }
  }
  else if (r.is_type<unsigned>())
  {
    int i = r.get_value<unsigned>();
    if (ImGui::InputInt(p.get_name().to_string().c_str(), &i))
    {
      unsigned u = static_cast<unsigned>(i);
      SetValue(p, c, u);
    }
  }
  else if (r.is_type<float>())
  {
    float i = r.get_value<float>();
    if (ImGui::InputFloat(p.get_name().to_string().c_str(), &i))
    {
      SetValue(p, c, i);
    }
  }
  else if (r.is_type<bool>())
  {
    bool i = r.get_value<bool>();
    if (ImGui::Checkbox(p.get_name().to_string().c_str(), &i))
    {
      SetValue(p, c, i);
    }
  }
  else if (r.is_type<double>())
  {
    double i = r.get_value<double>();
    if (ImGui::InputDouble(p.get_name().to_string().c_str(), &i))
    {
      SetValue(p, c, i);
    }
  }
  else if (r.is_type<glm::vec3>())
  {
    glm::vec3 prev = r.get_value<glm::vec3>();
    float f[3];

    f[0] = prev.x;
    f[1] = prev.y;
    f[2] = prev.z;

    if (ImGui::InputFloat3(p.get_name().to_string().c_str(), f, 2))
    {
      glm::vec3 v(f[0], f[1], f[2]);
      SetValue(p, c, v);
    }
  }
  else if (r.is_type<glm::vec2>())
  {
    glm::vec2 prev = r.get_value<glm::vec2>();
    float f[2];

    f[0] = prev.x;
    f[1] = prev.y;

    if (ImGui::InputFloat2(p.get_name().to_string().c_str(), f, 2))
    {
      glm::vec2 v(f[0], f[1]);
      SetValue(p, c, v);
    }
  }
  else if (r.is_type<std::string>())
  {
    std::string s = r.get_value<std::string>();
    char buf[128];
    strcpy(buf, s.c_str());

    if (ImGui::InputText(p.get_name().to_string().c_str(), buf, 128, ImGuiInputTextFlags_EnterReturnsTrue))
    {
      std::string s2 = buf;
      SetValue(p, c, s2);
    }
  }
}

void Editor::Shutdown()
{

}
