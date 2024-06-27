#include <glad/glad.h>
#include <iostream>
#include <R2/Shader/EBO.h>

R2::EBO::EBO(GLuint *p_indices, GLsizeiptr size)
{
  glGenBuffers(1, &m_ID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, p_indices, GL_STATIC_DRAW);
  std::cout << "EBO created with ID: " << m_ID << std::endl;
}

void R2::EBO::bind()
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

void R2::EBO::unbind()
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void R2::EBO::cleanup()
{
  std::cout << "EBO::cleanup()" << m_ID << std::endl;
  glDeleteBuffers(1, &m_ID);
}