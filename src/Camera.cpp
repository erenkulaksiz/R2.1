#define GLFW_INCLUDE_NONE
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <R2/Camera.h>

R2::Camera::Camera(Application *p_application) : Mesh()
{
  std::cout << "R2::Camera::Camera()" << std::endl;
  this->m_isCamera = true;
  this->m_isVisible = false;
  this->m_papplication = p_application;
  this->m_position = glm::vec3(0.0f, 0.0f, 0.0f);
}

void R2::Camera::updateViewMatrix()
{
  if (m_isOrthographic)
    m_view = glm::lookAt(glm::vec3(m_position.x, m_position.y, 0.0f), glm::vec3(m_position.x, m_position.y, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  else if (m_isPerspective)
    m_view = glm::lookAt(m_position, m_position + m_cameraFront, m_cameraUp);
}

void R2::Camera::updateProjectionMatrix()
{
  if (m_isOrthographic)
    m_projection = glm::ortho(0.0f, (float)m_papplication->getScreenWidth() / m_zoom, (float)m_papplication->getScreenHeight() / m_zoom, 0.0f, m_near, m_far);
  else if (m_isPerspective)
    m_projection = glm::perspective(glm::radians(m_fov), (float)m_papplication->getScreenWidth() / (float)m_papplication->getScreenHeight(), m_near, m_far);
}

void R2::Camera::updateMatrix()
{
  updateViewMatrix();
  updateProjectionMatrix();
}

void R2::Camera::setActive(bool isActive)
{
  m_isActive = isActive;
  updateMatrix();
}

void R2::Camera::setFov(float fov)
{
  m_fov = fov;
  updateMatrix();
}

bool R2::Camera::getIsActive()
{
  return m_isActive;
}

float R2::Camera::getFov()
{
  return m_fov;
}

glm::mat4 R2::Camera::getViewMatrix()
{
  return m_view;
}

glm::mat4 R2::Camera::getProjectionMatrix()
{
  return m_projection;
}

void R2::Camera::loop()
{
  if (!m_isActive)
  {
    updateMatrix();
    return;
  }

  if (m_isFreecam)
  {
    if (m_papplication->getIsMouseVisible())
    {
      m_firstMouse = true;
      return;
    }

    if (m_papplication->getInput()->getScrollYDirection() != 0)
    {
      m_speed += m_papplication->getInput()->getScrollYDirection() * (m_speed / 2.0f);

      if (m_speed < 1.0f)
        m_speed = 1.0f;
    }

    float cameraSpeed = m_speed * m_papplication->getDeltaTime();

    if (m_isOrthographic)
    {
      if (m_papplication->getInput()->isPressedKey(GLFW_KEY_W))
      {
        m_position.y += cameraSpeed;
      }
      if (m_papplication->getInput()->isPressedKey(GLFW_KEY_S))
      {
        m_position.y -= cameraSpeed;
      }
      if (m_papplication->getInput()->isPressedKey(GLFW_KEY_A))
      {
        m_position.x -= cameraSpeed;
      }
      if (m_papplication->getInput()->isPressedKey(GLFW_KEY_D))
      {
        m_position.x += cameraSpeed;
      }
    }
    else
    {
      if (m_papplication->getInput()->isPressedKey(GLFW_KEY_W))
      {
        m_position += cameraSpeed * m_cameraFront;
      }
      if (m_papplication->getInput()->isPressedKey(GLFW_KEY_S))
      {
        m_position -= cameraSpeed * m_cameraFront;
      }
      if (m_papplication->getInput()->isPressedKey(GLFW_KEY_A))
      {
        m_position -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed;
      }
      if (m_papplication->getInput()->isPressedKey(GLFW_KEY_D))
      {
        m_position += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed;
      }
      if (m_papplication->getInput()->isPressedKey(GLFW_KEY_LEFT_SHIFT))
      {
        m_position += m_cameraUp * cameraSpeed;
      }
      if (m_papplication->getInput()->isPressedKey(GLFW_KEY_LEFT_CONTROL))
      {
        m_position -= m_cameraUp * cameraSpeed;
      }
    }

    double x = m_papplication->getInput()->getMouseX();
    double y = m_papplication->getInput()->getMouseY();

    if (m_firstMouse)
    {
      m_lastX = x;
      m_lastY = y;
      m_firstMouse = false;
    }

    float xOffset = x - m_lastX;
    float yOffset = m_lastY - y;

    m_lastX = x;
    m_lastY = y;

    xOffset *= m_sensitivity;
    yOffset *= m_sensitivity;

    m_yaw += xOffset;
    m_pitch += yOffset;

    if (m_pitch > 89.0f)
      m_pitch = 89.0f;
    if (m_pitch < -89.0f)
      m_pitch = -89.0f;

    if (m_yaw > 360.0f)
      m_yaw = 0.0f;
    if (m_yaw < 0.0f)
      m_yaw = 360.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    direction.y = sin(glm::radians(m_pitch));
    direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    m_cameraFront = glm::normalize(direction);
    m_rotation = glm::vec3(m_pitch, m_yaw, 0);
  }

  updateMatrix();
}

void R2::Camera::setNear(float near)
{
  m_near = near;
  updateMatrix();
}

void R2::Camera::setFar(float far)
{
  m_far = far;
  updateMatrix();
}

float R2::Camera::getNear()
{
  return m_near;
}

float R2::Camera::getFar()
{
  return m_far;
}

void R2::Camera::setIsFreecam(bool isFreecam)
{
  m_isFreecam = isFreecam;
}

bool R2::Camera::getIsFreecam()
{
  return m_isFreecam;
}

void R2::Camera::setIsPerspective(bool isPerspective)
{
  m_isPerspective = isPerspective;
  updateMatrix();
}

bool R2::Camera::getIsPerspective()
{
  return m_isPerspective;
}

void R2::Camera::setIsOrthographic(bool isOrthographic)
{
  m_isOrthographic = isOrthographic;
  updateMatrix();
}

bool R2::Camera::getIsOrthographic()
{
  return m_isOrthographic;
}

void R2::Camera::setZoom(float zoom)
{
  m_zoom = zoom;
  updateMatrix();
}

float R2::Camera::getZoom()
{
  return m_zoom;
}

void R2::Camera::setPosition(glm::vec3 position)
{
  m_position = position;
  updateMatrix();
}

void R2::Camera::setRotation(glm::vec3 rotation)
{
  m_rotation = rotation;
  updateMatrix();
}

void R2::Camera::setScale(glm::vec3 scale)
{
  m_scale = scale;
  updateMatrix();
}