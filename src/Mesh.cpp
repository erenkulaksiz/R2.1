#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <R2/Mesh.h>
#include <R2/Camera.h>
#include <R2/Light.h>
#include <R2/MeshGroup.h>

R2::Mesh::Mesh(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, float* p_vertices, unsigned int* p_indices, size_t vertexCount, size_t indexCount, Shader* p_shader)
{
  std::cout << "R2::Mesh::Mesh()" << std::endl;
  m_position = position;
  m_rotation = rotation;
  m_scale = scale;
  m_pvertices = p_vertices;
  m_pindices = p_indices;
  m_vertexCount = vertexCount;
  m_indexCount = indexCount;
  m_pshader = p_shader;
}

R2::Mesh::Mesh(glm::vec3 position, float* p_vertices, unsigned int* p_indices, size_t vertexCount, size_t indexCount, Shader* p_shader)
{
  std::cout << "R2::Mesh::Mesh()" << std::endl;
  m_position = position;
  m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
  m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
  m_pvertices = p_vertices;
  m_pindices = p_indices;
  m_vertexCount = vertexCount;
  m_indexCount = indexCount;
  m_pshader = p_shader;
}

R2::Mesh::Mesh(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, Shader* p_shader)
{
  std::cout << "R2::Mesh::Mesh()" << std::endl;
  m_position = position;
  m_rotation = rotation;
  m_scale = scale;
  m_pshader = p_shader;
}

R2::Mesh::Mesh(Shader* p_shader)
{
  std::cout << "R2::Mesh::Mesh()" << std::endl;
  m_pshader = p_shader;
}

R2::Mesh::Mesh()
{
  std::cout << "R2::Mesh::Mesh()" << std::endl;
}

void R2::Mesh::setup()
{
  std::cout << "R2::Mesh::setup()" << std::endl;

  std::cout << "R2::Mesh::setup() vertexCount: " << m_vertexCount << std::endl;
  std::cout << "R2::Mesh::setup() indexCount: " << m_indexCount << std::endl;

  m_pvao = new VAO();
  m_pvao->bind();
  
  m_pvbo = new VBO(m_pvertices, m_vertexCount);
  m_pebo = new EBO(m_pindices, m_indexCount);

  if (m_isLine)
  {
    m_pvao->linkVBO(*m_pvbo, 0);
  }
  else
  {
    m_pvao->linkAttrib(*m_pvbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    m_pvao->linkAttrib(*m_pvbo, 1, 2, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    m_pvao->linkAttrib(*m_pvbo, 2, 3, GL_FLOAT, 8 * sizeof(float), (void*)(5 * sizeof(float)));
  }

  m_pvao->unbind();
  m_pvbo->unbind();
  m_pebo->unbind();
}

void R2::Mesh::setVertices(float* vertices, size_t vertexCount)
{
  this->m_pvertices = vertices;
  this->m_vertexCount = vertexCount;
}

void R2::Mesh::setIndices(unsigned int* indices, size_t indexCount)
{
  this->m_pindices = indices;
  this->m_indexCount = indexCount;
}

void R2::Mesh::setPosition(glm::vec3 position)
{
  this->m_position = position;
}

void R2::Mesh::setRotation(glm::vec3 rotation)
{
  this->m_rotation = rotation;
}

void R2::Mesh::setScale(glm::vec3 scale)
{
  this->m_scale = scale;
}

glm::vec3 R2::Mesh::getPosition()
{
  return m_position;
}

glm::vec3 R2::Mesh::getRotation()
{
  return m_rotation;
}

glm::vec3 R2::Mesh::getScale()
{
  return m_scale;
}

R2::Shader* R2::Mesh::getShader()
{
  return m_pshader;
}

void R2::Mesh::setIsCamera(bool isCamera)
{
  m_isCamera = isCamera;
}

bool R2::Mesh::getIsCamera()
{
  return m_isCamera;
}

void R2::Mesh::setName(std::string name)
{
  m_name = name;
}

std::string R2::Mesh::getName()
{
  return m_name;
}

void R2::Mesh::render(Camera* p_camera, Scene* p_scene)
{
  if (m_isCamera || !m_isVisible)
  {
    return;
  }

  if (m_isDrawingBoundingBox)
  {
    drawBoundingBox(p_scene);
  }

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);
  glViewport(0, 0, p_scene->getApplication()->getScreenWidth(), p_scene->getApplication()->getScreenHeight());

  m_pvao->bind();
  m_pshader->activate();

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, m_position);
  model = glm::rotate(model, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
  model = glm::scale(model, m_scale);

  m_pshader->setMat4("model", model);
  m_pshader->setMat4("view", p_camera->getViewMatrix());
  m_pshader->setMat4("projection", p_camera->getProjectionMatrix());
  m_pshader->setVec4("color", m_color);
  m_pshader->setVec3("camPos", p_camera->getPosition());

  for (size_t i = 0; i < m_ptextures.size(); i++)
  {
    m_ptextures[i]->bind();

    if (m_ptextures[i]->getShininess())
    {
      m_pshader->setFloat("material.shininess", m_ptextures[i]->getShininess());
    }
  }

  if (m_isBillboard)
  {
    m_pshader->setVec3("billboardPos", m_position);
  }

  if (!m_isLight && !m_isBillboard)
  {
    m_pshader->setBool("lightsEnabled", p_scene->getIsLightsActive());

    setShaderLightValues(p_scene->getPointLights(), "pointLights");
    setShaderLightValues(p_scene->getDirectionalLights(), "directionalLights");

    m_pshader->setInt("numberOfPointLights", p_scene->getPointLights().size());
    m_pshader->setInt("numberOfDirectionalLights", p_scene->getDirectionalLights().size());
  }

  glDrawElements(m_isLine ? GL_LINES : GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
}

void R2::Mesh::render(Camera* p_camera, Scene* p_scene, Shader* p_shader)
{
  if (m_isCamera || !m_isVisible)
  {
    return;
  }

  if (m_isDrawingBoundingBox)
  {
    drawBoundingBox(p_scene);
  }

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);
  glViewport(0, 0, p_scene->getApplication()->getScreenWidth(), p_scene->getApplication()->getScreenHeight());

  m_pvao->bind();
  p_shader->activate();

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, m_position);
  model = glm::rotate(model, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
  model = glm::scale(model, m_scale);

  p_shader->setMat4("model", model);
  p_shader->setMat4("view", p_camera->getViewMatrix());
  p_shader->setMat4("projection", p_camera->getProjectionMatrix());
  p_shader->setVec4("color", m_color);
  p_shader->setVec3("camPos", p_camera->getPosition());

  for (size_t i = 0; i < m_ptextures.size(); i++)
  {
    m_ptextures[i]->bind();

    if (m_ptextures[i]->getShininess())
    {
      m_pshader->setFloat("material.shininess", m_ptextures[i]->getShininess());
    }
  }

  if (m_isBillboard)
  {
    p_shader->setVec3("billboardPos", m_position);
  }

  if (!m_isLight && !m_isBillboard)
  {
    p_shader->setBool("lightsEnabled", p_scene->getIsLightsActive());

    setShaderLightValues(p_scene->getPointLights(), "pointLights", p_shader);
    setShaderLightValues(p_scene->getDirectionalLights(), "directionalLights", p_shader);

    p_shader->setInt("numberOfPointLights", p_scene->getPointLights().size());
    p_shader->setInt("numberOfDirectionalLights", p_scene->getDirectionalLights().size());
  }

  glDrawElements(m_isLine ? GL_LINES : GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
}

void R2::Mesh::renderShadowMap(Shader* p_shader, glm::mat4 lightSpaceMatrix)
{
  if (m_isCamera || !m_isVisible || m_isLight || m_isLine || m_isBillboard)
    return;

  m_pvao->bind();
  p_shader->activate();

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, m_position);
  model = glm::rotate(model, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
  model = glm::scale(model, m_scale);

  p_shader->setMat4("model", model);
  p_shader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

  glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
}

void R2::Mesh::renderShadowMap(Shader* p_shader, std::vector<glm::mat4> lightSpaceMatrix, glm::vec3 lightPos, float farPlane)
{
  if (m_isCamera || !m_isVisible || m_isLight || m_isLine || m_isBillboard)
    return;

  m_pvao->bind();
  p_shader->activate();

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, m_position);
  model = glm::rotate(model, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
  model = glm::scale(model, m_scale);

  p_shader->setVec3("lightPos", lightPos);
  p_shader->setFloat("farPlane", farPlane);
  p_shader->setMat4("model", model);

  for (int i = 0; i < lightSpaceMatrix.size(); i++)
  {
    p_shader->setMat4("shadowMatrices[" + std::to_string(i) + "]", lightSpaceMatrix[i]);
  }

  glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
}

void R2::Mesh::setShader(Shader* p_shader)
{
  m_pshader = p_shader;
}

void R2::Mesh::setIsLine(bool isLine)
{
  m_isLine = isLine;
}

void R2::Mesh::setIsVisible(bool isVisible)
{
  m_isVisible = isVisible;
}

bool R2::Mesh::getIsVisible()
{
  return m_isVisible;
}

void R2::Mesh::setColor(glm::vec4 color)
{
  m_color = color;
}

glm::vec4 R2::Mesh::getColor()
{
  return m_color;
}

void R2::Mesh::addTexture(Texture* p_texture)
{
  m_ptextures.push_back(p_texture);
}

void R2::Mesh::setIsBillboard(bool isBillboard)
{
  m_isBillboard = isBillboard;
}

bool R2::Mesh::getIsBillboard()
{
  return m_isBillboard;
}

bool R2::Mesh::getIsTextured()
{
  return m_ptextures.size() > 0;
}

void R2::Mesh::setIsVisibleOnMenu(bool isVisibleOnMenu)
{
  m_isVisibleOnMenu = isVisibleOnMenu;
}

bool R2::Mesh::getIsVisibleOnMenu()
{
  return m_isVisibleOnMenu;
}

void R2::Mesh::cleanup()
{
  std::cout << "R2::Mesh::cleanup()" << std::endl;

  if (m_isCamera || m_isLight)
    return;

  m_pvao->cleanup();
  m_pvbo->cleanup();
  m_pebo->cleanup();

  for (size_t i = 0; i < m_ptextures.size(); i++)
  {
    m_ptextures[i]->cleanup();
  }
}

void R2::Mesh::setIsPointLight(bool isPointLight)
{
  m_isPointLight = isPointLight;
}

bool R2::Mesh::getIsPointLight()
{
  return m_isPointLight;
}

void R2::Mesh::setIsLight(bool isLight)
{
  m_isLight = isLight;
}

bool R2::Mesh::getIsLight()
{
  return m_isLight;
}

bool R2::Mesh::getIsGroup()
{
  return m_isGroup;
}

int R2::Mesh::getFaceCount()
{
  return (m_vertexCount / 64 / 2);
}

std::vector<R2::Texture*> R2::Mesh::getTextures()
{
  return m_ptextures;
}

void R2::Mesh::setIsGroup(bool isGroup)
{
  m_isGroup = isGroup;
}

void R2::Mesh::setIsDirectionalLight(bool isDirectionalLight)
{
  m_isDirectionalLight = isDirectionalLight;
}

bool R2::Mesh::getIsDirectionalLight()
{
  return m_isDirectionalLight;
}

void R2::Mesh::setShaderLightValues(std::vector<Light*> p_lights, std::string lightType)
{
  for (int i = 0; i < p_lights.size(); i++)
  {
    if (!p_lights[i]->getIsVisible())
    {
      m_pshader->setBool(lightType + "[" + std::to_string(i) + "].enabled", false);
      continue;
    }

    m_pshader->setBool(lightType + "[" + std::to_string(i) + "].enabled", true);
    m_pshader->setVec3(lightType + "[" + std::to_string(i) + "].position", p_lights[i]->getPosition());
    m_pshader->setVec4(lightType + "[" + std::to_string(i) + "].color", p_lights[i]->getColor());
    m_pshader->setFloat(lightType + "[" + std::to_string(i) + "].intensity", p_lights[i]->getIntensity());
    m_pshader->setFloat(lightType + "[" + std::to_string(i) + "].constant", p_lights[i]->getConstant());
    m_pshader->setFloat(lightType + "[" + std::to_string(i) + "].linear", p_lights[i]->getLinear());
    m_pshader->setFloat(lightType + "[" + std::to_string(i) + "].quadratic", p_lights[i]->getQuadratic());
    m_pshader->setVec3(lightType + "[" + std::to_string(i) + "].specular", p_lights[i]->getSpecular());
    m_pshader->setVec3(lightType + "[" + std::to_string(i) + "].diffuse", p_lights[i]->getDiffuse());
    m_pshader->setVec3(lightType + "[" + std::to_string(i) + "].ambient", p_lights[i]->getAmbient());

    if (p_lights[i]->getIsDirectionalLight())
    {
      glActiveTexture(GL_TEXTURE16);
      GLuint depthMapTexture = p_lights[i]->getDepthMap();
      glBindTexture(GL_TEXTURE_2D, depthMapTexture);

      m_pshader->setVec3(lightType + "[" + std::to_string(i) + "].direction", p_lights[i]->getDirection());
      m_pshader->setMat4(lightType + "[" + std::to_string(i) + "].lightSpaceMatrix", p_lights[i]->getLightSpaceMatrix());
      m_pshader->setInt(lightType + "[" + std::to_string(i) + "].shadowMap", 16);
    }

    if (p_lights[i]->getIsPointLight())
    {
      glActiveTexture(GL_TEXTURE17 + i);
      GLuint depthMapTexture = p_lights[i]->getDepthMap();
      glBindTexture(GL_TEXTURE_CUBE_MAP, depthMapTexture);

      m_pshader->setFloat(lightType + "[" + std::to_string(i) + "].farPlane", p_lights[i]->getFarPlane());
      m_pshader->setInt(lightType + "[" + std::to_string(i) + "].shadowMap", i + 17);
    }
  }
}

void R2::Mesh::setShaderLightValues(std::vector<Light*> p_lights, std::string lightType, Shader* p_shader)
{
  for (int i = 0; i < p_lights.size(); i++)
  {
    if (!p_lights[i]->getIsVisible())
    {
      p_shader->setBool(lightType + "[" + std::to_string(i) + "].enabled", false);
      continue;
    }

    p_shader->setBool(lightType + "[" + std::to_string(i) + "].enabled", true);
    p_shader->setVec3(lightType + "[" + std::to_string(i) + "].position", p_lights[i]->getPosition());
    p_shader->setVec4(lightType + "[" + std::to_string(i) + "].color", p_lights[i]->getColor());
    p_shader->setFloat(lightType + "[" + std::to_string(i) + "].intensity", p_lights[i]->getIntensity());
    p_shader->setFloat(lightType + "[" + std::to_string(i) + "].constant", p_lights[i]->getConstant());
    p_shader->setFloat(lightType + "[" + std::to_string(i) + "].linear", p_lights[i]->getLinear());
    p_shader->setFloat(lightType + "[" + std::to_string(i) + "].quadratic", p_lights[i]->getQuadratic());
    p_shader->setVec3(lightType + "[" + std::to_string(i) + "].specular", p_lights[i]->getSpecular());
    p_shader->setVec3(lightType + "[" + std::to_string(i) + "].diffuse", p_lights[i]->getDiffuse());
    p_shader->setVec3(lightType + "[" + std::to_string(i) + "].ambient", p_lights[i]->getAmbient());

    if (p_lights[i]->getIsDirectionalLight())
    {
      glActiveTexture(GL_TEXTURE16);
      GLuint depthMapTexture = p_lights[i]->getDepthMap();
      glBindTexture(GL_TEXTURE_2D, depthMapTexture);

      p_shader->setVec3(lightType + "[" + std::to_string(i) + "].direction", p_lights[i]->getDirection());
      p_shader->setMat4(lightType + "[" + std::to_string(i) + "].lightSpaceMatrix", p_lights[i]->getLightSpaceMatrix());
      p_shader->setInt(lightType + "[" + std::to_string(i) + "].shadowMap", 16);
    }

    if (p_lights[i]->getIsPointLight())
    {
      glActiveTexture(GL_TEXTURE17 + i);
      GLuint depthMapTexture = p_lights[i]->getDepthMap();
      glBindTexture(GL_TEXTURE_CUBE_MAP, depthMapTexture);

      p_shader->setFloat(lightType + "[" + std::to_string(i) + "].farPlane", p_lights[i]->getFarPlane());
      p_shader->setInt(lightType + "[" + std::to_string(i) + "].shadowMap", i + 17);
    }
  }
}

glm::vec3 R2::Mesh::getBoundingBoxMin()
{
  return m_boundingBoxMin;
}

glm::vec3 R2::Mesh::getBoundingBoxMax()
{
  return m_boundingBoxMax;
}

void R2::Mesh::setIsDrawingBoundingBox(bool isDrawingBoundingBox)
{
  m_isDrawingBoundingBox = isDrawingBoundingBox;
}

bool R2::Mesh::getIsDrawingBoundingBox()
{
  return m_isDrawingBoundingBox;
}

void R2::Mesh::drawBoundingBox(Scene *p_scene)
{
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, m_position);
  model = glm::rotate(model, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
  model = glm::scale(model, m_scale);

  glm::vec3 min = m_boundingBoxMin;
  glm::vec3 max = m_boundingBoxMax;

  glm::vec3 vertices[] = {
    glm::vec3(min.x, min.y, min.z),
    glm::vec3(max.x, min.y, min.z),
    glm::vec3(max.x, max.y, min.z),
    glm::vec3(min.x, max.y, min.z),
    glm::vec3(min.x, min.y, max.z),
    glm::vec3(max.x, min.y, max.z),
    glm::vec3(max.x, max.y, max.z),
    glm::vec3(min.x, max.y, max.z)
  };

  unsigned int indices[] = {
    0, 1,
    1, 2,
    2, 3,
    3, 0,
    4, 5,
    5, 6,
    6, 7,
    7, 4,
    0, 4,
    1, 5,
    2, 6,
    3, 7
  };

  GLuint VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  Shader* p_shader = p_scene->getApplication()->getBoundingBoxShader();
  p_shader->activate();
  p_shader->setMat4("model", model);
  p_shader->setMat4("view", p_scene->getCamera()->getViewMatrix());
  p_shader->setMat4("projection", p_scene->getCamera()->getProjectionMatrix());

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glDisable(GL_DEPTH_TEST);
  glDrawElements(GL_LINES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glBindVertexArray(0);

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}

void R2::Mesh::setBoundingBox(glm::vec3 min, glm::vec3 max)
{
  m_boundingBoxMin = min;
  m_boundingBoxMax = max;
}