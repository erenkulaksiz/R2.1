#pragma once
#ifndef RAYCAST_H
#define RAYCAST_H
#include <R2/Application.h>
#include <R2/Mesh.h>

namespace R2
{
  class Raycast
  {
  public:
    Raycast(Application *p_application);
    void setOrigin(const glm::vec3 &origin);
    void setDirection(const glm::vec3 &direction);
    Mesh* castRay();

  private:
    Application *m_papplication;
    glm::vec3 m_origin;
    glm::vec3 m_direction;
  };
}

#endif