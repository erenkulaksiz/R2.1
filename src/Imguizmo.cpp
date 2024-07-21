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
  ImGuizmo::AllowAxisFlip(false);

  Camera* p_camera = m_papplication->getSceneManager()->getCurrentScene()->getCamera();

  glm::mat4 cameraView = p_camera->getViewMatrix();
  glm::mat4 cameraProjection = p_camera->getProjectionMatrix();

  Mesh* selectedMesh = m_papplication->getImgui()->getSelectedMesh();

  ImGuizmo::ViewManipulate(glm::value_ptr(cameraView), 8.0f, ImVec2(m_papplication->getScreenWidth() - 100, 0), ImVec2(100, 100), 0x10101010);

  if (selectedMesh != nullptr)
  {
    glm::vec3 position = selectedMesh->getPosition();
    glm::quat rotation = selectedMesh->getRotationQuaternion();
    glm::vec3 scale = selectedMesh->getScale();

    glm::vec2 screenPosition = m_papplication->getUtils()->worldToScreen(position, cameraView, cameraProjection, m_papplication->getScreenWidth(), m_papplication->getScreenHeight());

    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    drawList->AddText(ImVec2(screenPosition.x, screenPosition.y), IM_COL32(255, 255, 255, 255), selectedMesh->getName().c_str());

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

    glm::vec3 minBounds = selectedMesh->getBoundingBoxMin();
    glm::vec3 maxBounds = selectedMesh->getBoundingBoxMax();

    float bounds[6] = { minBounds.x, minBounds.y, minBounds.z, maxBounds.x, maxBounds.y, maxBounds.z };

    ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), m_gizmoOperation, m_gizmoMode, glm::value_ptr(model), nullptr, m_useSnap ? snap : nullptr, m_boundSizing ? bounds : nullptr);

    if (ImGuizmo::IsUsing())
    {
      glm::vec3 translation, newScale, skew;
      glm::quat newRotation;
      glm::vec4 perspective;

      glm::decompose(model, newScale, newRotation, translation, skew, perspective);

      switch (m_gizmoOperation)
      {
      case ImGuizmo::OPERATION::TRANSLATE:
        selectedMesh->setPosition(translation);
        break;

      case ImGuizmo::OPERATION::ROTATE:
        selectedMesh->setRotationQuaternion(newRotation);
        break;

      case ImGuizmo::OPERATION::SCALE:
        selectedMesh->setScale(newScale);
        break;

      default:
        std::cout << "Imguizmo::loop() - Invalid gizmo operation" << std::endl;
        break;
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

void R2::Imguizmo::decomposeMatrix(const glm::mat4& matrix, glm::vec3& translation, glm::quat& rotation, glm::vec3& scale)
{
  glm::vec3 skew;
  glm::vec4 perspective;
  glm::decompose(matrix, scale, rotation, translation, skew, perspective);
}