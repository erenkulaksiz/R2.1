#pragma once
#ifndef SCENE_H
#define SCENE_H
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <R2/Config.h>
#include <R2/Application.h>
#include <R2/Mesh.h>

namespace R2
{
  class Application;
  class Config;
  class Mesh;
  class Camera;
  class Light;

  class Scene
  {
  public:
    Scene(Application *p_application);
    bool getIsSetup();
    bool getIsStartedSetup();
    bool getIsActiveScene();
    void setIsActiveScene(bool m_isActiveScene);
    std::string getName();
    void setName(std::string m_name);
    virtual void setup() = 0;
    void cleanup();
    void loop();
    std::vector<Mesh *> getMeshes();
    void addLineMeshes();
    void addMesh(Mesh *mesh);
    void setCamera(Camera *p_camera);
    Camera *getCamera();
    std::vector<Camera *> getCameras();
    void addGridMesh();
    Mesh *getMesh(std::string name);
    Mesh *getMesh(int index);
    Mesh *lineMesh(glm::vec3 start, glm::vec3 end, glm::vec4 color);
    void loadMeshes();
    void reload();
    void setClearColor(glm::vec4 clearColor);
    glm::vec4 getClearColor();
    std::vector<Light *> getPointLights();
    std::vector<Light *> getDirectionalLights();
    void setIsLightsActive(bool isLightsActive);
    bool getIsLightsActive();
    Application *getApplication();

  protected:
    Application *m_papplication;
    Camera *m_pcamera;
    std::vector<Mesh *> m_meshes;
    std::vector<Light *> m_pointLights;
    std::vector<Light *> m_directionalLights;
    glm::vec4 m_clearColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    bool m_isSetup = false;
    bool m_isStartedSetup = false;
    bool m_isActiveScene = false;
    bool m_isLightsActive = true;
    std::string m_name;
  };
}

#endif