#include "Serializer.h"

#include <filereadstream.h>
#include <filewritestream.h>
#include <document.h>
#include <error/en.h>

#include "GameObject.h"
#include "Component.h"
#include "Logger.h"
#include "GameObjectFactory.h"
#include "Map.h"
#include "Transform.h"
#include "Sprite.h"
#include "Tile.h"
#include "Editor.h"

#pragma warning(disable : 4996)

#define cSer comp[p.get_name().to_string().c_str()]

void Serializer::SerializeObject(const GameObject* obj, rapidjson::PrettyWriter<rapidjson::FileWriteStream>& w)
{
  w.StartObject();
  w.Key("name");
  w.String(obj->GetName().c_str());

  w.Key("components");
  w.StartArray();

  for (auto& c : obj->GetComponents())
  {
    w.StartObject();
    w.Key("name");
    w.String(c->GetComponentName().c_str());

    w.Key("compName");
    w.String(c->GetName().c_str());

    rttr::type t = rttr::type::get_by_name(c->GetComponentName());
    rttr::variant var = c;

    for (auto& prop : t.get_properties())
    {
      w.Key(prop.get_name().to_string().c_str());
      auto value = prop.get_value(var);
      Serialize(w, value);
    }

    w.EndObject();
  }

  w.EndArray();

  w.EndObject();
}

void Serializer::Serialize(const GameObject* obj)
{
  using namespace rapidjson;

  FILE* fp = fopen("Objects/temp.json", "w");

  if (!fp)
  {
    L::Log(TL::ERR, "Could not save file " + obj->GetFilename() + "!");
  }

  char writeBuffer[4096];
  FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

  PrettyWriter<FileWriteStream> w(os);

  SerializeObject(obj, w);

  std::string filename = "Objects/";
  filename += obj->GetFilename();
  filename += ".json";

  if (fp)
    fclose(fp);

  if (remove(filename.c_str()) != 0)
    L::Log(TL::ERR, "Failed to delete file " + filename + "!");

  if (std::rename("Objects/temp.json", filename.c_str()) != 0)
    L::Log(TL::ERR, "Failed to rename file " + filename + "!");
}

void Serializer::SerializeTile(const GameObject* tile, rapidjson::PrettyWriter<rapidjson::FileWriteStream>& w)
{
  Transform* trans = GetComponent(Transform, tile);
  glm::vec3 pos = trans->GetPosition();

  Sprite* sprite = GetComponent(Sprite, tile);
  std::string filepath = sprite->GetFilepath();

  glm::vec2 u, v;
  u = sprite->GetU();
  v = sprite->GetV();

  w.StartObject();

  w.Key("pos");
  Serialize(w, pos);

  w.Key("file");
  Serialize(w, filepath);
  
  w.Key("u");
  Serialize(w, u);

  w.Key("v");
  Serialize(w, v);

  w.EndObject();
}

void Serializer::SerializeMap(const std::string& name)
{
  using namespace rapidjson;

  FILE* fp = fopen("Objects/temp.json", "w");

  if (!fp)
  {
    L::Log(TL::ERR, "Could not save file " + name + "!");
  }

  char writeBuffer[4096];
  FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

  PrettyWriter<FileWriteStream> w(os);
  w.StartObject();
  w.Key("name");
  w.String(name.c_str());

  w.Key("tiles");
  w.StartArray();

  auto tiles = GameObjectFactory::GetInstance()->GetAllTiles();

  for (auto obj : tiles)
  {
    SerializeTile(obj, w);
  }

  w.EndArray();
  
  w.Key("objects");
  w.StartArray();

  auto objects = GameObjectFactory::GetInstance()->GetAllLevelObjects();

  for (auto obj : objects)
  {
    w.String(obj->GetFilename().c_str());
  }

  w.EndArray();

  w.EndObject();

  std::string filename = "Maps/";
  filename += name;
  filename += ".json";

  if (fp)
    fclose(fp);

  if (remove(filename.c_str()) != 0)
    L::Log(TL::ERR, "Failed to delete file " + filename + "!");

  if (std::rename("Objects/temp.json", filename.c_str()) != 0)
    L::Log(TL::ERR, "Failed to rename file " + filename + "!");

}

void ConstructTiles(const rapidjson::Value& a, std::vector<std::vector<GameObject*>>& placedTiles)
{
  using namespace rapidjson;
  for (SizeType i = 0; i < a.Size(); i++)
  {
    GameObject* obj = new GameObject("Tile");
    Transform* trans = new Transform();
    Sprite* sprite = new Sprite();
    Tile* tile = new Tile();

    obj->AddComponent(trans);
    obj->AddComponent(sprite);
    obj->AddComponent(tile);

    obj->SetShowEditor(false);

    trans->SetScale(glm::vec3(.075f, .075f, 0.f));

    const Value& pos = a[i]["pos"];

    float f[3];
    for (SizeType j = 0; j < pos.Size(); ++j)
    {
      f[j] = pos[j].GetFloat();
    }

    glm::vec3 position(f[0], f[1], f[2]);
    trans->SetPosition(position);

    std::string filepath = a[i]["file"].GetString();
    sprite->SetFilepath(filepath);

    const Value& jsonU = a[i]["u"];

    for (SizeType j = 0; j < jsonU.Size(); ++j)
    {
      f[j] = jsonU[j].GetFloat();
    }

    glm::vec2 u(f[0], f[1]);
    sprite->SetU(u);

    const Value& jsonV = a[i]["v"];

    for (SizeType j = 0; j < jsonV.Size(); ++j)
    {
      f[j] = jsonV[j].GetFloat();
    }

    glm::vec2 v(f[0], f[1]);
    sprite->SetV(v);

    GameObjectFactory::GetInstance()->AddObject(obj);

    glm::vec2 findTile = trans->GetPosition();
    findTile.x /= .15f;
    findTile.y /= .15f;

    findTile.x = std::round(findTile.x);
    findTile.y = std::round(findTile.y);

    int x, y;
    x = findTile.x;
    y = -findTile.y;

    x += 500;
    y += 500;

    findTile *= .15f;

    if (!placedTiles[x][y])
    {
      Tile* t = GetComponent(Tile, obj);
      placedTiles[x][y] = obj;

      if (placedTiles[x + 1][y])
      {
        Tile* t2 = GetComponent(Tile, placedTiles[x + 1][y]);
        t->SetRight(t2);
        t2->SetLeft(t);
      }
      if (placedTiles[x - 1][y])
      {
        Tile* t2 = GetComponent(Tile, placedTiles[x - 1][y]);
        t->SetLeft(t2);
        t2->SetRight(t);
      }
      if (placedTiles[x][y + 1])
      {
        Tile* t2 = GetComponent(Tile, placedTiles[x][y + 1]);
        t->SetDown(t2);
        t2->SetUp(t);
      }
      if (placedTiles[x][y - 1])
      {
        Tile* t2 = GetComponent(Tile, placedTiles[x][y - 1]);
        t->SetUp(t2);
        t2->SetDown(t);
      }
    }
  }
}

Map* Serializer::ParseMap(const std::string& file)
{
  using namespace rapidjson;

  Document doc;
  std::string filepath = "Maps/";
  filepath += file;
  filepath += ".json";

  FILE* fp = fopen(filepath.c_str(), "rb"); // non-Windows use "r"

  char readBuffer[65536];
  FileReadStream is(fp, readBuffer, sizeof(readBuffer));

  doc.ParseStream(is);

  Map* map = nullptr;

  //assert(doc.HasMember("name"));
  //assert(doc["name"].IsString());
  map = new Map();

  assert(doc.HasMember("tiles"));
  assert(doc["tiles"].IsArray());

  const Value& a = doc["tiles"];
  assert(a.IsArray());

  if (Editor::GetInstance())
  {
    auto& placedTiles = Editor::GetInstance()->GetPlacedTiles();

    Editor::GetInstance()->ResetTiles();

    ConstructTiles(a, placedTiles);
  }
  else
  {
    std::vector<std::vector<GameObject*>> placedTiles;
    placedTiles.reserve(1000);
    for (int i = 0; i < 1000; ++i)
    {
      placedTiles.push_back(std::vector<GameObject*>());
      placedTiles.back().reserve(1000);

      for (int j = 0; j < 1000; ++j)
      {
        placedTiles.back().push_back(nullptr);
      }
    }

    ConstructTiles(a, placedTiles);
  }

  if (fp)
    fclose(fp);

  return nullptr;
}

GameObject* Serializer::Parse(const std::string& file)
{
  using namespace rapidjson;

  Document doc;
  std::string filepath = "Objects/";
  filepath += file;
  filepath += ".json";

  FILE* fp = fopen(filepath.c_str(), "rb"); // non-Windows use "r"

  char readBuffer[65536];
  FileReadStream is(fp, readBuffer, sizeof(readBuffer));

  doc.ParseStream(is);

  GameObject* obj = nullptr;
 
  assert(doc.HasMember("name"));
  assert(doc["name"].IsString());
  obj = new GameObject(doc["name"].GetString());

  assert(doc.HasMember("components"));
  assert(doc["components"].IsArray());

  const Value& a = doc["components"];
  assert(a.IsArray());
  for (SizeType i = 0; i < a.Size(); i++)
  {
    std::string name = a[i]["name"].GetString();
    rttr::type t = rttr::type::get_by_name(name);

    rttr::variant* var = new rttr::variant();
    *var = t.create();
    rttr::property n = t.get_property("name");
    n.set_value(*var, a[i]["name"].GetString());

    for (auto& p : t.get_properties())
    {
      rttr::type cType = p.get_type();
      rttr::variant cVar = p.get_value(*var);

      Parse(*var, a[i], cVar, p);
    }

    obj->AddComponent((*var).convert<Component*>());
  }

  if (fp)
    fclose(fp);

  return obj;
}

void Serializer::Serialize(rapidjson::PrettyWriter<rapidjson::FileWriteStream>& w, rttr::variant& r)
{
  if (r.is_type<int>())
  {
    int i = r.get_value<int>();
    Serialize(w, i);
  }
  else if (r.is_type<float>())
  {
    float i = r.get_value<float>();
    Serialize(w, i);
  }
  else if (r.is_type<bool>())
  {
    bool i = r.get_value<bool>();
    Serialize(w, i);
  }
  else if (r.is_type<double>())
  {
    double i = r.get_value<double>();
    Serialize(w, i);
  }
  else if (r.is_type<unsigned>())
  {
    unsigned i = r.get_value<unsigned>();
    Serialize(w, i);
  }
  else if (r.is_type<glm::vec3>())
  {
    glm::vec3 i = r.get_value<glm::vec3>();
    Serialize(w, i);
  }
  else if (r.is_type<glm::vec2>())
  {
    glm::vec2 i = r.get_value<glm::vec2>();
    Serialize(w, i);
  }
  else if (r.is_type<std::string>())
  {
    std::string i = r.get_value<std::string>();
    Serialize(w, i);
  }
  else if (r.is_type<std::vector<int>>())
  {
    std::vector<int> i = r.get_value<std::vector<int>>();
    Serialize(w, i);
  }
  else if (r.is_type<std::map<std::string, std::vector<int>>>())
  {
    auto i = r.get_value<std::map<std::string, std::vector<int>>>();
    Serialize(w, i);
  }
}

void Serializer::Parse(rttr::variant& cVar, const rapidjson::Value& comp, rttr::variant& r, const rttr::property& p)
{
  if (comp.HasMember(p.get_name().to_string().c_str()))
  {
    if (r.is_type<int>())
    {
      if (cSer.IsInt())
      {
        int i = cSer.GetInt();
        p.set_value(cVar, i);
      }
    }
    else if (r.is_type<float>())
    {
      if (cSer.IsFloat())
      {
        float i = cSer.GetFloat();
        p.set_value(cVar, i);
      }
    }
    else if (r.is_type<bool>())
    {
      if (cSer.IsBool())
      {
        bool i = cSer.GetBool();
        p.set_value(cVar, i);
      }
    }
    else if (r.is_type<double>())
    {
      if (cSer.IsDouble())
      {
        double i = cSer.GetDouble();
        p.set_value(cVar, i);
      }
    }
    else if (r.is_type<unsigned>())
    {
      if (cSer.IsUint())
      {
        unsigned i = cSer.GetUint();
        p.set_value(cVar, i);
      }
    }
    else if (r.is_type<glm::vec3>())
    {
      glm::vec3 v;
      unsigned count = 0;
      std::string name = p.get_name().to_string();
      const char* c = name.c_str();

      if (cSer.IsArray())
      {
        for (auto& f : comp[c].GetArray())
        {
          v[count++] = f.GetFloat();
        }

        p.set_value(cVar, v);
      }
    }
    else if (r.is_type<glm::vec2>())
    {
      if (cSer.IsArray())
      {
        auto arr = cSer.GetArray();
        glm::vec3 v;
        unsigned count = 0;

        for (auto& f : arr)
        {
          v[count++] = f.GetFloat(); // I changed it to count++ since it probably should have been - Alex
        }

        p.set_value(cVar, v);
      }
    }
    else if (r.is_type<std::string>())
    {
      if (cSer.IsString())
      {
        std::string i = cSer.GetString();
        p.set_value(cVar, i);
      }
    }
    else if (r.is_type<std::vector<int>>())
    {
      if (cSer.IsArray())
      {
        auto arr = cSer.GetArray();
        std::vector<int> v;

        for (auto& f : arr)
        {
          v.emplace_back(f.GetInt());
        }
        p.set_value(cVar, v);
      }
    }
    else if (r.is_type<std::map<std::string, std::vector<int>>>())
    {
      if (cSer.IsArray())
      {
        auto arr1 = cSer.GetArray();
        std::map<std::string, std::vector<int>> m;

        for (auto& mapVal : arr1)
        {
          if (mapVal.IsObject() && mapVal["vector"].IsArray())
          {
            auto key = mapVal["key"].GetString();
            std::vector<int> vec;
            for (auto& val : mapVal["vector"].GetArray())
            {
              vec.emplace_back(val.GetInt());
            }
            m.emplace(key, vec);
          }
        }
        p.set_value(cVar, m);
      }
    }
  }
}

void Serializer::Serialize(rapidjson::PrettyWriter<rapidjson::FileWriteStream>& w, const int i)
{
  w.Int(i);
}

void Serializer::Serialize(rapidjson::PrettyWriter<rapidjson::FileWriteStream>& w, const float f)
{
  w.Double(f);
}

void Serializer::Serialize(rapidjson::PrettyWriter<rapidjson::FileWriteStream>& w, const bool b)
{
  w.Bool(b);
}

void Serializer::Serialize(rapidjson::PrettyWriter<rapidjson::FileWriteStream>& w, const double d)
{
  w.Double(d);
}

void Serializer::Serialize(rapidjson::PrettyWriter<rapidjson::FileWriteStream>& w, const unsigned u)
{
  w.Uint(u);
}

void Serializer::Serialize(rapidjson::PrettyWriter<rapidjson::FileWriteStream>& w, const glm::vec3& v)
{
  w.StartArray();
  w.Double(v.x);
  w.Double(v.y);
  w.Double(v.z);
  w.EndArray();
}

void Serializer::Serialize(rapidjson::PrettyWriter<rapidjson::FileWriteStream>& w, const glm::vec2& v)
{
  w.StartArray();
  w.Double(v.x);
  w.Double(v.y);
  w.EndArray();
}

void Serializer::Serialize(rapidjson::PrettyWriter<rapidjson::FileWriteStream>& w, const std::string& s)
{
  w.String(s.c_str());
}

void Serializer::Serialize(rapidjson::PrettyWriter<rapidjson::FileWriteStream>& w, const std::vector<int>& v)
{
    w.StartArray();
    for (int i : v)
    {
        w.Int(i);
    }
    w.EndArray();
}

void Serializer::Serialize(rapidjson::PrettyWriter<rapidjson::FileWriteStream>& w, const std::map<std::string, std::vector<int>>& m)
{
  w.StartArray();
  for (const auto& mapVal : m)
  {
    w.StartObject();
    w.Key("key");
    w.String(mapVal.first.c_str());
    w.Key("vector");
    w.StartArray();
    for (const auto& vecVal : mapVal.second)
    {
      w.Int(vecVal);
    }
    w.EndArray();
    w.EndObject();
  }
  w.EndArray();
}
