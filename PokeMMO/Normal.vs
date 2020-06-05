#version 460 core

layout (location = 0) in vec3 Pos;
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;
uniform mat4 transform;
uniform mat4 view;
uniform mat4 persp;

void main()
{
  gl_Position = persp * view * transform * vec4(Pos, 1.0);
  texCoord = aTexCoord;
}