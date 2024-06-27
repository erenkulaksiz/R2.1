#define GLFW_INCLUDE_NONE
#include <rapidxml/rapidxml.hpp>
#include <R2/scenes/TestScene2.h>
#include <R2/Utils.h>
#include <R2/Mesh.h>
#include <R2/Camera.h>
#include <R2/Billboard.h>

R2::TestScene2::TestScene2(Application *m_papplication) : Scene(m_papplication)
{
  std::cout << "TestScene2::TestScene2()" << std::endl;
  this->m_papplication = m_papplication;
  setName("TestScene2");
}

void R2::TestScene2::setup()
{
  std::cout << "TestScene2::setup()" << std::endl;
  m_isStartedSetup = true;

  addLineMeshes();
  addGridMesh();
  loadMeshes();

  R2::Camera *camera = new R2::Camera(m_papplication);
  camera->setActive(true);
  camera->setName("MainCamera");
  setCamera(camera);
  addMesh(camera);

  Billboard *p_billboardMesh = new Billboard(m_papplication, m_papplication->getUtils()->getFilePath("/resources/default.png"));
  p_billboardMesh->setName("Billboard");
  p_billboardMesh->setup();
  addMesh(p_billboardMesh);

  m_isSetup = true;
  m_isStartedSetup = false;
}