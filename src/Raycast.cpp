#include <R2/Raycast.h>
#include <R2/Mesh.h>
#include <R2/Camera.h>
#include <glm/gtx/intersect.hpp>

R2::Raycast::Raycast(Application *p_application)
{
  //std::cout << "R2::Raycast::Raycast()" << std::endl;
  m_papplication = p_application;
}

void R2::Raycast::setOrigin(const glm::vec3 &origin)
{
  m_origin = origin;
}

void R2::Raycast::setDirection(const glm::vec3 &direction)
{
  m_direction = direction;
}

R2::Mesh* R2::Raycast::castRay() {
  float closestDistance = std::numeric_limits<float>::max();
  Mesh* closestMesh = nullptr;

  std::vector<Mesh*> sceneMeshes = m_papplication->getSceneManager()->getCurrentScene()->getMeshes();

  for (Mesh* mesh : sceneMeshes) 
  {
    if(mesh->getIsGroup() || mesh->getIsLight() || mesh->getIsCamera() || !mesh->getIsVisible()) 
    {
      continue;
    }

    glm::vec3 minBound = mesh->getBoundingBoxMin();
    glm::vec3 maxBound = mesh->getBoundingBoxMax();

    float tMin, tMax;
    bool intersected = m_papplication->getUtils()->rayIntersectsAABB(m_origin, m_direction, minBound, maxBound, tMin, tMax);

    if (intersected && tMin < closestDistance) {
      closestDistance = tMin;
      closestMesh = mesh;
    }
  }

  return closestMesh;
}