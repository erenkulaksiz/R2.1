#pragma once
#ifndef RENDERER_H
#define RENDERER_H
#include <vector>
#include <R2/Mesh.h>
#include <R2/Application.h>
#include <R2/Camera.h>

namespace R2
{
  class Renderer
  {
  public:
    Renderer(Application *p_application);
    void render(Camera *p_camera, Scene *p_scene);
    void render(Camera *p_camera, Scene *p_scene, Shader *p_shader);
    void renderShadowMap(Shader *p_shader, Scene *p_scene, glm::mat4 lightSpaceMatrix);
    void renderShadowMap(Shader *p_shader, Scene *p_scene, std::vector<glm::mat4> lightSpaceMatrix, glm::vec3 lightPos, float farPlane);

  private:
    Application *m_papplication;
  };
}

#endif