#include "Engine.h"

#include <thread>
#include <iostream>
#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>

#include "GraphicsEngine.h"
#include "Logger.h"

#include "ShaderManager.h"
#include "GameObjectFactory.h"
#include "Camera.h"
#include "InputManager.h"
#include "Serializer.h"
#include "Editor.h"

  //  temporary adds
#include "Sprite.h"
#include "Player.h"
#include "Animation.h"

constexpr float nano = 1000000000.f;

std::chrono::time_point<std::chrono::high_resolution_clock> Engine::beginFrame;
bool Engine::running;
float Engine::dt = 0.f;
std::vector<System*> Engine::systems;

GameObject *obj = new GameObject("Test");

//GameObject *obj2 = new GameObject("verizon");

void Engine::Init()
{
  L::Init(TL::VERBOSE);
  L::Log(TL::INFO, "Logger initialized!");

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();

  running = true;
  
  AddSystem<GameObjectFactory>();
  AddSystem<Editor>();
  AddSystem<Camera>();

  AddSystem<ShaderManager>();
    //  graphics goes last pls don't forget
  AddSystem<GraphicsEngine>();
  AddSystem<InputManager>();

  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(GraphicsEngine::GetInstance()->GetWindow()->GetWindow(), true);
  ImGui_ImplOpenGL3_Init("#version 460");
  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  ShaderManager::GetInstance()->AddShader("FSQ.vs", "FSQ.fs", "FSQ");
  ShaderManager::GetInstance()->AddShader("Normal.vs", "Normal.fs", "Normal");

  
  obj->AddComponent<Transform>();
  obj->AddComponent<Sprite>("../Textures/player.png");

  /*
  obj2->AddComponent<Transform>();
  obj2->AddComponent<Sprite>("../Textures/Pokemon/Virizion.png");
  obj2->AddComponent<Animation>();
  */

  //Transform* trubo = GetComponent(Transform, obj2);
  //trubo->SetScale(glm::vec3(.5, .5, 1));

  

  GameObjectFactory::GetInstance()->AddObject(obj);
  //GameObjectFactory::GetInstance()->AddObject(obj2);
  GameObjectFactory::GetInstance()->ParseObject("player");
  GameObjectFactory::GetInstance()->ParseObject("verizon");

  /*
  Animation* an = GetComponent(Animation, obj2);
  an->SetRows(11);
  an->SetCols(9);
  an->SetNumFrames(93);
  an->SetFrameTime(.1f);
  an->SetFrameOrderComp({ -1,0,93 });
  */

}

void Engine::Update()
{
  while (!GraphicsEngine::GetInstance()->Closed())
  {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    beginFrame = std::chrono::high_resolution_clock::now();
    using namespace std::chrono_literals;

    /*
    if (InputManager::GetInstance()->KeyPress(GLFW_KEY_SLASH))
    {
      Serializer::Serialize(obj2);
    }
    */

    for (auto sys : systems)
    {
      sys->Update(dt);
    }

    std::this_thread::sleep_until(beginFrame + (1000ms / 60));

    //auto framecount = std::chrono::high_resolution_clock::now();
    //while (framecount < (beginFrame + (1000ms / 60)))
    //{
    //  framecount = std::chrono::high_resolution_clock::now();
    //}

    auto beginMS = std::chrono::time_point_cast<std::chrono::milliseconds>(beginFrame);

    auto endFrame = std::chrono::high_resolution_clock::now();
    auto dtChrono = endFrame - beginFrame;
    dt = dtChrono.count() / nano;
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

template <typename T>
void Engine::AddSystem()
{
  T* sys = T::GetInstance();
  sys->Init();
  systems.push_back(sys);

  std::string name = typeid(T).name();
  name += " added!";

  L::Log(TL::INFO, name);
}

void Engine::Shutdown()
{
  for (auto sys : systems)
  {
    sys->Shutdown();
  }
}
