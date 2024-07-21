#pragma once
#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <string>
#include <glm/glm.hpp>

namespace R2
{
  class Utils
  {
  public:
    Utils();
    std::string getFilePath(std::string path);
    std::string getCurrentPath();
    glm::vec3 stringToVec3(std::string vec3String);
    glm::vec4 stringToVec4(std::string vec4String);
    float stringToFloat(std::string floatString);
    bool stringToBool(std::string boolString);
    glm::vec2 screenToNDC(const glm::vec2& screenPos, float screenWidth, float screenHeight);
    glm::vec4 ndcToClip(const glm::vec2& ndcPos);
    glm::vec4 clipToView(const glm::vec4& clipPos, const glm::mat4& inverseProjection);
    glm::vec4 viewToWorld(const glm::vec4& viewPos, const glm::mat4& inverseView);
    bool rayIntersectsAABB(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const glm::vec3& minBound, const glm::vec3& maxBound, float& tMin, float& tMax);
    glm::vec2 worldToScreen(const glm::vec3& worldPos, const glm::mat4& viewMatrix, const glm::mat4& projMatrix, int screenWidth, int screenHeight);

  private:
    std::string m_executableFolder = "";
  };
}

#endif