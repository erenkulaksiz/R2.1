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

glm::vec2 R2::Utils::screenToNDC(const glm::vec2& screenPos, float screenWidth, float screenHeight) 
{
  return glm::vec2((2.0f * screenPos.x) / screenWidth - 1.0f, 1.0f - (2.0f * screenPos.y) / screenHeight);
}

glm::vec4 R2::Utils::ndcToClip(const glm::vec2& ndcPos) 
{
  return glm::vec4(ndcPos.x, ndcPos.y, -1.0f, 1.0f);
}

glm::vec4 R2::Utils::clipToView(const glm::vec4& clipPos, const glm::mat4& inverseProjection) 
{
  return inverseProjection * clipPos;
}

glm::vec4 R2::Utils::viewToWorld(const glm::vec4& viewPos, const glm::mat4& inverseView) 
{
  return inverseView * viewPos;
}

bool R2::Utils::rayIntersectsAABB(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const glm::vec3& minBound, const glm::vec3& maxBound, float& tMin, float& tMax) 
{
  glm::vec3 invDir = 1.0f / rayDirection;
  glm::vec3 t0s = (minBound - rayOrigin) * invDir;
  glm::vec3 t1s = (maxBound - rayOrigin) * invDir;

  glm::vec3 t0 = glm::min(t0s, t1s);
  glm::vec3 t1 = glm::max(t0s, t1s);

  tMin = glm::max(glm::max(t0.x, t0.y), t0.z);
  tMax = glm::min(glm::min(t1.x, t1.y), t1.z);

  return tMax >= tMin && tMax > 0;
}

glm::vec2 R2::Utils::worldToScreen(const glm::vec3& worldPos, const glm::mat4& viewMatrix, const glm::mat4& projMatrix, int screenWidth, int screenHeight)
{
  glm::vec4 clipSpacePos = projMatrix * viewMatrix * glm::vec4(worldPos, 1.0f);
  glm::vec3 ndcSpacePos = glm::vec3(clipSpacePos) / clipSpacePos.w;

  glm::vec2 screenPos;
  screenPos.x = (ndcSpacePos.x + 1.0f) / 2.0f * screenWidth;
  screenPos.y = (1.0f - ndcSpacePos.y) / 2.0f * screenHeight;
  return screenPos;
}