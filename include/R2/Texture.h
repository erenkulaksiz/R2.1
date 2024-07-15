#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H
#include <stb/stb_image.h>
#include <string>
#include <R2/shader/Shader.h>
#include <R2/AssetManager.h>

typedef unsigned int GLuint;
typedef unsigned int GLenum;

namespace R2
{
  class Shader;
  class Application;

  class Texture
  {
  public:
    Texture(Application* p_application, std::string imagePath, GLenum texType, GLenum slot, GLenum pixelType);
    Texture(Application* p_application, GLenum texType, GLenum slot, GLenum pixelType);
    ~Texture();
    void texUnit(Shader *p_shader, const char *p_uniform, GLuint unit);
    void bind();
    void load();
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
    void setIsLoaded(bool value);
    bool getIsLoaded();

  private:
    Application* m_papplication;
    GLuint m_ID;
    GLenum m_slot;
    GLenum m_texType;
    GLenum m_pixelType;
    std::string m_imagePath;
    bool m_isDiffuse = false;
    bool m_isSpecular = false;
    bool m_isNormal = false;
    float m_shininess;
    bool m_isLoaded = false;
    int m_width, m_height, m_nrChannels, m_numColCh;
    unsigned char* m_bytes;
  };
}

#endif