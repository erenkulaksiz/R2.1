#include <iostream>
#include <R2/Imguizmo.h>
#include <R2/Camera.h>
#include <R2/Mesh.h>
#include <imgui.h>
#include <imguizmo/ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

static const float identityMatrix[16] =
{ 1.f, 0.f, 0.f, 0.f,
  0.f, 1.f, 0.f, 0.f,
  0.f, 0.f, 1.f, 0.f,
  0.f, 0.f, 0.f, 1.f };

R2::Imguizmo::Imguizmo(Application* p_application)
  : m_papplication(p_application), m_gizmoOperation(ImGuizmo::OPERATION::TRANSLATE), m_useSnap(false)
{
  std::cout << "Imguizmo::Imguizmo()" << std::endl;
  ImGuizmo::Enable(true);
}

void R2::Imguizmo::loop()
{
  ImGuizmo::BeginFrame();
  ImGuizmo::SetOrthographic(false);
  ImGuizmo::SetRect(0, 0, m_papplication->getScreenWidth(), m_papplication->getScreenHeight());

  Camera* p_camera = m_papplication->getSceneManager()->getCurrentScene()->getCamera();

  glm::mat4 view = p_camera->getViewMatrix();
  glm::mat4 projection = p_camera->getProjectionMatrix();

  Mesh* selectedMesh = m_papplication->getImgui()->getSelectedMesh();

  if (selectedMesh != nullptr)
  {
    glm::vec3 position = selectedMesh->getPosition();
    glm::quat rotation = selectedMesh->getRotationQuaternion();
    glm::vec3 scale = selectedMesh->getScale();

    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    model *= glm::toMat4(rotation);
    model = glm::scale(model, scale);

    float snap[3] = { 0.0f, 0.0f, 0.0f };
    if (m_useSnap)
    {
      switch (m_gizmoOperation)
      {
      case ImGuizmo::OPERATION::TRANSLATE:
        snap[0] = m_snapTranslation.x;
        snap[1] = m_snapTranslation.y;
        snap[2] = m_snapTranslation.z;
        break;
      case ImGuizmo::OPERATION::ROTATE:
        snap[0] = m_snapRotation;
        break;
      case ImGuizmo::OPERATION::SCALE:
        snap[0] = m_snapScale.x;
        snap[1] = m_snapScale.y;
        snap[2] = m_snapScale.z;
        break;
      }
    }

    glm::vec3 minBounds, maxBounds;
    minBounds = selectedMesh->getBoundingBoxMin();
    maxBounds = selectedMesh->getBoundingBoxMax();

    float bounds[6] = { minBounds.x, minBounds.y, minBounds.z, maxBounds.x, maxBounds.y, maxBounds.z };

    ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection), m_gizmoOperation, m_gizmoMode, glm::value_ptr(model), nullptr, m_useSnap ? snap : nullptr, m_boundSizing ? bounds : nullptr);

    if (ImGuizmo::IsUsing())
    {
      glm::vec3 translation, newScale, skew;
      glm::quat newRotation;
      glm::vec4 perspective;

      glm::decompose(model, newScale, newRotation, translation, skew, perspective);

      if (m_gizmoOperation == ImGuizmo::OPERATION::TRANSLATE)
      {
        selectedMesh->setPosition(translation);
      }
      else if (m_gizmoOperation == ImGuizmo::OPERATION::ROTATE)
      {
        selectedMesh->setRotationQuaternion(newRotation);
      }
      else if (m_gizmoOperation == ImGuizmo::OPERATION::SCALE)
      {
        selectedMesh->setScale(newScale);
      }
    }
  }
}

void R2::Imguizmo::setGizmoOperation(ImGuizmo::OPERATION operation)
{
  m_gizmoOperation = operation;
}

ImGuizmo::OPERATION R2::Imguizmo::getGizmoOperation()
{
  return m_gizmoOperation;
}

void R2::Imguizmo::setGizmoMode(ImGuizmo::MODE mode)
{
  m_gizmoMode = mode;
}

ImGuizmo::MODE R2::Imguizmo::getGizmoMode()
{
  return m_gizmoMode;
}

void R2::Imguizmo::setUseSnap(bool useSnap)
{
  m_useSnap = useSnap;
}

bool R2::Imguizmo::getUseSnap()
{
  return m_useSnap;
}

void R2::Imguizmo::setSnapValues(const glm::vec3& snapTranslation, float snapRotation, const glm::vec3& snapScale)
{
  m_snapTranslation = snapTranslation;
  m_snapRotation = snapRotation;
  m_snapScale = snapScale;
}

glm::vec3 R2::Imguizmo::getSnapTranslation()
{
  return m_snapTranslation;
}

float R2::Imguizmo::getSnapRotation()
{
  return m_snapRotation;
}

glm::vec3 R2::Imguizmo::getSnapScale()
{
  return m_snapScale;
}

void R2::Imguizmo::setBoundSizing(bool boundSizing)
{
  m_boundSizing = boundSizing;
}

bool R2::Imguizmo::getBoundSizing()
{
  return m_boundSizing;
}