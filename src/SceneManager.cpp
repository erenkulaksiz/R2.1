#define GLFW_INCLUDE_NONE
#include <iostream>
#include <thread>
#include <R2/SceneManager.h>
#include <R2/Scene.h>

R2::SceneManager::SceneManager(Application *p_application)
{
  std::cout << "SceneManager::SceneManager()" << std::endl;
  this->m_papplication = p_application;
}

void R2::SceneManager::addScene(Scene *p_scene)
{
  std::cout << "SceneManager::addScene()" << std::endl;
  m_pscenes.push_back(p_scene);
}

void R2::SceneManager::setCurrentScene(Scene *p_scene)
{
  std::cout << "SceneManager::setCurrentScene()" << std::endl;

  if (m_pscenes.size() == 0)
  {
    std::cout << "No scenes added to SceneManager" << std::endl;
    return;
  }

  if (m_pcurrentScene != nullptr && m_pcurrentScene->getName() == p_scene->getName())
  {
    std::cout << "SceneManager::setCurrentScene() m_pcurrentScene == p_scene->getName()" << std::endl;
    return;
  }

  m_pcurrentScene = p_scene;

  for (int i = 0; i < m_pscenes.size(); i++)
  {
    m_pscenes[i]->setIsActiveScene(false);
  }

  m_pcurrentScene->setIsActiveScene(true);

  if (!m_pcurrentScene->getIsSetup() && !m_pcurrentScene->getIsStartedSetup())
  {
    std::cout << "SceneManager::setCurrentScene() selected scene is not setup." << std::endl;
    // std::thread setupThread(&Scene::setup, m_pcurrentScene);
    // setupThread.detach();
    // fix threading.
    m_pcurrentScene->setup();
  }
}

void R2::SceneManager::setCurrentScene(int index)
{
  std::cout << "SceneManager::setCurrentScene()" << std::endl;

  if (m_pscenes.size() == 0)
  {
    std::cout << "No scenes added to SceneManager" << std::endl;
    return;
  }

  if (index < 0 || index >= m_pscenes.size())
  {
    std::cout << "SceneManager::setCurrentScene() index out of range" << std::endl;
    return;
  }

  if (m_pcurrentScene != nullptr)
  {
    if (m_pcurrentScene->getName() == m_pscenes[index]->getName())
    {
      std::cout << "Set scene is already the scene" << std::endl;
      return;
    }
  }

  m_pcurrentScene = m_pscenes[index];

  for (int i = 0; i < m_pscenes.size(); i++)
  {
    m_pscenes[i]->setIsActiveScene(false);
    m_pscenes[i]->setIsPlaying(false);
  }

  m_pcurrentScene->setIsActiveScene(true);

  if (!m_pcurrentScene->getIsSetup() && !m_pcurrentScene->getIsStartedSetup())
  {
    m_pcurrentScene->setup();
  }
}

R2::Scene *R2::SceneManager::getCurrentScene()
{
  return m_pcurrentScene;
}

std::vector<R2::Scene *> R2::SceneManager::getScenes()
{
  return m_pscenes;
}

void R2::SceneManager::cleanup()
{
  std::cout << "SceneManager::cleanup()" << std::endl;
  for (int i = 0; i < m_pscenes.size(); i++)
  {
    m_pscenes[i]->cleanup();
  }
}

void R2::SceneManager::loadScenes()
{
  std::cout << "SceneManager::loadScenes()" << std::endl;
  std::vector<rapidxml::xml_node<> *> sceneNodes = m_papplication->getConfig()->getSceneNodes();

  for (int i = 0; i < sceneNodes.size(); i++)
  {
    std::string sceneName = sceneNodes[i]->first_attribute("name")->value();

    Scene *p_scene = new Scene(m_papplication);
    p_scene->setName(sceneName);
    addScene(p_scene);

    if (sceneNodes[i]->first_attribute("isDefault") != nullptr)
    {
      p_scene->setIsActiveScene(true);
      setCurrentScene(p_scene);
    }
  }

  std::cout << "SceneManager::loadScenes() " << m_pscenes.size() << " scenes loaded" << std::endl;
}