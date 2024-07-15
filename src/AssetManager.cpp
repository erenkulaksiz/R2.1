#include <stb/stb_image.h>
#include <R2/AssetManager.h>

R2::AssetManager::AssetManager(Application* p_application)
{
  m_papplication = p_application;
}

unsigned char* R2::AssetManager::loadTexture(std::string imagePath, int& widthImgOut, int& heightImgOut, int& numColChOut)
{
  std::cout << "AssetManager::loadTexture() " << imagePath << std::endl;
  stbi_set_flip_vertically_on_load(true);
  unsigned char* bytes = stbi_load(imagePath.data(), &widthImgOut, &heightImgOut, &numColChOut, 0);

  return bytes;
}

void R2::AssetManager::textureFreeBytes(unsigned char* p_bytes)
{
  stbi_image_free(p_bytes);
}

std::pair<Assimp::Importer*, const aiScene*> R2::AssetManager::loadModel(std::string fileName, Shader* p_shader)
{
  std::cout << "AssetManager::loadModel() " << fileName << std::endl;

  Assimp::Importer* importer = new Assimp::Importer();
  const aiScene* scene = importer->ReadFile(fileName, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate);

  if (!scene)
  {
    std::cerr << "ERROR::ASSIMP::Failed to load object from file: " << fileName << std::endl;
    delete importer;
    return std::make_pair(nullptr, nullptr);
  }

  return std::make_pair(importer, scene);
}

std::string R2::AssetManager::getFileContents(std::string fileName)
{
  std::FILE* fp = std::fopen(fileName.data(), "rb");
  std::cout << "getFileContents() filename: " << fileName << std::endl;

  if (fp)
  {
    std::cout << "getFileContents() Reading file: " << fileName << std::endl;
    std::string contents;
    std::fseek(fp, 0, SEEK_END);
    contents.resize(std::ftell(fp));
    std::rewind(fp);
    std::fread(&contents[0], 1, contents.size(), fp);
    std::fclose(fp);
    return (contents);
  }

  std::cout << "getFileContents() Failed to read file: " << fileName << std::endl;
  return "";
}