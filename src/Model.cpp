#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <R2/Model.h>
#include <R2/Mesh.h>
#include <R2/Texture.h>

R2::Model::Model(Application *p_application)
{
  std::cout << "Model::Model()" << std::endl;
  m_papplication = p_application;
}

std::vector<R2::Mesh *> R2::Model::loadFromFile(std::string fileName, Shader *p_shader)
{
  std::cout << "Model::loadFromFile()" << std::endl;

  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(fileName, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate);

  std::vector<R2::Mesh *> meshes;

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
  {
    std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
    return meshes;
  }

  for (unsigned int i = 0; i < scene->mNumMeshes; i++)
  {
    aiMesh *mesh = scene->mMeshes[i];
    R2::Mesh *model = new R2::Mesh();
    if (mesh->mName.C_Str())
    {
      model->setName(std::string(mesh->mName.C_Str()));
    }
    else
    {
      model->setName("Mesh");
    }
    model->setShader(p_shader);
    model->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    model->setRotation(glm::vec3(0.0f));
    model->setScale(glm::vec3(1.0f));
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    vertices.reserve(mesh->mNumVertices * 8);
    indices.reserve(mesh->mNumFaces * 3);

    for (int j = 0; j < scene->mNumMaterials; j++)
    {
      aiMaterial *material = scene->mMaterials[j];
      aiString texturePath;

      if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
      {
        std::string textureFilePath = m_papplication->getUtils()->getCurrentPath() + "/resources/" + std::string(texturePath.C_Str());

        std::cout << "diffuse texture path: " << textureFilePath << std::endl;

        R2::Texture *texture = new R2::Texture(textureFilePath, GL_TEXTURE_2D, GL_TEXTURE0, GL_UNSIGNED_BYTE);
        texture->setup();
        texture->texUnit(p_shader, "material.diffuse", 0);
        texture->setShininess(1.0f);
        texture->setIsDiffuse(true);
        model->addTexture(texture);
      }
    }

    for (unsigned int j = 0; j < mesh->mNumVertices; j++)
    {
      vertices.push_back(mesh->mVertices[j].x);
      vertices.push_back(mesh->mVertices[j].y);
      vertices.push_back(mesh->mVertices[j].z);

      if (mesh->HasTextureCoords(0))
      {
        vertices.push_back(mesh->mTextureCoords[0][j].x);
        vertices.push_back(mesh->mTextureCoords[0][j].y);
      }
      else
      {
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
      }

      vertices.push_back(mesh->mNormals[j].x);
      vertices.push_back(mesh->mNormals[j].y);
      vertices.push_back(mesh->mNormals[j].z);
    }

    model->setVertices(vertices.data(), sizeof(vertices[0]) * vertices.size());

    for (unsigned int j = 0; j < mesh->mNumFaces; j++)
    {
      if (mesh->mFaces[j].mNumIndices < 3)
      {
        continue;
      }
      for (unsigned int k = 0; k < mesh->mFaces[j].mNumIndices; k++)
      {
        indices.push_back(mesh->mFaces[j].mIndices[k]);
      }
    }

    model->setIndices(indices.data(), sizeof(indices[0]) * indices.size());
    model->setup();
    meshes.push_back(model);
  }

  for (unsigned int i = 0; i < meshes.size(); i++)
  {
    if (meshes[i]->getTextures().size() == 0)
    {
      R2::Texture *texture = new R2::Texture(m_papplication->getUtils()->getFilePath("/resources/default.png"), GL_TEXTURE_2D, GL_TEXTURE0, GL_UNSIGNED_BYTE);
      texture->setup();
      texture->texUnit(p_shader, "material.diffuse", 0);
      texture->setShininess(1.0f);
      meshes[i]->addTexture(texture);
    }
  }

  return meshes;
}