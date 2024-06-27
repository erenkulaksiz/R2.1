#pragma once
#ifndef INPUT_H
#define INPUT_H
#include <GLFW/glfw3.h>
#include <vector>
#include <R2/Application.h>

namespace R2
{
  class Application;

  class Input
  {
  public:
    Input(Application *p_application);
    void handleInputs();
    bool isPressedKey(int key) const;
    bool isPressingMouseKey(int key) const;
    double getMouseX();
    double getMouseY();
    double getScrollX();
    double getScrollY();
    void setScrollX(double x);
    void setScrollY(double y);
    double getScrollXDirection();
    double getScrollYDirection();
    void resetScrollDirection();

  private:
    Application *m_papplication;
    std::vector<int> pressedKeys;
    double m_mouseX = 0;
    double m_mouseY = 0;
    double m_scrollX = 0;
    double m_scrollY = 0;
    double m_lastScrollX = 0;
    double m_lastScrollY = 0;
    double m_scrollXDirection = 0;
    double m_scrollYDirection = 0;
  };
}

#endif
