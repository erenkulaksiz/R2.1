#pragma once
#ifndef MODEL_H
#define MODEL_H
#include <string>
#include <vector>
#include <R2/Mesh.h>
#include <R2/Application.h>
#include <R2/shader/Shader.h>

namespace R2
{
  class Model
  {
  public:
    Model(Application *p_application);
    std::vector<Mesh *> loadFromFile(std::string fileName, Shader *p_shader);

  private:
    Application *m_papplication;
  };
}

#endif