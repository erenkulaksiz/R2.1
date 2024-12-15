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
  if (!m_isActiveScene) {
    std::cout << "Scene::loop() Scene is not active" << std::endl;
    return;
  }

  if (m_pcamera == nullptr && !m_isStartedSetup)
  {
    std::cout << "Scene::loop() No camera set" << std::endl;
    return;
  }

  if (m_isSetup)
  {
    if (m_papplication->getFrameCount() % 2 == 0)
    {
      for (Light* p_directionalLights : m_directionalLights)
      {
        p_directionalLights->updateDirectionalShadowMap(m_papplication->getRenderer()->getDirectionalDepthShader(), m_pcamera);
      }

      for (int i = 0; i < m_pointLights.size(); ++i)
      {
        float distance = glm::length(m_pcamera->getPosition() - m_pointLights[i]->getPosition());

        if (distance > (m_pointLights[i]->getFarPlane() / 2))
        {
          continue;
        }

        m_pointLights[i]->updatePointShadowMap(m_papplication->getRenderer()->getPointDepthShader(), m_pcamera, i);
      }
    }

    glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_papplication->getRenderer()->render(m_pcamera, this);
  }
  else
  {
    if (!m_isStartedSetup)
    {
      std::cout << "Scene::loop() Scene not setup" << std::endl;
      setup();
    }
    else {
      std::cout << "Scene::loop() Scene setup in progress" << std::endl;
    }
  }
}

void R2::Scene::cleanup()
{
  std::cout << "Scene::cleanup() " << m_name << std::endl;
  m_isSetup = false;
  m_isStartedSetup = false;
  m_pcamera = nullptr;
  for (Mesh *p_mesh : m_meshes)
  {
    delete p_mesh;
  }
  m_meshes.clear();
  m_directionalLights.clear();
  m_pointLights.clear();
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

void R2::Scene::setup()
{
  std::cout << "Scene::setup()" << std::endl;

  if (m_isStartedSetup)
  {
    std::cout << "Scene::setup() Already started setup" << std::endl;
    return;
  }

  m_isSetup = false;
  m_isStartedSetup = true;

  std::vector<rapidxml::xml_node<> *> objectNodes = m_papplication->getConfig()->getObjectNodes(m_name);

  std::cout << "Object nodes size: " << objectNodes.size() << std::endl;

  for (rapidxml::xml_node<> *objectNode : objectNodes)
  {
    std::cout << "Object name: " << objectNode->first_attribute("name")->value() << std::endl;

    if(objectNode->name() == std::string("camera"))
    {
      setupCameraObject(objectNode);
    } 
    else if (objectNode->name() == std::string("mesh"))
    {
      setupMeshObject(objectNode);
    }
    else if (objectNode->name() == std::string("light"))
    {
      setupLightObject(objectNode);
    } 
    else if(objectNode->name() == std::string("billboard"))
    {
      setupBillboardObject(objectNode);
    }
    else
    {
      std::cout << "Object type not recognized" << std::endl;
    }
  }

  m_isSetup = true;
  m_isStartedSetup = false;

  glfwMakeContextCurrent(m_papplication->getWindow());
}

void R2::Scene::reload()
{
  std::cout << "Scene::reload() " << m_name << std::endl;
  cleanup();
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

void R2::Scene::setIsPlaying(bool isPlaying)
{
  m_isPlaying = isPlaying;
}

bool R2::Scene::getIsPlaying()
{
  return m_isPlaying;
}

void R2::Scene::setupCameraObject(rapidxml::xml_node<> *objectNode)
{
  glm::vec3 pos = m_papplication->getUtils()->stringToVec3(objectNode->first_attribute("pos")->value());
  glm::vec3 rot = glm::vec3(0.0f, 0.0f, 0.0f);

  if (objectNode->first_attribute("rot") != nullptr)
  {
    rot = m_papplication->getUtils()->stringToVec3(objectNode->first_attribute("rot")->value());
  }

  glm::vec3 scl = glm::vec3(1.0f, 1.0f, 1.0f);

  if (objectNode->first_attribute("scl") != nullptr)
  {
    scl = m_papplication->getUtils()->stringToVec3(objectNode->first_attribute("scl")->value());
  }

  if (objectNode->first_attribute("pos") != nullptr)
  {
    pos = m_papplication->getUtils()->stringToVec3(objectNode->first_attribute("pos")->value());
  }
  
  Camera* p_camera = new Camera(m_papplication);
  p_camera->setName(objectNode->first_attribute("name")->value());
  p_camera->setPosition(pos);
  p_camera->setRotation(rot);
  p_camera->setScale(scl);
  
  if (objectNode->first_attribute("far") != nullptr)
  {
    p_camera->setFar(m_papplication->getUtils()->stringToFloat(objectNode->first_attribute("far")->value()));
  }

  if (objectNode->first_attribute("isMain") != nullptr)
  {
    setCamera(p_camera);
    p_camera->setIsActive(true);
  }

  addMesh(p_camera);
}

void R2::Scene::setupMeshObject(rapidxml::xml_node<> *objectNode)
{
  std::string path = objectNode->first_attribute("file")->value();
  Model *p_model = new Model(m_papplication);
  Shader *p_objectShader = nullptr;

  if (objectNode->first_attribute("shader") != nullptr)
  {
    std::string type = objectNode->first_attribute("shader")->value();

    std::cout << "Shader type: " << type << std::endl;

    if (std::string(type) == std::string("default"))
    {
      std::cout << "Default shader" << std::endl;
      p_objectShader = m_papplication->getRenderer()->getDefaultObjectShader();
    }
  }

  std::vector<R2::Mesh*> modelMeshes = p_model->loadFromFile(m_papplication->getUtils()->getFilePath(path), p_objectShader);

  glm::vec3 pos = m_papplication->getUtils()->stringToVec3(objectNode->first_attribute("pos")->value());
  glm::vec3 rot = glm::vec3(0.0f, 0.0f, 0.0f);

  if (objectNode->first_attribute("rot") != nullptr)
  {
    rot = m_papplication->getUtils()->stringToVec3(objectNode->first_attribute("rot")->value());
  }

  glm::vec3 scl = glm::vec3(1.0f, 1.0f, 1.0f);

  if (objectNode->first_attribute("scl") != nullptr)
  {
    scl = m_papplication->getUtils()->stringToVec3(objectNode->first_attribute("scl")->value());
  }

  if (modelMeshes.size() == 1)
  {
    bool hasPhysics = false;

    if (objectNode->first_attribute("hasPhysics") != nullptr)
    {
      hasPhysics = m_papplication->getUtils()->stringToBool(objectNode->first_attribute("hasPhysics")->value());
    }

    modelMeshes[0]->setName(objectNode->first_attribute("name")->value());
    modelMeshes[0]->setPosition(pos);
    modelMeshes[0]->setRotation(rot);
    modelMeshes[0]->setScale(scl);
    modelMeshes[0]->setHasPhysics(hasPhysics);
    addMesh(modelMeshes[0]);
  }
  else
  {
    MeshGroup* p_meshGroup = new MeshGroup();
    p_meshGroup->setName(objectNode->first_attribute("name")->value());
    for (Mesh* mesh : modelMeshes)
    {
      p_meshGroup->addMesh(mesh);
    }
    p_meshGroup->setPosition(pos);
    p_meshGroup->setRotation(rot);
    p_meshGroup->setScale(scl);
    addMesh(p_meshGroup);
  }
}

void R2::Scene::setupLightObject(rapidxml::xml_node<>* objectNode)
{
  glm::vec3 pos = m_papplication->getUtils()->stringToVec3(objectNode->first_attribute("pos")->value());
  glm::vec3 rot = glm::vec3(0.0f, 0.0f, 0.0f);

  if (objectNode->first_attribute("rot") != nullptr)
  {
    rot = m_papplication->getUtils()->stringToVec3(objectNode->first_attribute("rot")->value());
  }

  glm::vec3 scl = glm::vec3(1.0f, 1.0f, 1.0f);

  if (objectNode->first_attribute("scl") != nullptr)
  {
    scl = m_papplication->getUtils()->stringToVec3(objectNode->first_attribute("scl")->value());
  }

  Light* p_light = new Light(m_papplication);
  p_light->setName(objectNode->first_attribute("name")->value());

  if (objectNode->first_attribute("type") != nullptr)
  {
    std::string type = objectNode->first_attribute("type")->value();
    if (type == "point")
    {
      p_light->setIsPointLight(true);
    }
    else if (type == "directional")
    {
      p_light->setIsDirectionalLight(true);
    }
    else if (type == "spot")
    {
      p_light->setIsSpotLight(true);
    }
  }

  if (objectNode->first_attribute("color") != nullptr)
  {
    glm::vec4 color = m_papplication->getUtils()->stringToVec4(objectNode->first_attribute("color")->value());
    p_light->setColor(color);
  }

  if (objectNode->first_attribute("intensity") != nullptr)
  {
    float color = m_papplication->getUtils()->stringToFloat(objectNode->first_attribute("intensity")->value());
    p_light->setIntensity(color);
  }

  if (objectNode->first_attribute("dir") != nullptr)
  {
    glm::vec3 direction = m_papplication->getUtils()->stringToVec3(objectNode->first_attribute("dir")->value());
    p_light->setDirection(direction);
  }

  p_light->setPosition(pos);
  p_light->setRotation(rot);
  p_light->setScale(scl);
  p_light->setup();
  addMesh(p_light);
}

void R2::Scene::setupBillboardObject(rapidxml::xml_node<>* objectNode)
{
  glm::vec3 pos = m_papplication->getUtils()->stringToVec3(objectNode->first_attribute("pos")->value());
  glm::vec3 rot = glm::vec3(0.0f, 0.0f, 0.0f);

  if (objectNode->first_attribute("rot") != nullptr)
  {
    rot = m_papplication->getUtils()->stringToVec3(objectNode->first_attribute("rot")->value());
  }

  glm::vec3 scl = glm::vec3(1.0f, 1.0f, 1.0f);

  if (objectNode->first_attribute("scl") != nullptr)
  {
    scl = m_papplication->getUtils()->stringToVec3(objectNode->first_attribute("scl")->value());
  }

  std::string texturePath = m_papplication->getUtils()->getFilePath("/resources/default.png");

  if (objectNode->first_attribute("texture") != nullptr)
  {
    texturePath = m_papplication->getUtils()->getFilePath(objectNode->first_attribute("texture")->value());
  }

  Billboard* p_billboard = new Billboard(m_papplication, texturePath);
  p_billboard->setName(objectNode->first_attribute("name")->value());
  p_billboard->setPosition(pos);
  p_billboard->setRotation(rot);
  p_billboard->setScale(scl);
  p_billboard->setup();
  addMesh(p_billboard);
}

glm::vec3 R2::Scene::getGravityDirection()
{
  return m_gravityDirection;
}

void R2::Scene::setGravityDirection(glm::vec3 gravityDirection)
{
  m_gravityDirection = gravityDirection;
}

void R2::Scene::setGravityForce(float gravity)
{
  m_gravityForce = gravity;
}

float R2::Scene::getGravityForce()
{
  return m_gravityForce;
}