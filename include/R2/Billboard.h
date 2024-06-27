#pragma once
#ifndef BILLBOARD_H
#define BILLBOARD_H
#include <R2/Mesh.h>
#include <R2/Application.h>

namespace R2
{
  class Billboard : public Mesh
  {
  public:
    Billboard(Application *p_application, std::string imagePath);

  private:
    Application *m_papplication;
  };
}

#endif