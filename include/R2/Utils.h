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
    std::string getFileContents(std::string filename);
    glm::vec3 stringToVec3(std::string vec3String);
    glm::vec4 stringToVec4(std::string vec4String);
    float stringToFloat(std::string floatString);

  private:
    std::string m_executableFolder = "";
  };
}

#endif