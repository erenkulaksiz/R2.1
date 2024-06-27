#pragma once
#ifndef MESH_H
#define MESH_H
#include <vector>
#include <R2/shader/VAO.h>
#include <R2/shader/VBO.h>
#include <R2/shader/EBO.h>
#include <R2/shader/Shader.h>
#include <R2/Texture.h>

namespace R2
{
  class Light;
}

namespace R2
{
  class Shader;
  class Texture;
  class Camera;
  class Scene;

  class Mesh
  {
  public:
    Mesh(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, float *p_vertices, unsigned int *p_indices, size_t vertexCount, size_t indexCount, Shader *p_shader);
    Mesh(glm::vec3 position, float *p_vertices, unsigned int *p_indices, size_t vertexCount, size_t indexCount, Shader *p_shader);
    Mesh(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, Shader *shader);
    Mesh(Shader *shader);
    Mesh();
    virtual void setup();
    void setIndices(unsigned int *indices, size_t indexCount);
    void setVertices(float *vertices, size_t vertexCount);
    virtual void setPosition(glm::vec3 position);
    virtual void setRotation(glm::vec3 rotation);
    virtual void setScale(glm::vec3 scale);
    glm::vec3 getPosition();
    glm::vec3 getRotation();
    glm::vec3 getScale();
    Shader *getShader();
    void setIsCamera(bool isCamera);
    bool getIsCamera();
    void setName(std::string name);
    std::string getName();
    void setShader(Shader *p_shader);
    void setIsLine(bool isLine);
    virtual void render(Camera *p_camera, Scene *p_scene);
    virtual void render(Camera *p_camera, Scene *p_scene, Shader *p_shader);
    // Directional Light
    virtual void renderShadowMap(Shader *p_shader, glm::mat4 lightSpaceMatrix);
    // Point Light
    virtual void renderShadowMap(Shader *p_shader, std::vector<glm::mat4> lightSpaceMatrix, glm::vec3 lightPos, float farPlane);
    virtual void setIsVisible(bool isVisible);
    bool getIsVisible();
    void setColor(glm::vec4 color);
    glm::vec4 getColor();
    void addTexture(Texture *p_texture);
    void setIsBillboard(bool isBillboard);
    bool getIsBillboard();
    bool getIsTextured();
    void setIsVisibleOnMenu(bool isVisibleOnMenu);
    bool getIsVisibleOnMenu();
    virtual void cleanup();
    void setIsPointLight(bool isPointLight);
    bool getIsPointLight();
    void setIsLight(bool isLight);
    bool getIsLight();
    bool getIsGroup();
    int getFaceCount();
    std::vector<Texture *> getTextures();
    void setIsGroup(bool isGroup);
    void setIsDirectionalLight(bool isDirectionalLight);
    bool getIsDirectionalLight();
    glm::vec3 getBoundingBoxMin();
    glm::vec3 getBoundingBoxMax();
    void setIsDrawingBoundingBox(bool isDrawingBoundingBox);
    bool getIsDrawingBoundingBox();

  protected:
    Shader *m_pshader;
    std::vector<Texture *> m_ptextures;
    glm::vec3 m_position = glm::vec3(0.0f);
    glm::vec3 m_rotation = glm::vec3(0.0f);
    glm::vec3 m_scale = glm::vec3(1.0f);
    glm::vec4 m_color = glm::vec4(1.0f);
    VAO *m_pvao;
    VBO *m_pvbo;
    EBO *m_pebo;
    float *m_pvertices;
    unsigned int *m_pindices;
    size_t m_vertexCount;
    size_t m_indexCount;
    bool m_isCamera = false;
    bool m_isVisible = true;
    bool m_isLine = false;
    bool m_isBillboard = false;
    bool m_isVisibleOnMenu = true;
    bool m_isLight = false;
    bool m_isPointLight = false;
    bool m_isGroup = false;
    bool m_isDirectionalLight = false;
    bool m_isDrawingBoundingBox = false;
    std::string m_name;
    void setShaderLightValues(std::vector<Light *> p_lights, std::string lightType);
    void setShaderLightValues(std::vector<Light *> p_lights, std::string lightType, Shader *p_shader);
    glm::vec3 m_boundingBoxMin = glm::vec3(0.0f);
    glm::vec3 m_boundingBoxMax = glm::vec3(0.0f);
    bool m_boundingBoxInitialized = false;
    void drawBoundingBox(Camera *p_camera);
  };
}

#endif