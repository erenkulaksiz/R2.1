#include <iostream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <R2/shader/Shader.h>
#include <R2/Utils.h>

bool checkShaderCompilation(GLuint shader)
{
  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    GLchar infoLog[512];
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cout << "Shader Compilation Error: " << infoLog << std::endl;
    return false;
  }
  return true;
}

bool checkProgramLinking(GLuint program)
{
  GLint success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success)
  {
    GLchar infoLog[512];
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    std::cout << "Program Linking Error: " << infoLog << std::endl;
    return false;
  }
  return true;
}

R2::Shader::Shader(std::string vertexPath, std::string fragmentPath)
{
  std::cout << "Shader::Shader()" << std::endl;
  std::cout << "vertexPath: " << vertexPath << std::endl;
  std::cout << "fragmentPath: " << fragmentPath << std::endl;
  this->m_vertexPath = vertexPath;
  this->m_fragmentPath = fragmentPath;
}

R2::Shader::Shader(std::string vertexPath, std::string fragmentPath, std::string geometryPath)
{
  std::cout << "Shader::Shader()" << std::endl;
  std::cout << "vertexPath: " << vertexPath << std::endl;
  std::cout << "fragmentPath: " << fragmentPath << std::endl;
  std::cout << "geometryPath: " << geometryPath << std::endl;
  this->m_vertexPath = vertexPath;
  this->m_fragmentPath = fragmentPath;
  this->m_geometryPath = geometryPath;
}

R2::Shader::~Shader()
{
  std::cout << "Shader::~Shader() " << m_ID << std::endl;
  cleanup();
}

void R2::Shader::setup(Application *p_application)
{
  std::cout << "Shader::setup()" << m_vertexPath << std::endl;
  std::string vertexCode = p_application->getUtils()->getFileContents(m_vertexPath.data());
  std::string fragmentCode = p_application->getUtils()->getFileContents(m_fragmentPath.data());

  const char *vertexSource = vertexCode.data();
  const char *fragmentSource = fragmentCode.data();

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  std::cout << "Shader::setup() vertexshader " << vertexShader << std::endl;
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);
  if (!checkShaderCompilation(vertexShader))
  {
    std::cout << "Shader Compilation Error vertex" << std::endl;
  }

  GLuint geometryShader;
  if (m_geometryPath != "")
  {
    std::string geometryCode = p_application->getUtils()->getFileContents(m_geometryPath.data());
    const char *geometrySource = geometryCode.data();
    geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    std::cout << "Shader::setup() geometryshader " << geometryShader << std::endl;
    glShaderSource(geometryShader, 1, &geometrySource, NULL);
    glCompileShader(geometryShader);
    if (!checkShaderCompilation(geometryShader))
    {
      std::cout << "Shader Compilation Error geometry" << std::endl;
    }
  }

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  std::cout << "Shader::setup() fragmentshader " << fragmentShader << std::endl;
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);
  if (!checkShaderCompilation(fragmentShader))
  {
    std::cout << "Shader Compilation Error fragment" << std::endl;
  }

  m_ID = glCreateProgram();
  glAttachShader(m_ID, vertexShader);
  if (m_geometryPath != "")
  {
    glAttachShader(m_ID, geometryShader);
  }
  glAttachShader(m_ID, fragmentShader);
  glLinkProgram(m_ID);

  glDeleteShader(vertexShader);
  if (m_geometryPath != "")
  {
    glDeleteShader(geometryShader);
  }
  glDeleteShader(fragmentShader);
}

void R2::Shader::setVec3(std::string name, glm::vec3 value)
{
  glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(value));
}

void R2::Shader::setVec4(std::string name, glm::vec4 value)
{
  glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(value));
}

void R2::Shader::setMat4(std::string name, glm::mat4 value)
{
  glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void R2::Shader::setBool(std::string name, bool value)
{
  glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void R2::Shader::setFloat(std::string name, float value)
{
  glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

void R2::Shader::setInt(std::string name, int value)
{
  glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void R2::Shader::activate()
{
  glUseProgram(m_ID);
}

void R2::Shader::deactivate()
{
  glUseProgram(0);
}

GLuint R2::Shader::getId()
{
  return m_ID;
}

void R2::Shader::cleanup()
{
  std::cout << "Shader::cleanup()" << m_ID << std::endl;
  glDeleteProgram(m_ID);
}