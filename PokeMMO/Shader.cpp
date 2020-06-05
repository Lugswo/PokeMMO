#include "Shader.h"

#include <glad/glad.h>
#include <fstream>
#include <sstream>

#include <gtc/type_ptr.hpp>

#include "Logger.h"

unsigned CompileShader(std::fstream &vert, const std::string& filepath, bool b)
{
  unsigned int vertexShader;
  if (b)
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
  else
    vertexShader = glCreateShader(GL_FRAGMENT_SHADER);
  std::string vstring;
  std::stringstream vertexShaderContents; //contents of the shader

  vertexShaderContents << vert.rdbuf(); //read in file to string stream
  vstring = vertexShaderContents.str();

  const char* vertexSource = vstring.c_str();

  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);

  int  success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::string msg = filepath;
    msg += " failed to compile!  ";
    msg += infoLog;
    L::Log(TL::FATAL, msg);

    throw std::runtime_error("Shader failed to compile");
  }

  L::Log(TRACE_LEVEL::VERBOSE, filepath + " vertex shader compiled successfully.");

  return vertexShader;
}

Shader::Shader(const std::string& vs, const std::string& fs, const std::string& name)
{
  std::fstream vert(vs);
  std::fstream frag(fs);

  unsigned vertex = CompileShader(vert, vs, true);
  unsigned fragment = CompileShader(frag, fs, false);

  id = glCreateProgram();
  glAttachShader(id, vertex);
  glAttachShader(id, fragment);
  glLinkProgram(id);

  int success;
  char infoLog[512];
  glGetProgramiv(id, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(id, 512, NULL, infoLog);

    std::string msg = "Linking or creating the shader program has failed.  ";
    msg += infoLog;
    L::Log(TRACE_LEVEL::ERR, msg);
  }

  L::Log(TRACE_LEVEL::INFO, name + " shader successfully created.");

  // delete the created shaders
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

Shader::Shader(const std::string& n, const std::string& name)
{

}

Shader::~Shader()
{
  glDeleteProgram(id);
}


void Shader::Use() const
{
  glUseProgram(id);
}

void Shader::Uniform(const glm::mat4& mat, const std::string& name) const
{
  glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::Uniform(const int i, const std::string& name) const
{
  glUniform1i(glGetUniformLocation(id, name.c_str()), i);
}

void Shader::Uniform(const unsigned u, const std::string& name) const
{
  glUniform1ui(glGetUniformLocation(id, name.c_str()), u);
}

void Shader::Uniform(const float f, const std::string& name) const
{
  glUniform1f(glGetUniformLocation(id, name.c_str()), f);
}

void Shader::Uniform(const glm::vec3 &v, const std::string& name) const
{
  glUniform3f(glGetUniformLocation(id, name.c_str()), v.x, v.y, v.z);
}
