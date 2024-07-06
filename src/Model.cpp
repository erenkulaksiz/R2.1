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
  const aiScene* scene = importer.ReadFile(fileName, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate);

  if (!scene)
  {
    std::cerr << "ERROR::ASSIMP::Failed to load object from file: " << fileName << std::endl;
    return std::vector<R2::Mesh*>();
  }

  std::cout << scene->mNumMeshes << " meshes found." << std::endl;

  std::vector<R2::Mesh *> meshes;

  for (unsigned int i = 0; i < scene->mNumMeshes; i++)
  {
    aiMesh *mesh = scene->mMeshes[i];
    R2::Mesh *model = new R2::Mesh();
    if (mesh->mName.C_Str())
    {
      model->setName(mesh->mName.C_Str());
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

      if (material->GetTexture(aiTextureType_HEIGHT, 0, &texturePath) == AI_SUCCESS)
      {
        std::string textureFilePath = m_papplication->getUtils()->getCurrentPath() + "/resources/" + std::string(texturePath.C_Str());

        std::cout << "normal texture path: " << textureFilePath << std::endl;

        R2::Texture* texture = new R2::Texture(textureFilePath, GL_TEXTURE_2D, GL_TEXTURE1, GL_UNSIGNED_BYTE);
        texture->setup();
        texture->texUnit(p_shader, "material.normal", 1);
        texture->setShininess(1.0f);
        texture->setIsNormal(true);
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

    glm::vec3 min = glm::vec3(1000000.0f);
    glm::vec3 max = glm::vec3(-1000000.0f);

    for (unsigned int j = 0; j < mesh->mNumVertices; j++)
    {
      if (mesh->mVertices[j].x < min.x)
      {
        min.x = mesh->mVertices[j].x;
      }
      if (mesh->mVertices[j].y < min.y)
      {
        min.y = mesh->mVertices[j].y;
      }
      if (mesh->mVertices[j].z < min.z)
      {
        min.z = mesh->mVertices[j].z;
      }

      if (mesh->mVertices[j].x > max.x)
      {
        max.x = mesh->mVertices[j].x;
      }
      if (mesh->mVertices[j].y > max.y)
      {
        max.y = mesh->mVertices[j].y;
      }
      if (mesh->mVertices[j].z > max.z)
      {
        max.z = mesh->mVertices[j].z;
      }
    }

    model->setBoundingBox(min, max);
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
      texture->setIsDiffuse(true);
      texture->setShininess(1.0f);
      meshes[i]->addTexture(texture);
    }
  }

  return meshes;
}