#pragma once
#ifndef VBO_H
#define VBO_H

typedef unsigned int GLuint;
typedef float GLfloat;
typedef signed long long int GLsizeiptr;
typedef unsigned int GLenum;

namespace R2
{
  class VBO
  {
  public:
    VBO(GLfloat *p_vertices, GLsizeiptr size);
    void cleanup();
    GLuint m_ID;
    void bind();
    void unbind();
  };
}

#endif