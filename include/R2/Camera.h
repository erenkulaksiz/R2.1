#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <R2/Application.h>

namespace R2
{
  class Camera : public Mesh
  {
  public:
    Camera(Application *p_application);
    void updateViewMatrix();
    void updateProjectionMatrix();
    void updateMatrix();
    void setIsActive(bool isActive);
    void setFov(float fov);
    bool getIsActive();
    float getFov();
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
    void loop();
    void setNear(float near);
    void setFar(float far);
    float getNear();
    float getFar();
    void setIsFreecam(bool isFreecam);
    bool getIsFreecam();
    void setIsPerspective(bool isPerspective);
    bool getIsPerspective();
    void setIsOrthographic(bool isOrthographic);
    bool getIsOrthographic();
    void setZoom(float zoom);
    float getZoom();
    void setPosition(glm::vec3 position) override;
    void setRotation(glm::vec3 rotation) override;
    void setScale(glm::vec3 scale) override;
    glm::vec3 getDirection();
    glm::vec3 getUp();

  private:
    Application *m_papplication;
    glm::mat4 m_view;
    glm::mat4 m_projection;
    glm::vec3 m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 m_direction = glm::vec3(0.0f, 0.0f, 0.0f);
    float m_fov = 80.0f;
    float m_yaw = 0.0f;
    float m_pitch = 0.0f;
    float m_speed = 2.5f;
    float m_sensitivity = 0.1f;
    float m_lastX = 0.0f;
    float m_lastY = 0.0f;
    float m_near = 0.1f;
    float m_far = 100.0f;
    float m_zoom = 45.0f; // used for orthographic
    bool m_isPerspective = true;
    bool m_isOrthographic = false;
    bool m_isFreecam = true;
    bool m_isActive = false;
    bool m_isFirstMouse = true;
  };
}

#endif