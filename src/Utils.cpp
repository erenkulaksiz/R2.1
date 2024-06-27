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
