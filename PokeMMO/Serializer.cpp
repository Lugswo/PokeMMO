#include "Serializer.h"

#include <filereadstream.h>
#include <filewritestream.h>
#include <document.h>
#include <error/en.h>

#include "GameObject.h"
#include "Component.h"
#include "Logger.h"

#pragma warning(disable : 4996)

#define cSer comp[p.get_name().to_string().c_str()]

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
          v[count] = f.GetFloat();
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
