#include <iostream>
#include <R2/Renderer.h>
#include <R2/Light.h>

R2::Renderer::Renderer(Application *p_application)
{
  std::cout << "Renderer::Renderer()" << std::endl;
  m_papplication = p_application;

  m_pdirectionalDepthShader = new Shader(m_papplication->getUtils()->getFilePath("/shaders/shadow/shadow.vert"), m_papplication->getUtils()->getFilePath("/shaders/shadow/shadow.frag"));
  m_pdirectionalDepthShader->setup(m_papplication);

  m_ppointDepthShader = new Shader(m_papplication->getUtils()->getFilePath("/shaders/pointShadow/pointShadow.vert"), m_papplication->getUtils()->getFilePath("/shaders/pointShadow/pointShadow.frag"), m_papplication->getUtils()->getFilePath("/shaders/pointShadow/pointShadow.geom"));
  m_ppointDepthShader->setup(m_papplication);

  m_pboundingBoxShader = new Shader(m_papplication->getUtils()->getFilePath("/shaders/line/line.vert"), m_papplication->getUtils()->getFilePath("/shaders/line/line.frag"));
  m_pboundingBoxShader->setup(m_papplication);

  m_defaultObjectShader = new Shader(m_papplication->getUtils()->getFilePath("/shaders/default/default.vert"), m_papplication->getUtils()->getFilePath("/shaders/default/default.frag"));
  m_defaultObjectShader->setup(m_papplication);
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

    mesh->updatePhysics(m_papplication->getDeltaTime(), m_papplication->getSceneManager()->getCurrentScene()->getGravityDirection(), p_meshes);
    mesh->render(p_camera, p_scene);
  }
}

void R2::Renderer::renderDirectionalShadowMap(Shader *p_shader, Scene *p_scene, glm::mat4 lightSpaceMatrix)
{
  std::vector<Mesh *> p_meshes = p_scene->getMeshes();

  for (Mesh *mesh : p_meshes)
  {
    mesh->renderDirectionalShadowMap(p_shader, lightSpaceMatrix);
  }
}

void R2::Renderer::renderPointShadowMap(Shader *p_shader, Scene *p_scene, std::vector<glm::mat4> lightSpaceMatrix, glm::vec3 lightPos, float farPlane)
{
  std::vector<Mesh *> p_meshes = p_scene->getMeshes();

  for (Mesh *mesh : p_meshes)
  {
    mesh->renderPointShadowMap(p_shader, lightSpaceMatrix, lightPos, farPlane);
  }
}

R2::Shader *R2::Renderer::getDirectionalDepthShader()
{
  return m_pdirectionalDepthShader;
}

R2::Shader *R2::Renderer::getPointDepthShader()
{
  return m_ppointDepthShader;
}

R2::Shader *R2::Renderer::getBoundingBoxShader()
{
  return m_pboundingBoxShader;
}

R2::Shader *R2::Renderer::getDefaultObjectShader()
{
  return m_defaultObjectShader;
}