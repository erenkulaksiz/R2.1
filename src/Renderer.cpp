#include <iostream>
#include <R2/Renderer.h>
#include <R2/Light.h>

R2::Renderer::Renderer(Application *p_application)
{
  std::cout << "Renderer::Renderer()" << std::endl;
  m_papplication = p_application;
}

void R2::Renderer::render(Camera *p_camera, Scene *p_scene)
{
  std::vector<Mesh *> p_meshes = p_scene->getMeshes();

  for (Mesh *mesh : p_meshes)
  {
    if (mesh->getIsCamera())
    {
      Camera *camera = static_cast<Camera *>(mesh);
      camera->loop();
      continue;
    }

    mesh->updatePhysics(m_papplication->getDeltaTime(), m_papplication->getSceneManager()->getCurrentScene()->getGravityDirection());
    mesh->render(p_camera, p_scene);
  }
}

void R2::Renderer::render(Camera *p_camera, Scene *p_scene, Shader *p_shader)
{
  std::vector<Mesh *> p_meshes = p_scene->getMeshes();

  for (Mesh *mesh : p_meshes)
  {
    if (mesh->getIsCamera())
    {
      Camera *camera = static_cast<Camera *>(mesh);
      camera->loop();
      continue;
    }

    mesh->updatePhysics(m_papplication->getDeltaTime(), m_papplication->getSceneManager()->getCurrentScene()->getGravityDirection());
    mesh->render(p_camera, p_scene, p_shader);
  }
}

void R2::Renderer::renderShadowMap(Shader *p_shader, Scene *p_scene, glm::mat4 lightSpaceMatrix)
{
  std::vector<Mesh *> p_meshes = p_scene->getMeshes();

  for (Mesh *mesh : p_meshes)
  {
    mesh->renderShadowMap(p_shader, lightSpaceMatrix);
  }
}

void R2::Renderer::renderShadowMap(Shader *p_shader, Scene *p_scene, std::vector<glm::mat4> lightSpaceMatrix, glm::vec3 lightPos, float farPlane)
{
  std::vector<Mesh *> p_meshes = p_scene->getMeshes();

  for (Mesh *mesh : p_meshes)
  {
    mesh->renderShadowMap(p_shader, lightSpaceMatrix, lightPos, farPlane);
  }
}