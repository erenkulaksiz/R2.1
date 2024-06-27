#include <iostream>
#include <glad/glad.h>
#include <R2/Shader/VAO.h>

R2::VAO::VAO()
{
  glGenVertexArrays(1, &m_ID);
  std::cout << "VAO created with ID: " << m_ID << std::endl;
}

void R2::VAO::linkAttrib(VBO &VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void *p_offset)
{
  VBO.bind();
  glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, p_offset);
  glEnableVertexAttribArray(layout);
  VBO.unbind();
}

void R2::VAO::linkVBO(VBO &VBO, GLuint layout)
{
  VBO.bind();
  glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
  glEnableVertexAttribArray(layout);
  VBO.unbind();
}

void R2::VAO::bind()
{
  glBindVertexArray(m_ID);
}

void R2::VAO::unbind()
{
  glBindVertexArray(0);
}

void R2::VAO::cleanup()
{
  std::cout << "VAO::cleanup()" << m_ID << std::endl;
  glDeleteVertexArrays(1, &m_ID);
}