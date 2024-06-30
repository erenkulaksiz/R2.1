#include <R2/Utils.h>
#include <wai/whereami.h>

R2::Utils::Utils()
{
  std::cout << "Utils::Utils()" << std::endl;
}

std::string R2::Utils::getCurrentPath()
{
  char *path = NULL;
  int length, dirname_length;
  int i;

  length = wai_getExecutablePath(NULL, 0, &dirname_length);
  if (length > 0)
  {
    path = (char *)malloc(length + 1);
    if (!path)
      abort();
    wai_getExecutablePath(path, length, &dirname_length);
    path[length] = '\0';
    path[dirname_length] = '\0';
    printf("  dirname: %s\n", path);
    m_executableFolder = std::string(path);
    free(path);
  }

  return m_executableFolder;
}

std::string R2::Utils::getFilePath(std::string path)
{
  if (m_executableFolder == "")
  {
    getCurrentPath();
  }

  return (m_executableFolder + path);
}

std::string R2::Utils::getFileContents(std::string filename)
{
  std::FILE *fp = std::fopen(filename.data(), "rb");
  std::cout << "getFileContents() filename: " << filename << std::endl;
  if (fp)
  {
    std::cout << "getFileContents() Reading file: " << filename << std::endl;
    std::string contents;
    std::fseek(fp, 0, SEEK_END);
    contents.resize(std::ftell(fp));
    std::rewind(fp);
    std::fread(&contents[0], 1, contents.size(), fp);
    std::fclose(fp);
    return (contents);
  }

  std::cout << "getFileContents() Failed to read file: " << filename << std::endl;
  return "";
}

glm::vec3 R2::Utils::stringToVec3(std::string vec3String)
{
  if(vec3String == "")
  {
    return glm::vec3(0.0f, 0.0f, 0.0f);
  }

  glm::vec3 vec3;
  std::string delimiter = ",";
  size_t pos = 0;
  std::string token;
  int i = 0;

  while ((pos = vec3String.find(delimiter)) != std::string::npos)
  {
    token = vec3String.substr(0, pos);
    vec3[i] = std::stof(token);
    vec3String.erase(0, pos + delimiter.length());
    i++;
  }

  vec3[i] = std::stof(vec3String);

  return vec3;
}

glm::vec4 R2::Utils::stringToVec4(std::string vec4String)
{
  if(vec4String == "")
  {
    return glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
  }

  glm::vec4 vec4;
  std::string delimiter = ",";
  size_t pos = 0;
  std::string token;
  int i = 0;

  while ((pos = vec4String.find(delimiter)) != std::string::npos)
  {
    token = vec4String.substr(0, pos);
    vec4[i] = std::stof(token);
    vec4String.erase(0, pos + delimiter.length());
    i++;
  }

  vec4[i] = std::stof(vec4String);

  return vec4;
}

float R2::Utils::stringToFloat(std::string floatString)
{
  return std::stof(floatString);
}

bool R2::Utils::stringToBool(std::string boolString)
{
  return (boolString == "true");
}