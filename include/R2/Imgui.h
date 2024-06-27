#pragma once
#ifndef IMGUI_H
#define IMGUI_H
#include <GLFW/glfw3.h>
#include <vector>
#include <R2/Application.h>
#include <R2/Mesh.h>

namespace R2
{
  class Application;
  class Mesh;
  class Camera;
  class Light;
  class Scene;

  class Imgui
  {
  public:
    Imgui(Application *m_papplication);
    void cleanup();
    void render();
    void setMouseInteractions(bool value);
    Mesh *getSelectedMesh();
    void resizeFramebuffer();

  private:
    Application *m_papplication;
    Mesh *m_selectedMesh = nullptr;
    unsigned int m_framebuffer;
    unsigned int m_textureColorbuffer;
    unsigned int m_rbo;
    void newFrame();
    void drawGui();
    void setupFramebuffer();
    void renderScene(Scene *p_scene);
    void renderMeshTree(const std::vector<R2::Mesh *> &meshes);
    void renderMeshInspector(Mesh *p_mesh);
    void renderCameraSettings(Camera *p_camera);
    void renderLightSettings(Light *p_light);
    void renderMeshGeneral(Mesh *p_mesh);
    void renderMeshTransform(Mesh *p_mesh);
    void renderScenes(std::vector<Scene *> scenes);
    void renderMeshTextures(Mesh *p_mesh);
  };
}

#endif
