#include <iostream>
#include <R2/Light.h>
#include <R2/Renderer.h>

R2::Light::Light(Application *p_application) : Billboard(p_application, p_application->getUtils()->getFilePath("/resources/light.png"))
{
  std::cout << "R2::Light::Light()" << std::endl;
  m_isLight = true;
  m_papplication = p_application;
}

void R2::Light::setup()
{
  std::cout << "R2::Light::setup()" << std::endl;

  if (m_isDirectionalLight)
  {
    glGenFramebuffers(1, &m_depthMapFBO);

    glGenTextures(1, &m_depthMap);
    glBindTexture(GL_TEXTURE_2D, m_depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_shadowWidth, m_shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
      std::cerr << "Directional light framebuffer is not complete!" << std::endl;
    }
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  else if (m_isPointLight)
  {
    glGenFramebuffers(1, &m_depthMapFBO);

    glGenTextures(1, &m_depthMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_depthMap);
    for (unsigned int i = 0; i < 6; ++i)
    {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, m_shadowWidth, m_shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthMap, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
      std::cerr << "Point light framebuffer is not complete!" << std::endl;
    }
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  }

  std::cout << "R2::Light::Light() - m_depthMapFBO: " << m_depthMapFBO << std::endl;
  std::cout << "R2::Light::Light() - m_depthMap: " << m_depthMap << std::endl;

  Billboard::setup();
}

void R2::Light::setIntensity(float intensity)
{
  m_intensity = intensity;
}

float R2::Light::getIntensity() const
{
  return m_intensity;
}

void R2::Light::setAmbient(glm::vec3 ambient)
{
  m_ambient = ambient;
}

glm::vec3 R2::Light::getAmbient() const
{
  return m_ambient;
}

void R2::Light::setDiffuse(glm::vec3 diffuse)
{
  m_diffuse = diffuse;
}

glm::vec3 R2::Light::getDiffuse() const
{
  return m_diffuse;
}

void R2::Light::setSpecular(glm::vec3 specular)
{
  m_specular = specular;
}

glm::vec3 R2::Light::getSpecular() const
{
  return m_specular;
}

void R2::Light::setConstant(float constant)
{
  m_constant = constant;
}

float R2::Light::getConstant() const
{
  return m_constant;
}

void R2::Light::setLinear(float linear)
{
  m_linear = linear;
}

float R2::Light::getLinear() const
{
  return m_linear;
}

void R2::Light::setQuadratic(float quadratic)
{
  m_quadratic = quadratic;
}

float R2::Light::getQuadratic() const
{
  return m_quadratic;
}

void R2::Light::setDirection(glm::vec3 direction)
{
  m_direction = direction;
}

glm::vec3 R2::Light::getDirection() const
{
  return m_direction;
}

void R2::Light::updateShadowMap(Shader *p_shader, Camera *p_camera)
{
  if (!p_shader)
  {
    std::cout << "R2::Light::updateShadowMap() - Shader is not valid" << std::endl;
    return;
  }

  if (m_isDirectionalLight)
  {
    m_lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 30.0f);
    m_lightView = glm::lookAt(m_position, m_position + m_direction, glm::vec3(0.0f, 1.0f, 0.0f));
    m_lightSpaceMatrix = m_lightProjection * m_lightView;

    glViewport(0, 0, m_shadowWidth, m_shadowHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);

    glClear(GL_DEPTH_BUFFER_BIT);

    m_papplication->getRenderer()->renderShadowMap(p_shader, m_papplication->getSceneManager()->getCurrentScene(), m_lightSpaceMatrix);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
  else if (m_isPointLight)
  {
    m_lightProjection = glm::perspective(glm::radians(90.0f), (float)m_shadowWidth / (float)m_shadowHeight, m_nearPlane, m_farPlane);
    m_lightSpaceMatrices.clear();
    m_lightSpaceMatrices.push_back(m_lightProjection * glm::lookAt(m_position, m_position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    m_lightSpaceMatrices.push_back(m_lightProjection * glm::lookAt(m_position, m_position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    m_lightSpaceMatrices.push_back(m_lightProjection * glm::lookAt(m_position, m_position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    m_lightSpaceMatrices.push_back(m_lightProjection * glm::lookAt(m_position, m_position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
    m_lightSpaceMatrices.push_back(m_lightProjection * glm::lookAt(m_position, m_position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    m_lightSpaceMatrices.push_back(m_lightProjection * glm::lookAt(m_position, m_position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

    glViewport(0, 0, m_shadowWidth, m_shadowHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);

    glClear(GL_DEPTH_BUFFER_BIT);

    m_papplication->getRenderer()->renderShadowMap(p_shader, m_papplication->getSceneManager()->getCurrentScene(), m_lightSpaceMatrices, m_position, m_farPlane);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
}

glm::mat4 R2::Light::getLightSpaceMatrix() const
{
  return m_lightSpaceMatrix;
}

std::vector<glm::mat4> R2::Light::getLightSpaceMatrices() const
{
  return m_lightSpaceMatrices;
}

GLuint R2::Light::getDepthMap() const
{
  return m_depthMap;
}

void R2::Light::cleanup()
{
  std::cout << "R2::Light::cleanup()" << std::endl;
  glDeleteFramebuffers(1, &m_depthMapFBO);
  glDeleteTextures(1, &m_depthMap);

  m_pvao->cleanup();
  m_pvbo->cleanup();
}

void R2::Light::setNearPlane(float nearPlane)
{
  m_nearPlane = nearPlane;
}

float R2::Light::getNearPlane() const
{
  return m_nearPlane;
}

void R2::Light::setFarPlane(float farPlane)
{
  m_farPlane = farPlane;
}

float R2::Light::getFarPlane() const
{
  return m_farPlane;
}

GLuint R2::Light::getDepthMapFBO() const
{
  return m_depthMapFBO;
}