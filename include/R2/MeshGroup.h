#pragma once
#ifndef MESHGROUP_H
#define MESHGROUP_H
#include <vector>
#include <R2/Mesh.h>

namespace R2
{
  class MeshGroup : public Mesh
  {
  public:
    MeshGroup();
    ~MeshGroup() override;
    void addMesh(Mesh *p_mesh);
    void render(Camera *p_camera, Scene *p_scene) override;
    void cleanup() override;
    void setPosition(glm::vec3 position) override;
    void setRotation(glm::vec3 rotation) override;
    void setRotationQuaternion(glm::quat rotation) override;
    void setScale(glm::vec3 scale) override;
    void renderDirectionalShadowMap(Shader *p_shader, glm::mat4 lightSpaceMatrix) override;
    void renderPointShadowMap(Shader *p_shader, std::vector<glm::mat4> lightSpaceMatrix, glm::vec3 lightPos, float farPlane) override;
    std::vector<Mesh *> getChildMeshes();
    void setIsVisible(bool isVisible) override;
    void drawBoundingBox(Scene *p_scene) override;
    void setIsDrawingBoundingBox(bool isDrawingBoundingBox) override;

  private:
    std::vector<Mesh *> m_meshes;
  };
}

#endif