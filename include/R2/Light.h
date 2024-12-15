#pragma once
#ifndef LIGHT_H
#define LIGHT_H
#include <R2/Billboard.h>
#include <R2/Application.h>

namespace R2
{
  class Billboard;
}

namespace R2
{
  class Light : public Billboard
  {
  public:
    Light(Application *p_application);
    ~Light();
    void setup() override;
    void cleanup() override;
    float getIntensity() const;
    void setIntensity(float intensity);
    glm::vec3 getAmbient() const;
    void setAmbient(glm::vec3 ambient);
    glm::vec3 getDiffuse() const;
    void setDiffuse(glm::vec3 diffuse);
    glm::vec3 getSpecular() const;
    void setSpecular(glm::vec3 specular);
    float getConstant() const;
    void setConstant(float constant);
    float getLinear() const;
    void setLinear(float linear);
    float getQuadratic() const;
    void setQuadratic(float quadratic);
    void setDirection(glm::vec3 direction);
    glm::vec3 getDirection() const;
    void updateDirectionalShadowMap(Shader *p_shader, Camera *p_camera);
    void updatePointShadowMap(Shader *p_shader, Camera *p_camera, int lightIndex);
    void updateSpotShadowMap(Shader *p_shader, Camera *p_camera);
    glm::mat4 getLightSpaceMatrix() const;
    GLuint getDepthMap() const;
    void setNearPlane(float nearPlane);
    float getNearPlane() const;
    void setFarPlane(float farPlane);
    float getFarPlane() const;
    std::vector<glm::mat4> getLightSpaceMatrices() const;
    GLuint getDepthMapFBO() const;
    GLuint getDepthCubemap() const;
    void setCutOff(float cutOffDegrees);
    float getCutOff() const;
    void setOuterCutOff(float outerCutOffDegrees);
    float getOuterCutOff() const;

  private:
    Application *m_papplication;
    glm::vec3 m_ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    glm::vec3 m_diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    glm::vec3 m_specular = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 m_direction = glm::vec3(0.0f, -1.0f, 0.0f);
    float m_constant = 1.0f;
    float m_linear = 0.09f;
    float m_quadratic = 0.032f;
    float m_intensity = 1.0f;
    GLuint m_depthMapFBO;
    GLuint m_depthMap;
    GLuint m_depthCubemap;
    int m_shadowWidth = 2048;
    int m_shadowHeight = 2048;
    glm::mat4 m_lightSpaceMatrix;
    glm::mat4 m_lightView;
    glm::mat4 m_lightProjection;
    std::vector<glm::mat4> m_lightSpaceMatrices;
    float m_nearPlane = 1.0f;
    float m_farPlane = 100.0f;
    float m_cutOff = glm::cos(glm::radians(12.5f));
    float m_outerCutOff = glm::cos(glm::radians(17.5f));
  };
}

#endif