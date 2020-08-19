#include "FileExplorer.hpp"

#include <imgui.h>
#include <filesystem>

namespace fs = std::filesystem;

FileExplorer::FileExplorer() : foldtex("../Textures/Editor/Folder.png"), filetex("../Textures/Editor/File.png")
{
}

void FileExplorer::Init()
{
  currpath = "../Textures";
  selectedfile = "None";
  open = clicked = false;

  EnterDirectory(currpath);
}

const std::string& FileExplorer::Explorer()
{
  clicked = false;

  char path[2048];
  strcpy(path, selectedfile.c_str());
  ImGui::InputText("Filepath", path, 2048);

  ImGui::SameLine();

  if (ImGui::ImageButton((ImTextureID)foldtex.GetTexture(), ImVec2(30, 30), ImVec2(0, 1), ImVec2(1, 0)))
  {
    open = true;
  }

  if (open)
  {
    clicked = onUpdate();
    if (clicked)
    {
      open = false;
    }
  }

  return selectedfile;
}

void FileExplorer::EnterDirectory(const std::string& path)
{
  currpath = path;
  folders.clear();
  files.clear();
  for (auto& file : fs::directory_iterator(currpath))
  {
    std::string path = file.path().string();
    if (path.find_last_of("/\\") != std::string::npos)
    {
      path.erase(path.begin(), path.begin() + path.find_last_of("/\\") + 1);
    }
      
    if (file.is_directory())
    {
      folders.push_back(path);
    }
    else
    {
      files.push_back(path);
    }
  }
}

bool FileExplorer::onUpdate()
{
  ImGui::Begin("File Explorer");

  auto& io = ImGui::GetIO();
  bool dbl = false;
  static bool buttonlast = false;
  static bool filelast = false;
  static std::string dir;
  bool enter = false;
  bool getfile = false;

  static float timer = .1f;

  if (buttonlast)
  {
    if (timer <= 0)
    {
      buttonlast = false;
      timer = .1f;
    }
    else
    {
      timer -= 1.f / 60.f;
    }
  }

  if (io.MouseDoubleClicked[0])
  {
    dbl = true;
  }

  std::string temp = currpath;
  while (!temp.empty())
  {
    std::string small = temp;
    auto pos = temp.find_first_of("/\\");
    if (pos != std::string::npos)
    {
      small = temp.substr(0, temp.find_first_of("/\\"));

      temp = temp.substr(temp.find_first_of("/\\") + 1);
    }
    else
    {
      temp.clear();
    }
    if (ImGui::Button(small.c_str()))
    {
      currpath = currpath.substr(0, currpath.find(small) + small.size());
      EnterDirectory(currpath);
    }

    ImGui::SameLine();
  }

  ImGui::Separator();

  unsigned count = 0;

  for (int i = 0; i < folders.size(); ++i)
  {
    ImGui::BeginGroup();

    ImGui::PushID(i);

    if (ImGui::ImageButton((ImTextureID)foldtex.GetTexture(), ImVec2(50, 50), ImVec2(0, 1), ImVec2(1, 0)))
    {
      buttonlast = true;
      dir = folders[i];
    }

    if (dbl)
    {
      if (buttonlast)
      {
        enter = true;
        buttonlast = false;
      }
    }

    ImGui::PopID();

    ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 50);
    ImGui::Text(folders[i].c_str());
    ImGui::PopTextWrapPos();

    ImGui::EndGroup();

    if (count++ != 6)
      ImGui::SameLine();
    else
      count = 0;
  }

  static std::string fingle;

  for (int i = 0; i < files.size(); ++i)
  {
    ImGui::BeginGroup();

    ImGui::PushID(i);

    if (ImGui::ImageButton((ImTextureID)filetex.GetTexture(), ImVec2(50, 50), ImVec2(0, 1), ImVec2(1, 0)))
    {
      filelast = true;
      fingle = files[i];
    }

    if (dbl)
    {
      if (filelast)
      {
        selectedfile = currpath + "/" + fingle;
        getfile = true;
        filelast = false;
      }
    }

    ImGui::PopID();

    ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 50);
    ImGui::Text(files[i].c_str());
    ImGui::PopTextWrapPos();

    ImGui::EndGroup();

    if (count++ != 6)
      ImGui::SameLine();
    else
      count = 0;
  }

  if (enter)
  {
    EnterDirectory(currpath + "/" + dir);
  }


  ImGui::End();

  if (getfile)
  {
    return true;
  }

  return false;
}
