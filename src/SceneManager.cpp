#define GLFW_INCLUDE_NONE
#include <iostream>
#include <R2/SceneManager.h>

R2::SceneManager::SceneManager(Application *p_application)
{
  std::cout << "SceneManager::SceneManager()" << std::endl;
  this->p_mapplication = p_application;
}

void R2::SceneManager::addScene(Scene *p_scene)
{
  std::cout << "SceneManager::addScene()" << std::endl;
  p_mscenes.push_back(p_scene);
}

void R2::SceneManager::setCurrentScene(unsigned int index)
{
  std::cout << "SceneManager::setCurrentScene()" << std::endl;
  if (p_mcurrentScene == index)
  {
    std::cout << "SceneManager::setCurrentScene() p_mcurrentScene == index" << std::endl;
    return;
  }

  if (index < p_mscenes.size())
  {
    std::cout << "SceneManager::setCurrentScene() index < p_mscenes.size()" << std::endl;
    p_mcurrentScene = index;

    if (p_mscenes.size() == 0)
    {
      std::cout << "No scenes added to SceneManager" << std::endl;
      return;
    }

    if (p_mscenes[index]->getIsActiveScene())
    {
      return;
    }

    for (int i = 0; i < p_mscenes.size(); i++)
    {
      if (i != index)
      {
        // p_mscenes[i]->cleanup(); #TODO: Cleaned up scenes are not being re-setup
        p_mscenes[i]->setIsActiveScene(false);
      }
    }

    p_mscenes[index]->setIsActiveScene(true);

    if (!p_mscenes[index]->getIsSetup() && !p_mscenes[index]->getIsStartedSetup())
    {
      p_mscenes[index]->setup();
    }
  }
  else
  {
    std::cout << "SceneManager::setCurrentScene() index >= p_mscenes.size()" << std::endl;
  }
}

R2::Scene *R2::SceneManager::getCurrentScene()
{
  return p_mscenes[p_mcurrentScene];
}

std::vector<R2::Scene *> R2::SceneManager::getScenes()
{
  return p_mscenes;
}

void R2::SceneManager::cleanup()
{
  std::cout << "SceneManager::cleanup()" << std::endl;
  for (int i = 0; i < p_mscenes.size(); i++)
  {
    p_mscenes[i]->cleanup();
  }
}