#pragma once
#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include <vector>
#include <R2/Application.h>
#include <R2/Scene.h>

namespace R2
{
  class Scene;
  class Application;

  class SceneManager
  {
  public:
    SceneManager(Application *p_application);
    void addScene(Scene *p_scene);
    void setCurrentScene(unsigned int index);
    void setupCurrentScene();
    Scene *getCurrentScene();
    std::vector<Scene *> getScenes();
    void cleanup();

  private:
    Application *p_mapplication;
    std::vector<Scene *> p_mscenes;
    unsigned int p_mcurrentScene = -1;
  };
}

#endif