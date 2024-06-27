#include <iostream>
#include <glad/glad.h>
#include <R2/Scene.h>
#include <R2/Mesh.h>
#include <R2/Camera.h>
#include <R2/Renderer.h>
#include <R2/Model.h>
#include <R2/Light.h>
#include <R2/MeshGroup.h>
#include <R2/Application.h>

R2::Scene::Scene(Application *m_papplication)
{
  std::cout << "Scene::Scene()" << std::endl;
  this->m_papplication = m_papplication;
}

bool R2::Scene::getIsSetup()
{
  return m_isSetup;
}

bool R2::Scene::getIsStartedSetup()
{
  return m_isStartedSetup;
}

void R2::Scene::setIsActiveScene(bool isActiveScene)
{
  this->m_isActiveScene = isActiveScene;
}

bool R2::Scene::getIsActiveScene()
{
  return m_isActiveScene;
}

std::string R2::Scene::getName()
{
  return m_name;
}

void R2::Scene::setName(std::string name)
{
  this->m_name = name;
}

std::vector<R2::Mesh *> R2::Scene::getMeshes()
{
  return m_meshes;
}

void R2::Scene::addLineMeshes()
{
  Mesh *xLineMesh = lineMesh(glm::vec3(0.0f), glm::vec3(5.0f, 0.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
  addMesh(xLineMesh);

  Mesh *yLineMesh = lineMesh(glm::vec3(0.0f), glm::vec3(0.0f, 5.0f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
  addMesh(yLineMesh);

  Mesh *zLineMesh = lineMesh(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 5.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
  addMesh(zLineMesh);
}

void R2::Scene::addMesh(Mesh *mesh)
{
  if (mesh->getIsLight())
  {
    Light *p_light = (Light *)mesh;
    if (p_light->getIsPointLight())
    {
      m_pointLights.push_back(p_light);
    }
    else if (p_light->getIsDirectionalLight())
    {
      m_directionalLights.push_back(p_light);
    }
  }

  m_meshes.push_back(mesh);
}

void R2::Scene::setCamera(Camera *p_camera)
{
  m_pcamera = p_camera;
}

R2::Camera *R2::Scene::getCamera()
{
  return m_pcamera;
}

void R2::Scene::loop()
{
  if (!m_isActiveScene)
    return;

  if (m_isSetup)
  {
    for (Light *p_directionalLights : m_directionalLights)
    {
      p_directionalLights->updateShadowMap(m_papplication->getDirectionalDepthShader(), m_pcamera);
    }
    for (Light *p_pointLight : m_pointLights)
    {
      p_pointLight->updateShadowMap(m_papplication->getPointDepthShader(), m_pcamera);
    }

    glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);

    m_papplication->getRenderer()->render(m_pcamera, this);
  }
  else
  {
    if (!m_isStartedSetup)
    {
      setup();
    }
  }
}

R2::Mesh *R2::Scene::lineMesh(glm::vec3 start, glm::vec3 end, glm::vec4 color)
{
  Shader *p_shader = new Shader(m_papplication->getUtils()->getFilePath("/shaders/line/line.vert"), m_papplication->getUtils()->getFilePath("/shaders/line/line.frag"));
  p_shader->setup(m_papplication);

  float lineVertices[6] = {
      start.x, start.y, start.z,
      end.x, end.y, end.z};

  unsigned int lineIndices[2] = {
      0, 1};

  Mesh *p_lineMesh = new Mesh(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), p_shader);
  p_lineMesh->setName("LineMesh");
  p_lineMesh->setIsLine(true);
  p_lineMesh->setVertices(lineVertices, sizeof(lineVertices));
  p_lineMesh->setIndices(lineIndices, sizeof(lineIndices));
  p_lineMesh->setColor(color);
  p_lineMesh->setIsVisibleOnMenu(false);
  p_lineMesh->setup();

  return p_lineMesh;
}

void R2::Scene::cleanup()
{
  std::cout << "Scene::cleanup()" << std::endl;
  for (Mesh *mesh : m_meshes)
  {
    mesh->cleanup();
  }
  m_isSetup = false;
  m_isStartedSetup = false;
}

std::vector<R2::Camera *> R2::Scene::getCameras()
{
  std::vector<Camera *> cameras;
  for (Mesh *mesh : m_meshes)
  {
    if (mesh->getIsCamera())
    {
      cameras.push_back((Camera *)mesh);
    }
  }
  return cameras;
}

void R2::Scene::addGridMesh()
{
  Shader *p_shader = new Shader(m_papplication->getUtils()->getFilePath("/shaders/line/line.vert"), m_papplication->getUtils()->getFilePath("/shaders/line/line.frag"));
  p_shader->setup(m_papplication);

  float gridSize = 300.0f;
  int divisions = 100;

  float halfSize = gridSize / 2.0f;
  float step = gridSize / static_cast<float>(divisions);

  std::vector<float> vertices;
  std::vector<unsigned int> indices;

  for (int i = -divisions / 2; i <= divisions / 2; ++i)
  {
    vertices.push_back(static_cast<float>(i) * step);
    vertices.push_back(0.0f);
    vertices.push_back(-halfSize);

    vertices.push_back(static_cast<float>(i) * step);
    vertices.push_back(0.0f);
    vertices.push_back(halfSize);
  }

  for (int i = -divisions / 2; i <= divisions / 2; ++i)
  {
    vertices.push_back(-halfSize);
    vertices.push_back(0.0f);
    vertices.push_back(static_cast<float>(i) * step);

    vertices.push_back(halfSize);
    vertices.push_back(0.0f);
    vertices.push_back(static_cast<float>(i) * step);
  }

  for (size_t i = 0; i < vertices.size() / 3; ++i)
  {
    indices.push_back(static_cast<unsigned int>(i));
  }

  Mesh *p_gridMesh = new Mesh(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), p_shader);
  p_gridMesh->setName("GridMesh");
  p_gridMesh->setIsLine(true);
  p_gridMesh->setVertices(vertices.data(), vertices.size() * sizeof(float));
  p_gridMesh->setIndices(indices.data(), indices.size() * sizeof(unsigned int));
  p_gridMesh->setColor(glm::vec4(0.5f, 0.5f, 0.5f, 0.3f));
  p_gridMesh->setIsVisibleOnMenu(false);
  p_gridMesh->setup();

  addMesh(p_gridMesh);
}

R2::Mesh *R2::Scene::getMesh(std::string name)
{
  for (Mesh *mesh : m_meshes)
  {
    if (mesh->getName() == name)
    {
      return mesh;
    }
  }
  return nullptr;
}

R2::Mesh *R2::Scene::getMesh(int index)
{
  return m_meshes[index];
}

void R2::Scene::loadMeshes()
{
  std::cout << "Scene::loadMeshes()" << std::endl;
  std::vector<rapidxml::xml_node<> *> objectNodes = m_papplication->getConfig()->getObjectNodes(m_name);

  for (rapidxml::xml_node<> *objectNode : objectNodes)
  {
    std::cout << "Object name: " << objectNode->first_attribute("name")->value() << std::endl;

    if (objectNode->first_attribute("type")->value() == std::string("mesh"))
    {
      std::string path = objectNode->first_attribute("file")->value();
      Model *p_model = new Model(m_papplication);
      Shader *p_objectShader = new Shader(m_papplication->getUtils()->getFilePath("/shaders/default/default.vert"), m_papplication->getUtils()->getFilePath("/shaders/default/default.frag"));
      p_objectShader->setup(m_papplication);
      std::vector<R2::Mesh *> modelMeshes = p_model->loadFromFile(m_papplication->getUtils()->getFilePath(path), p_objectShader);

      if (modelMeshes.size() == 1)
      {
        modelMeshes[0]->setName(objectNode->first_attribute("name")->value());
        float posX = std::atoi(objectNode->first_attribute("posX")->value());
        float posY = std::atoi(objectNode->first_attribute("posY")->value());
        float posZ = std::atoi(objectNode->first_attribute("posZ")->value());
        modelMeshes[0]->setPosition(glm::vec3(posX, posY, posZ));
        addMesh(modelMeshes[0]);
      }
      else
      {
        MeshGroup *p_meshGroup = new MeshGroup();
        p_meshGroup->setName(objectNode->first_attribute("name")->value());
        for (Mesh *mesh : modelMeshes)
        {
          p_meshGroup->addMesh(mesh);
        }
        float posX = std::atoi(objectNode->first_attribute("posX")->value());
        float posY = std::atoi(objectNode->first_attribute("posY")->value());
        float posZ = std::atoi(objectNode->first_attribute("posZ")->value());
        p_meshGroup->setPosition(glm::vec3(posX, posY, posZ));
        addMesh(p_meshGroup);
      }
    }
  }
}

void R2::Scene::reload()
{
  cleanup();
  m_meshes.clear();
  setup();
}

void R2::Scene::setClearColor(glm::vec4 clearColor)
{
  m_clearColor = clearColor;
}

glm::vec4 R2::Scene::getClearColor()
{
  return m_clearColor;
}

std::vector<R2::Light *> R2::Scene::getPointLights()
{
  return m_pointLights;
}

std::vector<R2::Light *> R2::Scene::getDirectionalLights()
{
  return m_directionalLights;
}

void R2::Scene::setIsLightsActive(bool isLightsActive)
{
  m_isLightsActive = isLightsActive;
}

bool R2::Scene::getIsLightsActive()
{
  return m_isLightsActive;
}

R2::Application *R2::Scene::getApplication()
{
  return m_papplication;
}