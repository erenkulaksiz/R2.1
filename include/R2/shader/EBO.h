#pragma once
#ifndef EBO_H
#define EBO_H

typedef unsigned int GLuint;
typedef signed long long int GLsizeiptr;

namespace R2
{
  class EBO
  {
  public:
    EBO(GLuint *p_indices, GLsizeiptr size);
    void cleanup();
    GLuint m_ID;
    void bind();
    void unbind();
  };
}

#endif