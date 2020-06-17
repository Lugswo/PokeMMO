#pragma once

#include "GameObject.h"

#include <prettywriter.h>
#include <glm.hpp>
#include <map>
#include <rttr/variant.h>
#include <fstream>
#include <filewritestream.h>
#include <document.h>

class Serializer
{
public:
  static void Serialize(const GameObject* obj);
  static GameObject* Parse(const std::string& file);

private:
  static void Serialize(rapidjson::PrettyWriter<rapidjson::FileWriteStream>& w, rttr::variant& r);
  static void Parse(rttr::variant& v, const rapidjson::Value& comp, rttr::variant& cVar, const rttr::property& p);

  template <typename T>
  static void Serialize(rapidjson::PrettyWriter<rapidjson::FileWriteStream>& w, rttr::variant& r)
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
  }

  static void Serialize(rapidjson::PrettyWriter<rapidjson::FileWriteStream>& w, const int i);
  static void Serialize(rapidjson::PrettyWriter<rapidjson::FileWriteStream>& w, const float f);
  static void Serialize(rapidjson::PrettyWriter<rapidjson::FileWriteStream>& w, const bool b);
  static void Serialize(rapidjson::PrettyWriter<rapidjson::FileWriteStream>& w, const double d);
  static void Serialize(rapidjson::PrettyWriter<rapidjson::FileWriteStream>& w, const unsigned u);
  static void Serialize(rapidjson::PrettyWriter<rapidjson::FileWriteStream>& w, const glm::vec3& v);
  static void Serialize(rapidjson::PrettyWriter<rapidjson::FileWriteStream>& w, const glm::vec2& i);
  static void Serialize(rapidjson::PrettyWriter<rapidjson::FileWriteStream>& w, const std::string& s);
  static void Serialize(rapidjson::PrettyWriter<rapidjson::FileWriteStream>& w, const std::vector<int>& v);
  static void Serialize(rapidjson::PrettyWriter<rapidjson::FileWriteStream>& w, const std::map<std::string,std::vector<int>>& m);


  //template <typename T>
  //static void Serialize(rapidjson::PrettyWriter<rapidjson::FileWriteStream>& w, const std::vector<T>& v)
  //{
  //
  //}

  //template <typename T1, typename T2>
  //static void Serialize(rapidjson::PrettyWriter<rapidjson::FileWriteStream>& w, const std::map<T1, T2>& m)
  //{
  //
  //}
};