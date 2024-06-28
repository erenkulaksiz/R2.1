#include <iostream>
#include <R2/Application.h>
#include <R2/Imgui.h>
#include <R2/Utils.h>
#include <R2/SceneManager.h>
#include <R2/Renderer.h>
#include <R2/Camera.h>
#include <R2/scenes/TestScene.h>
#include <R2/scenes/TestScene2.h>

R2::Application::Application()
{
  std::cout << "Application::Application()" << std::endl;
}

bool R2::Application::setup()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  glfwWindowHint(GLFW_SAMPLES, 4);

  int count = 1;
  GLFWmonitor **p_monitors = glfwGetMonitors(&count);
  const GLFWvidmode *p_videoMode = glfwGetVideoMode(p_monitors[0]);

  m_screenHeight = p_videoMode->height / 2;
  m_screenWidth = p_videoMode->width / 2;

  m_pwindow = glfwCreateWindow(m_screenWidth, m_screenHeight, p_mtitle, NULL, NULL);

  if (m_pwindow == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(m_pwindow);
  glfwShowWindow(m_pwindow);

  if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
  {
	std::cout << "Failed to initialize GLAD" << std::endl;
	return false;
  }

  glfwSetWindowUserPointer(m_pwindow, this);
  glfwSetInputMode(m_pwindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_MULTISAMPLE);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  glEnable(GL_TEXTURE_CUBE_MAP);
  glEnable(GL_FRAMEBUFFER_SRGB);

  m_putils = new Utils();
  m_prenderer = new Renderer(this);
  m_pimgui = new Imgui(this);
  m_pinput = new Input(this);
  m_pconfig = new Config(this);
  m_psceneManager = new SceneManager(this);
  TestScene *p_testScene = new TestScene(this);
  m_psceneManager->addScene(p_testScene);
  m_psceneManager->setCurrentScene(0);
  TestScene2 *p_testScene2 = new TestScene2(this);
  m_psceneManager->addScene(p_testScene2);

  m_pdirectionalDepthShader = new Shader(m_putils->getFilePath("/shaders/shadow/shadow.vert"), m_putils->getFilePath("/shaders/shadow/shadow.frag"));
  m_pdirectionalDepthShader->setup(this);

  m_ppointDepthShader = new Shader(m_putils->getFilePath("/shaders/pointShadow/pointShadow.vert"), m_putils->getFilePath("/shaders/pointShadow/pointShadow.frag"), m_putils->getFilePath("/shaders/pointShadow/pointShadow.geom"));
  m_ppointDepthShader->setup(this);

  glfwSetWindowSizeCallback(m_pwindow, [](GLFWwindow *window, int width, int height)
                            {
              Application* p_application = (Application*)glfwGetWindowUserPointer(window);
              p_application->setScreenSize(width, height); });

  glfwSetScrollCallback(m_pwindow, [](GLFWwindow *window, double xoffset, double yoffset)
                        { 
                          Application *p_application = (Application *)glfwGetWindowUserPointer(window);
                          p_application->getInput()->setScrollX(p_application->getInput()->getScrollX() + xoffset);
                          p_application->getInput()->setScrollY(p_application->getInput()->getScrollY() + yoffset); });

  std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "OpenGL Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  std::cout << "Application::setup() Finished Setup" << std::endl;

  return true;
}

void R2::Application::loop()
{
  m_pinput->handleInputs();

  if (m_pinput->isPressedKey(GLFW_KEY_ESCAPE))
  {
    glfwSetWindowShouldClose(m_pwindow, true);
  }

  if (m_pinput->isPressedKey(GLFW_KEY_V) && glfwGetTime() - m_lastTimeMouseVisible >= 0.3f)
  {
    m_lastTimeMouseVisible = glfwGetTime();
    m_isMouseVisible = !m_isMouseVisible;
    glfwSetInputMode(m_pwindow, GLFW_CURSOR, m_isMouseVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    m_pimgui->setMouseInteractions(m_isMouseVisible);
  }

  if (m_pinput->isPressedKey(GLFW_KEY_C) && glfwGetTime() - m_lastTimeImguiEnabled >= 0.3f)
  {
    m_lastTimeImguiEnabled = glfwGetTime();
    m_isImguiEnabled = !m_isImguiEnabled;
  }

  if (m_pinput->isPressedKey(GLFW_KEY_R) && glfwGetTime() - m_lastTimeReloadScene >= 0.3f)
  {
    m_lastTimeReloadScene = glfwGetTime();
    m_psceneManager->getCurrentScene()->reload();
  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_currentFrame = glfwGetTime();
  m_deltaTime = m_currentFrame - m_previousFrame;
  m_previousFrame = m_currentFrame;
  m_fps = 1.0f / m_deltaTime;

  if (m_isVsyncEnabled)
  {
    glfwSwapInterval(1);
  }
  else
  {
    glfwSwapInterval(0);
  }

  m_psceneManager->getCurrentScene()->loop();

  if (m_isImguiEnabled)
  {
    m_pimgui->render();
  }

  m_pinput->resetScrollDirection();

  glfwSwapBuffers(m_pwindow);
  glfwPollEvents();
}

void R2::Application::setScreenSize(unsigned int screenWidth, unsigned int screenHeight)
{
  std::cout << "Application::setScreenSize() " << screenWidth << "x" << screenHeight << std::endl;
  if (screenWidth == 0 || screenHeight == 0)
  {
    std::cout << "Application::setScreenSize() width or height is 0" << std::endl;
    return;
  }

  m_screenWidth = screenWidth;
  m_screenHeight = screenHeight;

  glfwSetWindowSize(m_pwindow, screenWidth, screenHeight);
  glfwSetWindowAspectRatio(m_pwindow, screenWidth, screenHeight);
  m_pimgui->resizeFramebuffer();
}

R2::SceneManager *R2::Application::getSceneManager()
{
  return m_psceneManager;
}

float R2::Application::getDeltaTime()
{
  return m_deltaTime;
}

float R2::Application::getFps()
{
  return m_fps;
}

float R2::Application::getCurrentFrame()
{
  return m_currentFrame;
}

void R2::Application::setVsyncEnabled(bool value)
{
  m_isVsyncEnabled = value;
}

bool R2::Application::getVsyncEnabled()
{
  return m_isVsyncEnabled;
}

void R2::Application::cleanup()
{
  std::cout << "Application::cleanup()" << std::endl;
  glfwTerminate();
  m_pimgui->cleanup();
  m_psceneManager->cleanup();
}

R2::Imgui *R2::Application::getImgui()
{
  return m_pimgui;
}

R2::Config *R2::Application::getConfig()
{
  return m_pconfig;
}

GLFWwindow *R2::Application::getWindow()
{
  return m_pwindow;
}

R2::Input *R2::Application::getInput()
{
  return m_pinput;
}

R2::Utils *R2::Application::getUtils()
{
  return m_putils;
}

R2::Renderer *R2::Application::getRenderer()
{
  return m_prenderer;
}

bool R2::Application::getIsMouseVisible()
{
  return m_isMouseVisible;
}

unsigned int R2::Application::getScreenWidth()
{
  return m_screenWidth;
}

unsigned int R2::Application::getScreenHeight()
{
  return m_screenHeight;
}

R2::Shader *R2::Application::getDirectionalDepthShader()
{
  return m_pdirectionalDepthShader;
}

R2::Shader *R2::Application::getPointDepthShader()
{
  return m_ppointDepthShader;
}