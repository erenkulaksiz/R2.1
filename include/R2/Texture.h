#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H
#include <stb/stb_image.h>
#include <string>
#include <R2/shader/Shader.h>

typedef unsigned int GLuint;
typedef unsigned int GLenum;

namespace R2
{
  class Shader;

  class Texture
  {
  public:
    Texture(std::string imagePath, GLenum texType, GLenum slot, GLenum pixelType);
    Texture(GLenum texType, GLenum slot, GLenum pixelType);
    void texUnit(Shader *p_shader, const char *p_uniform, GLuint unit);
    void bind();
    void setup();
    void unbind();
    void cleanup();
    void setIsDiffuse(bool value);
    void setIsSpecular(bool value);
    void setIsNormal(bool value);
    bool getIsDiffuse();
    bool getIsSpecular();
    bool getIsNormal();
    GLuint getId();
    std::string getImagePath();
    void setShininess(float value);
    float getShininess();

  private:
    GLuint m_ID;
    GLenum m_slot;
    GLenum m_texType;
    GLenum m_pixelType;
    std::string m_imagePath;
    bool m_isDiffuse = false;
    bool m_isSpecular = false;
    bool m_isNormal = false;
    float m_shininess;
  };
}

#endif