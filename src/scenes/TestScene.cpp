#define GLFW_INCLUDE_NONE
#include <rapidxml/rapidxml.hpp>
#include <R2/scenes/TestScene.h>
#include <R2/Utils.h>
#include <R2/Mesh.h>
#include <R2/Camera.h>
#include <R2/Light.h>
#include <R2/Billboard.h>

R2::TestScene::TestScene(Application *m_papplication) : Scene(m_papplication)
{
  std::cout << "TestScene::TestScene()" << std::endl;
  this->m_papplication = m_papplication;
  setName("TestScene");
}

void R2::TestScene::setup()
{
  std::cout << "TestScene::setup()" << std::endl;
  m_isStartedSetup = true;

  addLineMeshes();
  addGridMesh();
  loadMeshes();

  R2::Camera *camera = new R2::Camera(m_papplication);
  camera->setActive(true);
  camera->setName("MainCamera");
  setCamera(camera);
  addMesh(camera);

  /*
  R2::Camera *camera2 = new R2::Camera(m_papplication);
  camera2->setPosition(glm::vec3(2.0f));
  camera2->setName("SecondCamera");
  addMesh(camera2);*/

  Billboard *p_billboardMesh = new Billboard(m_papplication, m_papplication->getUtils()->getFilePath("/resources/default.png"));
  p_billboardMesh->setPosition(glm::vec3(2.0f));
  p_billboardMesh->setName("Billboard");
  p_billboardMesh->setup();
  addMesh(p_billboardMesh);

  Light *p_light = new Light(m_papplication);
  p_light->setPosition(glm::vec3(-2.0f));
  p_light->setIsPointLight(true);
  p_light->setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
  p_light->setName("PointLight");
  p_light->setup();
  addMesh(p_light);

  /*
    Light *p_directionalLight = new Light(m_papplication);
    p_directionalLight->setIsDirectionalLight(true);
    p_directionalLight->setDirection(glm::vec3(-1.0f, -1.0f, 0.0f));
    p_directionalLight->setPosition(glm::vec3(0.0f, 20.0f, 0.0f));
    p_directionalLight->setAmbient(glm::vec3(0.0f, 0.0f, 0.0f));
    p_directionalLight->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    p_directionalLight->setName("DirectionalLight");
    p_directionalLight->setup();
    addMesh(p_directionalLight);*/

  m_isSetup = true;
  m_isStartedSetup = false;
}