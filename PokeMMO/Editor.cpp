#include "Editor.h"

#include <filesystem>

#include "GameObjectFactory.h"
#include "Component.h"
#include "Animation.h"
#include "Serializer.h"
#include "InputManager.h"
#include "Camera.h"

#pragma warning(disable : 4996)

Editor* Editor::instance;

namespace fs = std::experimental::filesystem;

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
  createObject = loadPrefab = false;
  for (auto& p : fs::recursive_directory_iterator("Objects"))
  {
    auto objPos = p.path().string().find(".json");
    if (objPos != std::string::npos)
    {
      std::string str = p.path().string();

      str.erase(str.begin() + objPos, str.end());

      auto pos = str.find_last_of("\\");

      if (pos != std::string::npos)
      {
          //  + 1 because pos sucks xd
        str.erase(str.begin(), str.begin() + pos + 1);
      }

      objPaths.push_back(str);
    }
  }

  rttr::type t = rttr::type::get_by_name("classComponent");
  auto derived = t.get_derived_classes();

  for (auto& comp : derived)
  {
    componentTypes.push_back(comp.get_name().to_string());
  }
}

void Editor::MenuBar()
{
  if (ImGui::BeginMenuBar())
  {
    if (ImGui::BeginMenu("File"))
    {
      if (ImGui::MenuItem("New Object", "CTRL + N"))
      {
        createObject = true;
      }

      if (ImGui::MenuItem("Load Object", "CTRL + SHIFT + N"))
      {
        loadPrefab = true;
      }

      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }

  if (createObject)
  {
    ImGui::OpenPopup("Create New Object");
  }

  if (ImGui::BeginPopupModal("Create New Object", NULL, ImGuiWindowFlags_AlwaysAutoResize))
  {
    char buf[128] = { 0 };
    if (ImGui::InputText("Object Name", buf, 128, ImGuiInputTextFlags_EnterReturnsTrue))
    {
      GameObject* obj = new GameObject(buf);
      obj->EditorName();
      GameObjectFactory::GetInstance()->AddObject(obj);
      obj->SetSaved(false);
      objPaths.push_back(buf);
      ImGui::CloseCurrentPopup();
      createObject = false;
    }

    if (ImGui::Button("Cancel"))
    {
      ImGui::CloseCurrentPopup();
      createObject = false;
    }
    ImGui::EndPopup();
  }

  if (loadPrefab)
  {
    ImGui::OpenPopup("Load Prefab");
  }

  if (ImGui::BeginPopupModal("Load Prefab", NULL, ImGuiWindowFlags_AlwaysAutoResize))
  {
    static ImGuiTextFilter filter;
    filter.Draw();

    static const char* selectedObj = objPaths[0].c_str();

    if (ImGui::BeginCombo("Object Name", selectedObj))
    {
      for (int i = 0; i < objPaths.size(); i++)
      {
        if (filter.PassFilter(objPaths[i].c_str()))
        {
          bool is_selected = (selectedObj == objPaths[i]);
          if (ImGui::Selectable(objPaths[i].c_str(), is_selected))
            selectedObj = objPaths[i].c_str();
          if (is_selected)
            ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
        }
      }

      ImGui::EndCombo();
    }

    if (ImGui::Button("Load"))
    {
      GameObject* obj = GameObjectFactory::GetInstance()->ParseObject(selectedObj);
      obj->EditorName();

      Transform* trans = GetComponent(Transform, obj);
      if (trans)
      {
        glm::vec3 pos = Camera::GetInstance()->GetPosition();
        pos.z = 0.f;
        trans->SetPosition(pos);
      }

      ImGui::CloseCurrentPopup();
      loadPrefab = false;
    }

    if (ImGui::Button("Cancel"))
    {
      ImGui::CloseCurrentPopup();
      loadPrefab = false;
    }
    ImGui::EndPopup();
  }
}

void Editor::Update(float dt)
{
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(ImVec2(500, 1080));
  ImGui::Begin("Outliner", nullptr, ImGuiWindowFlags_MenuBar);

  MenuBar();

  auto objects = GameObjectFactory::GetInstance()->GetAllObjects();

  for (auto& obj : objects)
  {
    bool popped = false;
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1));
    if (obj->GetSaved())
    {
      ImGui::PopStyleColor();
      popped = true;
    }

    std::string objName = obj->GetName();
    objName += ", ";
    objName += obj->GetFilename();

    if (ImGui::TreeNode(objName.c_str()))
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
      if (ImGui::SmallButton("Add Component"))
      {
        ImGui::OpenPopup("Add Component?");
      }

      if (ImGui::BeginPopupModal("Add Component?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
      {
        static ImGuiTextFilter filter;
        filter.Draw();

        static const char* selectedComponent = componentTypes[0].c_str();

        if (ImGui::BeginCombo("Component", selectedComponent))
        {
          for (int i = 0; i < componentTypes.size(); i++)
          {
            if (filter.PassFilter(componentTypes[i].c_str()))
            {
              bool is_selected = (selectedComponent == componentTypes[i]);
              if (ImGui::Selectable(componentTypes[i].c_str(), is_selected))
                selectedComponent = componentTypes[i].c_str();
              if (is_selected)
                ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
            }
          }

          ImGui::EndCombo();
        }

        if (ImGui::Button("Add Component"))
        {
          rttr::type t = rttr::type::get_by_name(selectedComponent);

          rttr::variant* toAdd = new rttr::variant();
          *toAdd = t.create();
          Component* componentPtr = toAdd->get_value<Component*>();

          obj->AddComponent(componentPtr);

          ImGui::CloseCurrentPopup();
          loadPrefab = false;
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
        {
          ImGui::CloseCurrentPopup();
          loadPrefab = false;
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
          ImGui::SameLine();
          if (ImGui::SmallButton("Delete Component"))
          {
            ImGui::OpenPopup("Delete Component?");
          }

          if (ImGui::BeginPopupModal("Delete Component?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
          {
            ImGui::Text("Delete this component?");

            if (ImGui::Button("Yes"))
            {
              c->SetDelete();
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
  if (c->GetComponentName() != "Transform")
  {
    obj->SetSaved(false);
  }
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
