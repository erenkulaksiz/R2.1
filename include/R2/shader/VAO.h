#pragma once
#ifndef VAO_H
#define VAO_H
#include "VBO.h"

namespace R2
{
  class VAO
  {
  public:
    VAO();
    ~VAO();
    GLuint m_ID;
    void cleanup();
    void linkAttrib(VBO &VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void *p_offset);
    void linkVBO(VBO &VBO, GLuint layout);
    void bind();
    void unbind();
  };
}

#endif