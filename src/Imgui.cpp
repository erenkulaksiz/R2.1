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
  newFrame();
  drawGui();
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

  // ImGui::ShowMetricsWindow();

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

        bool isDrawingBoundingBox = p_mesh->getIsDrawingBoundingBox();
        if (ImGui::Checkbox("bounding box", &isDrawingBoundingBox))
        {
          p_mesh->setIsDrawingBoundingBox(isDrawingBoundingBox);
        }
      }
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
            camera->setActive(false);
          }
        }

        p_camera->setActive(true);
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
    if (ImGui::Button("reload scene"))
    {
      m_selectedMesh = nullptr;
      p_scene->reload();
    }
    ImGui::Text("name: %s", p_scene->getName().c_str());
    // ImGui::Text("mesh count: %d", p_scene->getMeshes().size()); takes account for invisible meshes + needs to take care of meshgroup
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
        ImGui::Text("diffuse");
      }
      ImGui::Image((void *)(intptr_t)texture->getId(), ImVec2(64, 64));
      ImGui::SameLine();
      ImGui::Text(texture->getImagePath().data());
    }
  }
}