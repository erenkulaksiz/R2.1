#include <glad/glad.h>
#include <iostream>
#include <R2/Shader/VBO.h>

R2::VBO::VBO(GLfloat *p_vertices, GLsizeiptr size)
{
  glGenBuffers(1, &m_ID);
  glBindBuffer(GL_ARRAY_BUFFER, m_ID);
  glBufferData(GL_ARRAY_BUFFER, size, p_vertices, GL_STATIC_DRAW);
  std::cout << "VBO created with ID: " << m_ID << std::endl;
}

void R2::VBO::bind()
{
  glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

void R2::VBO::unbind()
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void R2::VBO::cleanup()
{
  std::cout << "VBO::cleanup()" << m_ID << std::endl;
  glDeleteBuffers(1, &m_ID);
}