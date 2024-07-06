#include <iostream>
#include <R2/Billboard.h>

GLfloat billboardVertices[] =
    {
        //     COORDINATES     / TexCoord  /   normals
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Lower left corner
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // Upper left corner
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,   // Upper right corner
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // Lower right corner
};

GLuint billboardIndices[] =
    {
        0, 2, 1,
        0, 3, 2};

R2::Billboard::Billboard(Application *p_application, std::string imagePath) : Mesh()
{
  std::cout << "R2::Billboard::Billboard()" << std::endl;
  m_papplication = p_application;
  m_isBillboard = true;

  setVertices(billboardVertices, sizeof(billboardVertices));
  setIndices(billboardIndices, sizeof(billboardIndices));

  Shader *p_shader = new Shader(m_papplication->getUtils()->getFilePath("/shaders/billboard/billboard.vert"), m_papplication->getUtils()->getFilePath("/shaders/billboard/billboard.frag"));
  p_shader->setup(m_papplication);
  setShader(p_shader);

  Texture *p_texture = new Texture(imagePath, GL_TEXTURE_2D, GL_TEXTURE0, GL_UNSIGNED_BYTE);
  p_texture->setup();
  p_texture->setIsDiffuse(true);
  p_texture->texUnit(p_shader, "tex0", 0);
  addTexture(p_texture);
}