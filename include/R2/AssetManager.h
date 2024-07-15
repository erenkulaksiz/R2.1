#pragma once
#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H
#include <R2/Application.h>
#include <R2/shader/Shader.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace R2
{
  class Application;
  class Shader;

  class AssetManager
  {
  public:
    AssetManager(Application* p_application);
    unsigned char* loadTexture(std::string p_imagePath, int& widthImgOut, int& heightImgOut, int& numColChOut);
    void textureFreeBytes(unsigned char* p_bytes);
    std::pair<Assimp::Importer*, const aiScene*> loadModel(std::string p_fileName, Shader* p_shader);
    std::string getFileContents(std::string p_fileName);

  private:
    Application* m_papplication;
  };
}

#endif