#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <R2/Imgui.h>
#include <R2/SceneManager.h>
#include <R2/Input.h>
#include <R2/Config.h>
#include <R2/Utils.h>

namespace R2
{
  class Imgui;
  class SceneManager;
  class Input;
  class Config;
  class Renderer;
  class Shader;

  class Application
  {
  public:
    Application();
    bool setup();
    void loop();
    void setScreenSize(unsigned int screenWidth, unsigned int screenHeight);
    void cleanup();
    float getDeltaTime();
    float getFps();
    float getCurrentFrame();
    void setVsyncEnabled(bool value);
    bool getVsyncEnabled();
    GLFWwindow *getWindow();
    Imgui *getImgui();
    Config *getConfig();
    SceneManager *getSceneManager();
    Input *getInput();
    Utils *getUtils();
    Renderer *getRenderer();
    bool getIsMouseVisible();
    unsigned int getScreenWidth();
    unsigned int getScreenHeight();
    int getFrameCount();
    GLFWwindow *getSharedWindow();

  private:
    GLFWwindow *m_pwindow;
    GLFWwindow *m_psharedWindow;
    Imgui *m_pimgui;
    SceneManager *m_psceneManager;
    Input *m_pinput;
    Config *m_pconfig;
    Utils *m_putils;
    Renderer *m_prenderer;
    const char *p_mtitle = "R2";
    unsigned int m_screenWidth;
    unsigned int m_screenHeight;
    float m_lastTimeMouseVisible = 0.0f;
    float m_lastTimeImguiEnabled = 0.0f;
    float m_lastTimeReloadScene = 0.0f;
    bool m_isMouseVisible = false;
    bool m_isVsyncEnabled = true;
    bool m_isImguiEnabled = true;
    float m_fps;
    float m_deltaTime;
    float m_currentFrame;
    float m_previousFrame;
    int m_frameCount = 0;
  };
}

#endif