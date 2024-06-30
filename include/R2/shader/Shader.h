#pragma once
#ifndef SHADER_H
#define SHADER_H
#include <glm/glm.hpp>
#include <string>
#include <R2/Application.h>

typedef unsigned int GLuint;

namespace R2
{
  class Application;

  class Shader
  {
  public:
    Shader(std::string vertexPath, std::string fragmentPath);
    Shader(std::string vertexPath, std::string fragmentPath, std::string geometryPath);
    ~Shader();
    void setup(Application *p_application);
    void setVec3(std::string name, glm::vec3 value);
    void setVec4(std::string name, glm::vec4 value);
    void setMat4(std::string name, glm::mat4 value);
    void setBool(std::string name, bool value);
    void setFloat(std::string name, float value);
    void setInt(std::string name, int value);
    void activate();
    void deactivate();
    GLuint getId();
    void cleanup();

  protected:
    std::string m_vertexPath;
    std::string m_fragmentPath;
    std::string m_geometryPath = "";
    GLuint m_ID;
  };
};

#endif