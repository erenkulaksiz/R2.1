#include <glad/glad.h>
#include <iostream>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <R2/Imgui.h>
#include <R2/Mesh.h>
#include <R2/Camera.h>
#include <R2/Renderer.h>
#include <R2/Light.h>
#include <R2/MeshGroup.h>
#include <R2/Imguizmo.h>
#include <glm/gtc/type_ptr.hpp>

R2::Imgui::Imgui(Application *m_papplication)
{
  std::cout << "Imgui::Imgui()" << std::endl;
  this->m_papplication = m_papplication;
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(glfwGetCurrentContext(), true);
  ImGui_ImplOpenGL3_Init("#version 330");
  setupFramebuffer();
}

void R2::Imgui::setupFramebuffer()
{
  std::cout << "Imgui::setupFramebuffer()" << std::endl;
  int width = m_papplication->getScreenWidth();
  int height = m_papplication->getScreenHeight();

  glGenFramebuffers(1, &m_framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

  glGenTextures(1, &m_textureColorbuffer);
  glBindTexture(GL_TEXTURE_2D, m_textureColorbuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureColorbuffer, 0);

  glGenRenderbuffers(1, &m_rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "Framebuffer not complete!" << std::endl;

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void R2::Imgui::resizeFramebuffer()
{
  std::cout << "Imgui::resizeFramebuffer()" << std::endl;
  int width = m_papplication->getScreenWidth();
  int height = m_papplication->getScreenHeight();

  if (width == 0 || height == 0)
  {
    std::cout << "Imgui::resizeFramebuffer() width or height is 0" << std::endl;
    return;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

  glBindTexture(GL_TEXTURE_2D, m_textureColorbuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glBindTexture(GL_TEXTURE_2D, 0);

  glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void R2::Imgui::newFrame()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void R2::Imgui::render()
{
  setupTheme();
  newFrame();
  drawGui();
  m_papplication->getImguizmo()->loop();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void R2::Imgui::setMouseInteractions(bool value)
{
  ImGuiIO &io = ImGui::GetIO();
  if (value == false)
  {
    io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
  }
  else
  {
    io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
  }
}

void R2::Imgui::drawGui()
{
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::Begin("application");

  bool isSettingUpCurrentScene = m_papplication->getSceneManager()->getCurrentScene()->getIsStartedSetup();
  if (isSettingUpCurrentScene)
  {
    ImGui::Text("setting up current scene...");
    ImGui::End();
    return;
  }

  ImGuizmo::OPERATION gizmoOperation = m_papplication->getImguizmo()->getGizmoOperation();
  if (ImGui::RadioButton("translate", gizmoOperation == ImGuizmo::TRANSLATE))
  {
    m_papplication->getImguizmo()->setGizmoOperation(ImGuizmo::TRANSLATE);
  }
  ImGui::SameLine();
  if (ImGui::RadioButton("rotate", gizmoOperation == ImGuizmo::ROTATE))
  {
    m_papplication->getImguizmo()->setGizmoOperation(ImGuizmo::ROTATE);
  }
  ImGui::SameLine();
  if (ImGui::RadioButton("scale", gizmoOperation == ImGuizmo::SCALE))
  {
    m_papplication->getImguizmo()->setGizmoOperation(ImGuizmo::SCALE);
    m_papplication->getImguizmo()->setGizmoMode(ImGuizmo::LOCAL);
  }

  if (gizmoOperation != ImGuizmo::SCALE)
  {
    ImGuizmo::MODE gizmoMode = m_papplication->getImguizmo()->getGizmoMode();
    if (ImGui::RadioButton("local", gizmoMode == ImGuizmo::LOCAL))
    {
      m_papplication->getImguizmo()->setGizmoMode(ImGuizmo::LOCAL);
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("world", gizmoMode == ImGuizmo::WORLD))
    {
      m_papplication->getImguizmo()->setGizmoMode(ImGuizmo::WORLD);
    }
  }

  /*
  bool useSnap = m_papplication->getImguizmo()->getUseSnap();
  if (ImGui::Checkbox("Snap", &useSnap))
  {
    m_papplication->getImguizmo()->setUseSnap(useSnap);
  }

  ImGui::SameLine();

  glm::vec3 snapTranslation = m_papplication->getImguizmo()->getSnapTranslation();
  float snapRotation = m_papplication->getImguizmo()->getSnapRotation();
  glm::vec3 snapScale = m_papplication->getImguizmo()->getSnapScale();

  switch (m_papplication->getImguizmo()->getGizmoOperation())
  {
  case ImGuizmo::TRANSLATE:
    if (ImGui::DragFloat3("Snap", &snapTranslation.x, 0.1f, -1000.0f, 1000.0f, "%.1f"))
    {
      m_papplication->getImguizmo()->setSnapValues(snapTranslation, snapRotation, snapScale);
    }
    break;
  case ImGuizmo::ROTATE:
    if (ImGui::DragFloat("Angle Snap", &snapRotation, 0.1f, -1000.0f, 1000.0f, "%.1f"))
    {
      m_papplication->getImguizmo()->setSnapValues(snapTranslation, snapRotation, snapScale);
    }
    break;
  case ImGuizmo::SCALE:
    if (ImGui::DragFloat3("Scale Snap", &snapScale.x, 0.1f, -1000.0f, 1000.0f, "%.1f"))
    {
      m_papplication->getImguizmo()->setSnapValues(snapTranslation, snapRotation, snapScale);
    }
    break;
  }

  bool boundSizing = m_papplication->getImguizmo()->getBoundSizing();
  if (ImGui::Checkbox("Bound Sizing", &boundSizing))
  {
    m_papplication->getImguizmo()->setBoundSizing(boundSizing);
  }*/

  if (ImGui::CollapsingHeader("statistics"))
  {
    ImGui::Text("fps: %f", m_papplication->getFps());
    ImGui::Text("deltaTime: %f", m_papplication->getDeltaTime() * 1000.0f);
    ImGui::Text("%dx%d", m_papplication->getScreenWidth(), m_papplication->getScreenHeight());
  }

  if (ImGui::CollapsingHeader("settings"))
  {
    bool isVsyncEnabled = m_papplication->getVsyncEnabled();

    if (ImGui::Checkbox("vsync", &isVsyncEnabled))
    {
      m_papplication->setVsyncEnabled(isVsyncEnabled);
    }

    if (ImGui::Button("debug"))
    {
      m_showingImGuiDebug = !m_showingImGuiDebug;
    }

    if (m_showingImGuiDebug)
    {
      ImGui::ShowMetricsWindow();
    }
  }

  std::vector<R2::Mesh *> p_meshes = m_papplication->getSceneManager()->getCurrentScene()->getMeshes();

  renderScenes(m_papplication->getSceneManager()->getScenes());
  renderScene(m_papplication->getSceneManager()->getCurrentScene());

  if (ImGui::CollapsingHeader("meshes"))
  {
    renderMeshTree(p_meshes);
  }

  if (m_selectedMesh != nullptr)
  {
    renderMeshInspector(m_selectedMesh);
  }

  ImGui::End();
}

void R2::Imgui::renderMeshInspector(Mesh *p_mesh)
{
  ImGui::Begin("inspector");

  renderMeshGeneral(p_mesh);
  renderMeshTransform(p_mesh);
  renderMeshPhysics(p_mesh);

  if (p_mesh->getIsCamera())
  {
    renderCameraSettings(static_cast<Camera *>(p_mesh));
  }

  if (p_mesh->getIsLight())
  {
    renderLightSettings(static_cast<Light *>(p_mesh));
  }

  if (p_mesh->getTextures().size() != 0)
  {
    renderMeshTextures(p_mesh);
  }

  ImGui::End();
}

void R2::Imgui::renderMeshGeneral(Mesh *p_mesh)
{
  if (ImGui::CollapsingHeader("general"))
  {
    char meshName[256];
    strcpy(meshName, p_mesh->getName().c_str());
    if (ImGui::InputText("name", meshName, sizeof(meshName)))
    {
      p_mesh->setName(meshName);
    }

    if (!p_mesh->getIsCamera())
    {
      bool isVisible = p_mesh->getIsVisible();
      if (ImGui::Checkbox("visible", &isVisible))
      {
        p_mesh->setIsVisible(isVisible);
      }

      if (!p_mesh->getIsGroup())
      {
        ImGui::Text("face count: %d", p_mesh->getFaceCount());
      }

      if (!p_mesh->getIsLight())
      {
        bool isDrawingBoundingBox = p_mesh->getIsDrawingBoundingBox();
        if (ImGui::Checkbox("bounding box", &isDrawingBoundingBox))
        {
          p_mesh->setIsDrawingBoundingBox(isDrawingBoundingBox);
        }
      }
    }
  }
}

void R2::Imgui::renderMeshPhysics(Mesh* p_mesh)
{
  if (p_mesh->getIsGroup() || p_mesh->getIsLight() || p_mesh->getIsCamera())
  {
    return;
  }

  if (ImGui::CollapsingHeader("physics"))
  {
    glm::vec3 velocity = p_mesh->getVelocity();
    float velocityVec3[3] = {velocity.x, velocity.y, velocity.z};
    if (ImGui::DragFloat3("velocity", velocityVec3, 0.1f, -1000.0f, 1000.0f, "%.1f"))
    {
      p_mesh->setVelocity(glm::vec3(velocityVec3[0], velocityVec3[1], velocityVec3[2]));
    }

    float mass = p_mesh->getMass();
    if (ImGui::DragFloat("mass", &mass, 0.1f, 0.0f, 100.0f))
    {
      p_mesh->setMass(mass);
    }

    bool hasPhysics = p_mesh->getHasPhysics();
    if (ImGui::Checkbox("has physics", &hasPhysics))
    {
      p_mesh->setHasPhysics(hasPhysics);
    }

    bool isAffectedByGravity = p_mesh->getIsAffectedByGravity();
    if (ImGui::Checkbox("gravity", &isAffectedByGravity))
    {
      p_mesh->setIsAffectedByGravity(isAffectedByGravity);
    }

    bool isStatic = p_mesh->getIsStatic();
    if (ImGui::Checkbox("static", &isStatic))
    {
      p_mesh->setIsStatic(isStatic);
    }
  }
}

void R2::Imgui::renderMeshTransform(Mesh *p_mesh)
{
  if (ImGui::CollapsingHeader("transform"))
  {
    float positionVec3[3] = {p_mesh->getPosition().x, p_mesh->getPosition().y, p_mesh->getPosition().z};
    float rotationVec3[3] = {p_mesh->getRotation().x, p_mesh->getRotation().y, p_mesh->getRotation().z};
    float scaleVec3[3] = {p_mesh->getScale().x, p_mesh->getScale().y, p_mesh->getScale().z};

    if (ImGui::DragFloat3("position", positionVec3, 0.1f, -1000.0f, 1000.0f, "%.1f"))
    {
      p_mesh->setPosition(glm::vec3(positionVec3[0], positionVec3[1], positionVec3[2]));
    }

    if (ImGui::DragFloat3("scale", scaleVec3, 0.1f, -1000.0f, 1000.0f, "%.1f"))
    {
      p_mesh->setScale(glm::vec3(scaleVec3[0], scaleVec3[1], scaleVec3[2]));
    }

    if (ImGui::DragFloat3("rotation", rotationVec3, 0.1f, -1000.0f, 1000.0f, "%.1f"))
    {
      p_mesh->setRotation(glm::vec3(rotationVec3[0], rotationVec3[1], rotationVec3[2]));
    }
  }
}

void R2::Imgui::renderCameraSettings(Camera *p_camera)
{
  if (ImGui::CollapsingHeader("camera"))
  {
    if (!p_camera->getIsActive())
    {
      if (ImGui::Button("set camera"))
      {
        std::vector<R2::Mesh *> meshes = m_papplication->getSceneManager()->getCurrentScene()->getMeshes();

        for (int i = 0; i < meshes.size(); i++)
        {
          if (meshes[i]->getIsCamera())
          {
            Camera *camera = static_cast<Camera *>(meshes[i]);
            camera->setIsActive(false);
          }
        }

        p_camera->setIsActive(true);
        m_papplication->getSceneManager()->getCurrentScene()->setCamera(p_camera);
      }
    }

    if (!p_camera->getIsOrthographic())
    {
      float fov = p_camera->getFov();
      if (ImGui::DragFloat("fov", &fov, 0.1f, 0.0f, 180.0f, "%.1f"))
      {
        p_camera->setFov(fov);
      }
    }

    float near = p_camera->getNear();
    if (ImGui::DragFloat("near", &near, 0.1f, 0.0f, 500.0f, "%.1f"))
    {
      p_camera->setNear(near);
    }

    float far = p_camera->getFar();
    if (ImGui::DragFloat("far", &far, 0.1f, 0.0f, 500.0f, "%.1f"))
    {
      p_camera->setFar(far);
    }

    bool isPerspective = p_camera->getIsPerspective();
    if (ImGui::Checkbox("perspective", &isPerspective))
    {
      p_camera->setIsPerspective(isPerspective);
      p_camera->setIsOrthographic(!isPerspective);
    }

    bool isOrthographic = p_camera->getIsOrthographic();
    if (ImGui::Checkbox("orthographic", &isOrthographic))
    {
      p_camera->setIsOrthographic(isOrthographic);
      p_camera->setIsPerspective(!isOrthographic);
    }

    if (p_camera->getIsOrthographic())
    {
      float zoom = p_camera->getZoom();
      if (ImGui::DragFloat("zoom", &zoom, 0.1f, 0.0f, 500.0f, "%.1f"))
      {
        p_camera->setZoom(zoom);
      }
    }

    float speed = p_camera->getSpeed();
    if (ImGui::DragFloat("speed", &speed, 0.1f, 0.0f, 500.0f, "%.1f"))
    {
      p_camera->setSpeed(speed);
    }

    float sensitivity = p_camera->getSensitivity();
    if (ImGui::DragFloat("sensitivity", &sensitivity, 0.01f, 0.0f, 500.0f, "%.01f"))
    {
      p_camera->setSensitivity(sensitivity);
    }

    ImGui::Begin("cam view");

    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_papplication->getRenderer()->render(p_camera, m_papplication->getSceneManager()->getCurrentScene());

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    ImGui::Image((void *)(intptr_t)m_textureColorbuffer, ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight()), ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
  }
}

void R2::Imgui::renderMeshTree(const std::vector<R2::Mesh *> &meshes)
{
  renderAddMesh();

  for (int i = 0; i < meshes.size(); i++)
  {
    Mesh *mesh = meshes[i];
    if (!mesh->getIsVisibleOnMenu())
    {
      continue;
    }

    ImGui::PushID(i);
    std::string meshName = mesh->getName();
    const char *label = meshName.c_str();

    if (mesh->getIsGroup())
    {
      MeshGroup *group = static_cast<MeshGroup *>(mesh);
      bool nodeOpen = ImGui::TreeNodeEx(label, ImGuiTreeNodeFlags_OpenOnArrow | (m_selectedMesh == mesh ? ImGuiTreeNodeFlags_Selected : 0));
      if (ImGui::IsItemClicked())
      {
        m_selectedMesh = (m_selectedMesh == mesh) ? nullptr : mesh;
      }

      if (nodeOpen)
      {
        renderMeshTree(group->getChildMeshes());
        ImGui::TreePop();
      }
    }
    else
    {
      if (ImGui::Selectable(label, m_selectedMesh == mesh))
      {
        m_selectedMesh = (m_selectedMesh == mesh) ? nullptr : mesh;
      }
    }

    ImGui::PopID();
  }
}

void R2::Imgui::renderLightSettings(Light *p_light)
{
  if (ImGui::CollapsingHeader("light"))
  {
    ImVec4 lightColor = ImVec4(p_light->getColor().r, p_light->getColor().g, p_light->getColor().b, p_light->getColor().a);
    if (ImGui::ColorEdit4("color", (float *)&lightColor))
    {
      p_light->setColor(glm::vec4(lightColor.x, lightColor.y, lightColor.z, lightColor.w));
    }

    float intensity = p_light->getIntensity();
    if (ImGui::DragFloat("intensity", &intensity, 0.01f, 0.0f, 50.0f))
    {
      p_light->setIntensity(intensity);
    }

    if (p_light->getIsPointLight())
    {
      if (ImGui::CollapsingHeader("point light"))
      {
        glm::vec3 ambient = p_light->getAmbient();
        if (ImGui::DragFloat("ambient", &ambient.x, 0.01f, 0.0f, 50.0f))
        {
          p_light->setAmbient(glm::vec3(ambient.x, ambient.x, ambient.x));
        }
        glm::vec3 diffuse = p_light->getDiffuse();
        if (ImGui::DragFloat("diffuse", &diffuse.x, 0.01f, 0.0f, 50.0f))
        {
          p_light->setDiffuse(glm::vec3(diffuse.x, diffuse.x, diffuse.x));
        }
        glm::vec3 specular = p_light->getSpecular();
        if (ImGui::DragFloat("specular", &specular.x, 0.01f, 0.0f, 50.0f))
        {
          p_light->setSpecular(glm::vec3(specular.x, specular.x, specular.x));
        }
        float constant = p_light->getConstant();
        if (ImGui::DragFloat("constant", &constant, 0.01f, 0.0f, 50.0f))
        {
          p_light->setConstant(constant);
        }
        float linear = p_light->getLinear();
        if (ImGui::DragFloat("linear", &linear, 0.001f, 0.0f, 50.0f))
        {
          p_light->setLinear(linear);
        }
        float quadratic = p_light->getQuadratic();
        if (ImGui::DragFloat("quadratic", &quadratic, 0.001f, 0.0f, 50.0f))
        {
          p_light->setQuadratic(quadratic);
        }
        float farPlane = p_light->getFarPlane();
        if (ImGui::DragFloat("far plane", &farPlane, 0.1f, 0.0f, 500.0f))
        {
          p_light->setFarPlane(farPlane);
        }
      }
    }
    else if (p_light->getIsDirectionalLight())
    {
      if (ImGui::CollapsingHeader("directional light"))
      {
        glm::vec3 ambient = p_light->getAmbient();
        if (ImGui::DragFloat("ambient", &ambient.x, 0.01f, 0.0f, 50.0f))
        {
          p_light->setAmbient(glm::vec3(ambient.x, ambient.x, ambient.x));
        }
        glm::vec3 diffuse = p_light->getDiffuse();
        if (ImGui::DragFloat("diffuse", &diffuse.x, 0.01f, 0.0f, 50.0f))
        {
          p_light->setDiffuse(glm::vec3(diffuse.x, diffuse.x, diffuse.x));
        }
        glm::vec3 specular = p_light->getSpecular();
        if (ImGui::DragFloat("specular", &specular.x, 0.01f, 0.0f, 50.0f))
        {
          p_light->setSpecular(glm::vec3(specular.x, specular.x, specular.x));
        }
        float direction[3] = {p_light->getDirection().x, p_light->getDirection().y, p_light->getDirection().z};
        if (ImGui::DragFloat3("direction", direction, 0.01f, -1000.0f, 1000.0f, "%.1f"))
        {
          p_light->setDirection(glm::vec3(direction[0], direction[1], direction[2]));
        }
        float nearPlane = p_light->getNearPlane();
        if (ImGui::DragFloat("near plane", &nearPlane, 0.1f, 0.0f, 500.0f))
        {
          p_light->setNearPlane(nearPlane);
        }
        float farPlane = p_light->getFarPlane();
        if (ImGui::DragFloat("far plane", &farPlane, 0.1f, 0.0f, 500.0f))
        {
          p_light->setFarPlane(farPlane);
        }
      }

      ImGui::Begin("shadow map");

      GLuint depthMapTexture = p_light->getDepthMap();

      ImVec2 imageSize(256, 256);

      ImGui::Image((void *)(intptr_t)depthMapTexture, imageSize, ImVec2(0, 1), ImVec2(1, 0), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.0f));

      ImGui::End();
    }
  }
}

void R2::Imgui::cleanup()
{
  std::cout << "Imgui::cleanup()" << std::endl;
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

R2::Mesh *R2::Imgui::getSelectedMesh()
{
  return m_selectedMesh;
}

void R2::Imgui::renderScene(Scene *p_scene)
{
  if (ImGui::CollapsingHeader("scene"))
  {
    bool isPlaying = p_scene->getIsPlaying();
    std::string playText = isPlaying ? "pause" : "play";
    if (ImGui::Button(playText.c_str()))
    {
      p_scene->setIsPlaying(!isPlaying);
    }

    if (ImGui::Button("reload scene"))
    {
      m_selectedMesh = nullptr;
      p_scene->reload();
    }

    if (ImGui::Button("export scene"))
    {
      //...
    }

    ImGui::Text("name: %s", p_scene->getName().c_str());

    float clearColor[3] = {p_scene->getClearColor().r, p_scene->getClearColor().g, p_scene->getClearColor().b};
    if (ImGui::ColorEdit3("clear color", clearColor))
    {
      p_scene->setClearColor(glm::vec4(clearColor[0], clearColor[1], clearColor[2], 1.0f));
    }

    bool isLightsActive = p_scene->getIsLightsActive();
    if (ImGui::Checkbox("lights", &isLightsActive))
    {
      p_scene->setIsLightsActive(isLightsActive);
    }

    if (ImGui::CollapsingHeader("physics"))
    {
      glm::vec3 gravityDirection = p_scene->getGravityDirection();
      float gravityDirectionVec3[3] = { gravityDirection.x, gravityDirection.y, gravityDirection.z };
      if (ImGui::DragFloat3("gravity direction", gravityDirectionVec3, 0.1f, -1000.0f, 1000.0f, "%.1f"))
      {
        p_scene->setGravityDirection(glm::vec3(gravityDirectionVec3[0], gravityDirectionVec3[1], gravityDirectionVec3[2]));
      }

      float gravity = p_scene->getGravityForce();
      if (ImGui::DragFloat("gravity force", &gravity, 0.1f, 0.0f, 50.0f))
      {
        p_scene->setGravityForce(gravity);
      }
    }
  }
}

void R2::Imgui::renderScenes(std::vector<Scene *> scenes)
{
  if (ImGui::CollapsingHeader("scenes"))
  {
    for (int i = 0; i < scenes.size(); i++)
    {
      Scene *scene = scenes[i];
      ImGui::PushID(i);
      std::string sceneName = scene->getName();
      const char *label = sceneName.c_str();
      bool isSelected = scene->getIsActiveScene();
      if (ImGui::Selectable(label, isSelected))
      {
        m_selectedMesh = nullptr;
        m_papplication->getSceneManager()->setCurrentScene(i);
      }
      ImGui::PopID();
    }
  }
}

void R2::Imgui::renderMeshTextures(Mesh *p_mesh)
{
  std::vector<Texture *> textures = p_mesh->getTextures();
  std::string texturesTitle = "textures (" + std::to_string(textures.size()) + ")";

  if (ImGui::CollapsingHeader(texturesTitle.c_str()))
  {
    for (Texture *texture : textures)
    {
      if (texture->getIsDiffuse())
      {
        ImGui::SeparatorText("diffuse");
      }
      else if (texture->getIsSpecular())
      {
        ImGui::SeparatorText("specular");
      }
      else if (texture->getIsNormal())
      {
        ImGui::SeparatorText("normal");
      }
      else
      {
        ImGui::SeparatorText("unknown");
      }
      ImGui::Image((void *)(intptr_t)texture->getId(), ImVec2(64, 64));
      ImGui::SameLine();
      ImGui::Text(texture->getImagePath().data());
    }
  }
}

void R2::Imgui::renderAddMesh()
{
  /*
  if (ImGui::Button("+ add"))
    ImGui::OpenPopup("add");

  if (ImGui::BeginPopup("add", ImGuiWindowFlags_None))
  {
    if (ImGui::Button("point light"))
    {
      ImGui::OpenPopup("add point light");
    }

    if(ImGui::Button("directional light"))
      ImGui::OpenPopup("add directional light");

    if(ImGui::Button("mesh"))
      ImGui::OpenPopup("add mesh");

    ImGui::EndPopup();
  }

  //ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  //ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

  if (ImGui::BeginPopupModal("add point light", NULL, ImGuiWindowFlags_AlwaysAutoResize))
  {
    ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!");

    if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
    ImGui::SetItemDefaultFocus();
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
    ImGui::EndPopup();
  }*/
}

void R2::Imgui::setupTheme()
{
  ImGuiStyle& style = ImGui::GetStyle();

  style.Alpha = 1.0f;
  style.DisabledAlpha = 0.6000000238418579f;
  style.WindowPadding = ImVec2(8.0f, 8.0f);
  style.WindowRounding = 0.0f;
  style.WindowBorderSize = 1.0f;
  style.WindowMinSize = ImVec2(32.0f, 32.0f);
  style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
  style.WindowMenuButtonPosition = ImGuiDir_Left;
  style.ChildRounding = 0.0f;
  style.ChildBorderSize = 1.0f;
  style.PopupRounding = 0.0f;
  style.PopupBorderSize = 1.0f;
  style.FramePadding = ImVec2(4.0f, 3.0f);
  style.FrameRounding = 0.0f;
  style.FrameBorderSize = 0.0f;
  style.ItemSpacing = ImVec2(8.0f, 4.0f);
  style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
  style.CellPadding = ImVec2(4.0f, 2.0f);
  style.IndentSpacing = 21.0f;
  style.ColumnsMinSpacing = 6.0f;
  style.ScrollbarSize = 14.0f;
  style.ScrollbarRounding = 0.0f;
  style.GrabMinSize = 10.0f;
  style.GrabRounding = 0.0f;
  style.TabRounding = 0.0f;
  style.TabBorderSize = 0.0f;
  style.TabMinWidthForCloseButton = 0.0f;
  style.ColorButtonPosition = ImGuiDir_Right;
  style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
  style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

  style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
  style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.4980392158031464f, 0.4980392158031464f, 0.4980392158031464f, 1.0f);
  style.Colors[ImGuiCol_WindowBg] = ImVec4(0.03921568766236305f, 0.03921568766236305f, 0.03921568766236305f, 1.0f);
  style.Colors[ImGuiCol_ChildBg] = ImVec4(0.05490196123719215f, 0.05490196123719215f, 0.05490196123719215f, 1.0f);
  style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.8583691120147705f);
  style.Colors[ImGuiCol_Border] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
  style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.6995707750320435f);
  style.Colors[ImGuiCol_FrameBg] = ImVec4(0.05490196123719215f, 0.05490196123719215f, 0.05490196123719215f, 1.0f);
  style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.06666667014360428f, 0.06666667014360428f, 0.06666667014360428f, 1.0f);
  style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.05490196123719215f, 0.05490196123719215f, 0.05490196123719215f, 0.05490196123719215f);
  style.Colors[ImGuiCol_TitleBg] = ImVec4(0.1843137294054031f, 0.1921568661928177f, 0.2117647081613541f, 1.0f);
  style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.03921568766236305f, 0.03921568766236305f, 0.03921568766236305f, 1.0f);
  style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.05490196123719215f, 0.05490196123719215f, 0.05490196123719215f, 1.0f);
  style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.9399999976158142f);
  style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.01960784383118153f, 0.01960784383118153f, 0.01960784383118153f, 0.5299999713897705f);
  style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3098039329051971f, 1.0f);
  style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.407843142747879f, 0.407843142747879f, 0.407843142747879f, 1.0f);
  style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.5098039507865906f, 0.5098039507865906f, 0.5098039507865906f, 1.0f);
  style.Colors[ImGuiCol_CheckMark] = ImVec4(0.6470588445663452f, 0.2313725501298904f, 0.2313725501298904f, 1.0f);
  style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
  style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
  style.Colors[ImGuiCol_Button] = ImVec4(0.05490196123719215f, 0.05490196123719215f, 0.05490196123719215f, 1.0f);
  style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.06666667014360428f, 0.06666667014360428f, 0.06666667014360428f, 1.0f);
  style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 1.0f);
  style.Colors[ImGuiCol_Header] = ImVec4(0.05490196123719215f, 0.05490196123719215f, 0.05490196123719215f, 1.0f);
  style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.06666667014360428f, 0.06666667014360428f, 0.06666667014360428f, 1.0f);
  style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 1.0f);
  style.Colors[ImGuiCol_Separator] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.501960813999176f);
  style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.6695278882980347f);
  style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.9570815563201904f);
  style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.1019607856869698f, 0.1137254908680916f, 0.1294117718935013f, 0.2000000029802322f);
  style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.2039215713739395f, 0.2078431397676468f, 0.2156862765550613f, 0.2000000029802322f);
  style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.3019607961177826f, 0.3019607961177826f, 0.3019607961177826f, 0.2000000029802322f);
  style.Colors[ImGuiCol_Tab] = ImVec4(0.1843137294054031f, 0.1921568661928177f, 0.2117647081613541f, 1.0f);
  style.Colors[ImGuiCol_TabHovered] = ImVec4(0.2352941185235977f, 0.2470588237047195f, 0.2705882489681244f, 1.0f);
  style.Colors[ImGuiCol_TabActive] = ImVec4(0.2588235437870026f, 0.2745098173618317f, 0.3019607961177826f, 1.0f);
  style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.06666667014360428f, 0.06666667014360428f, 0.06666667014360428f, 0.9725490212440491f);
  style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.06666667014360428f, 0.06666667014360428f, 0.06666667014360428f, 1.0f);
  style.Colors[ImGuiCol_PlotLines] = ImVec4(0.6078431606292725f, 0.6078431606292725f, 0.6078431606292725f, 1.0f);
  style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.9490196108818054f, 0.3450980484485626f, 0.3450980484485626f, 1.0f);
  style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.9490196108818054f, 0.3450980484485626f, 0.3450980484485626f, 1.0f);
  style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.4274509847164154f, 0.3607843220233917f, 0.3607843220233917f, 1.0f);
  style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
  style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
  style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
  style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
  style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
  style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.0f, 0.8784313797950745f, 0.8784313797950745f, 1.0f);
  style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.2588235437870026f, 0.2705882489681244f, 0.3803921639919281f, 1.0f);
  style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.1803921610116959f, 0.2274509817361832f, 0.2784313857555389f, 1.0f);
  style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
  style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
  style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);
}