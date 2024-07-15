#include <thread>
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <R2/Texture.h>
#include <R2/Application.h>

R2::Texture::Texture(Application* p_application, std::string imagePath, GLenum texType, GLenum slot, GLenum pixelType)
{
  std::cout << "Texture::Texture() " << imagePath << std::endl;
  this->m_texType = texType;
  this->m_imagePath = imagePath;
  this->m_slot = slot;
  this->m_pixelType = pixelType;
  this->m_papplication = p_application;
}

R2::Texture::Texture(Application* p_application, GLenum texType, GLenum slot, GLenum pixelType)
{
  std::cout << "Texture::Texture() " << m_imagePath << std::endl;
  this->m_texType = texType;
  this->m_imagePath = m_imagePath;
  this->m_slot = slot;
  this->m_pixelType = pixelType;
  this->m_papplication = p_application;
}

R2::Texture::~Texture()
{
  std::cout << "Texture::~Texture()" << std::endl;
  cleanup();
}

void R2::Texture::load()
{
  std::cout << "Texture::load()" << std::endl;

  m_bytes = m_papplication->getAssetManager()->loadTexture(m_imagePath.c_str(), m_width, m_height, m_numColCh);
  m_isLoaded = true;
}

void R2::Texture::setup()
{
  std::cout << "Texture::setup()" << std::endl;

  if(!m_isLoaded)
  {
    std::cerr << "Texture::setup() texture not loaded" << std::endl;
    return;
  }

  glGenTextures(1, &m_ID);
  glActiveTexture(m_slot);
  glBindTexture(m_texType, m_ID);

  glTexParameteri(m_texType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
  glTexParameteri(m_texType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameteri(m_texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(m_texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

  GLenum format = GL_RED;
  if (m_numColCh == 1)
    format = GL_RED;
  else if (m_numColCh == 3)
    format = GL_RGB;
  else if (m_numColCh == 4)
    format = GL_RGBA;

  // float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
  // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);
  glTexImage2D(m_texType, 0, format, m_width, m_height, 0, format, m_pixelType, m_bytes);
  glGenerateMipmap(m_texType);
  m_papplication->getAssetManager()->textureFreeBytes(m_bytes);
  glBindTexture(m_texType, 0);
}

void R2::Texture::texUnit(Shader *p_shader, const char *p_uniform, GLuint unit)
{
  p_shader->activate();
  glUniform1i(glGetUniformLocation(p_shader->getId(), p_uniform), unit);
}

void R2::Texture::bind()
{
  glActiveTexture(m_slot);
  glBindTexture(m_texType, m_ID);
}

void R2::Texture::unbind()
{
  glBindTexture(m_texType, 0);
}

void R2::Texture::cleanup()
{
  std::cout << "Texture::cleanup()" << m_ID << std::endl;
  glDeleteTextures(1, &m_ID);
}

void R2::Texture::setIsDiffuse(bool value)
{
  m_isDiffuse = value;
}

void R2::Texture::setIsSpecular(bool value)
{
  m_isSpecular = value;
}

void R2::Texture::setIsNormal(bool value)
{
  m_isNormal = value;
}

bool R2::Texture::getIsDiffuse()
{
  return m_isDiffuse;
}

bool R2::Texture::getIsSpecular()
{
  return m_isSpecular;
}

bool R2::Texture::getIsNormal()
{
  return m_isNormal;
}

GLuint R2::Texture::getId()
{
  return m_ID;
}

std::string R2::Texture::getImagePath()
{
  return m_imagePath;
}

void R2::Texture::setShininess(float value)
{
  m_shininess = value;
}

float R2::Texture::getShininess()
{
  return m_shininess;
}

void R2::Texture::setIsLoaded(bool value)
{
  m_isLoaded = value;
}

bool R2::Texture::getIsLoaded()
{
  return m_isLoaded;
}