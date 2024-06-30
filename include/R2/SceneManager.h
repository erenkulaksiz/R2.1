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
    void setCurrentScene(Scene *p_scene);
    void setCurrentScene(int index);
    void loadScenes();
    Scene *getCurrentScene();
    std::vector<Scene *> getScenes();
    void cleanup();

  private:
    Application *m_papplication;
    std::vector<Scene *> m_pscenes;
    Scene* m_pcurrentScene = nullptr;
  };
}

#endif