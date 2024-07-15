#pragma once
#ifndef IM3D_H
#define IM3D_H
#include <R2/Application.h>
#include <imgui.h>
#include <imguizmo/ImGuizmo.h>

namespace R2
{
  class Application;

  class Imguizmo
  {
  public:
    Imguizmo(Application* p_application);
    void loop();
    void setGizmoOperation(ImGuizmo::OPERATION operation);
    ImGuizmo::OPERATION getGizmoOperation();
    void setGizmoMode(ImGuizmo::MODE mode);
    ImGuizmo::MODE getGizmoMode();
    void setUseSnap(bool useSnap);
    bool getUseSnap();
    void setSnapValues(const glm::vec3& snapTranslation, float snapRotation, const glm::vec3& snapScale);
    glm::vec3 getSnapTranslation();
    float getSnapRotation();
    glm::vec3 getSnapScale();
    void setBoundSizing(bool boundSizing);
    bool getBoundSizing();

  private:
    Application* m_papplication;
    ImGuizmo::OPERATION m_gizmoOperation = ImGuizmo::TRANSLATE;
    ImGuizmo::MODE m_gizmoMode = ImGuizmo::MODE::LOCAL;
    bool m_useSnap = false;
    bool m_boundSizing = false;
    glm::vec3 m_snapTranslation = glm::vec3(5.0f);
    float m_snapRotation = 5.0f;
    glm::vec3 m_snapScale = glm::vec3(5.0f);
  };
}

#endif