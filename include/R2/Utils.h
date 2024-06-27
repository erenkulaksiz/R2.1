#pragma once
#ifndef UTILS_H
#define UTILS_H
#include <iostream>

namespace R2
{
  class Utils
  {
  public:
    Utils();
    std::string getFilePath(std::string path);
    std::string getCurrentPath();
    std::string getFileContents(std::string filename);

  private:
    std::string m_executableFolder = "";
  };
}

#endif