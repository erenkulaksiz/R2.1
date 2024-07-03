#include <R2/MeshGroup.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

R2::MeshGroup::MeshGroup() : Mesh()
{
  std::cout << "R2::MeshGroup::MeshGroup()" << std::endl;
  m_isGroup = true;
}

R2::MeshGroup::~MeshGroup()
{
  std::cout << "R2::MeshGroup::~MeshGroup()" << std::endl;
  cleanup();
}

void R2::MeshGroup::addMesh(Mesh *p_mesh)
{
  std::cout << "R2::MeshGroup::addMesh()" << std::endl;
  m_meshes.push_back(p_mesh);
}

void R2::MeshGroup::render(Camera *p_camera, Scene *p_scene)
{
  for (size_t i = 0; i < m_meshes.size(); i++)
  {
    m_meshes[i]->render(p_camera, p_scene);
  }
}

void R2::MeshGroup::cleanup()
{
  std::cout << "R2::MeshGroup::cleanup()" << std::endl;
  for (size_t i = 0; i < m_meshes.size(); i++)
  {
    std::cout << "R2::MeshGroup::cleanup() deleting mesh " << m_meshes[i]->getName() << std::endl;
    delete m_meshes[i];
  }
}

void R2::MeshGroup::setPosition(glm::vec3 position)
{
  glm::vec3 offset = position - m_position;
  m_position = position;
  for (size_t i = 0; i < m_meshes.size(); i++)
  {
    m_meshes[i]->setPosition(m_meshes[i]->getPosition() + offset);
  }
}

void R2::MeshGroup::setRotation(glm::vec3 rotation)
{
  glm::quat oldRotationQuat = glm::quat(glm::radians(m_rotation));
  glm::quat newRotationQuat = glm::quat(glm::radians(rotation));
  glm::quat rotationOffset = glm::inverse(oldRotationQuat) * newRotationQuat;
  
  m_rotation = rotation;

  for (size_t i = 0; i < m_meshes.size(); i++)
  {
    glm::vec3 relativePosition = m_meshes[i]->getPosition() - m_position;
    relativePosition = rotationOffset * relativePosition;
    m_meshes[i]->setPosition(m_position + relativePosition);

    glm::quat meshRotation = glm::quat(glm::radians(m_meshes[i]->getRotation()));
    meshRotation = rotationOffset * meshRotation;
    m_meshes[i]->setRotation(glm::degrees(glm::eulerAngles(meshRotation)));
  }
}

void R2::MeshGroup::setScale(glm::vec3 scale)
{
  glm::vec3 scaleOffset = scale / m_scale;
  m_scale = scale;
  
  for (size_t i = 0; i < m_meshes.size(); i++)
  {
    glm::vec3 relativePosition = m_meshes[i]->getPosition() - m_position;
    relativePosition *= scaleOffset;
    m_meshes[i]->setPosition(m_position + relativePosition);

    glm::vec3 meshScale = m_meshes[i]->getScale();
    meshScale *= scaleOffset;
    m_meshes[i]->setScale(meshScale);
  }
}

std::vector<R2::Mesh *> R2::MeshGroup::getChildMeshes()
{
  return m_meshes;
}

void R2::MeshGroup::renderDirectionalShadowMap(Shader *p_shader, glm::mat4 lightSpaceMatrix)
{
  for (size_t i = 0; i < m_meshes.size(); i++)
  {
    m_meshes[i]->renderDirectionalShadowMap(p_shader, lightSpaceMatrix);
  }
}

void R2::MeshGroup::renderPointShadowMap(Shader *p_shader, std::vector<glm::mat4> lightSpaceMatrix, glm::vec3 lightPos, float farPlane)
{
  for (size_t i = 0; i < m_meshes.size(); i++)
  {
    m_meshes[i]->renderPointShadowMap(p_shader, lightSpaceMatrix, lightPos, farPlane);
  }
}

void R2::MeshGroup::setIsVisible(bool isVisible)
{
  m_isVisible = isVisible;
  for (size_t i = 0; i < m_meshes.size(); i++)
  {
    m_meshes[i]->setIsVisible(isVisible);
  }
}

void R2::MeshGroup::drawBoundingBox(Scene *p_scene)
{
  for (size_t i = 0; i < m_meshes.size(); i++)
  {
    m_meshes[i]->drawBoundingBox(p_scene);
  }
}

void R2::MeshGroup::setIsDrawingBoundingBox(bool isDrawingBoundingBox)
{
  m_isDrawingBoundingBox = isDrawingBoundingBox;
  for (size_t i = 0; i < m_meshes.size(); i++)
  {
    m_meshes[i]->setIsDrawingBoundingBox(isDrawingBoundingBox);
  }
}