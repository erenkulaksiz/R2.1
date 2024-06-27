#define GLFW_INCLUDE_NONE
#include <R2/Input.h>
#include <algorithm>

R2::Input::Input(Application *p_application)
{
  this->m_papplication = p_application;
}

void R2::Input::handleInputs()
{
  pressedKeys.clear();

  for (int i = 0; i < GLFW_KEY_LAST; i++)
  {
    if (glfwGetKey(m_papplication->getWindow(), i) == GLFW_PRESS)
    {
      pressedKeys.push_back(i);
    }
  }

  glfwGetCursorPos(m_papplication->getWindow(), &m_mouseX, &m_mouseY);
}

bool R2::Input::isPressedKey(int key) const
{
  return std::find(pressedKeys.begin(), pressedKeys.end(), key) != pressedKeys.end();
}

bool R2::Input::isPressingMouseKey(int key) const
{
  return glfwGetMouseButton(m_papplication->getWindow(), key) == GLFW_PRESS;
}

double R2::Input::getMouseX()
{
  return m_mouseX;
}

double R2::Input::getMouseY()
{
  return m_mouseY;
}

double R2::Input::getScrollX()
{
  return m_scrollX;
}

double R2::Input::getScrollY()
{
  return m_scrollY;
}

void R2::Input::setScrollX(double x)
{
  if (x != m_scrollX) // Update direction only if there is a change
  {
    m_scrollX = x;

    if (m_scrollX > m_lastScrollX)
    {
      m_scrollXDirection = 1;
    }
    else if (m_scrollX < m_lastScrollX)
    {
      m_scrollXDirection = -1;
    }

    m_lastScrollX = m_scrollX;
  }
}

void R2::Input::setScrollY(double y)
{
  if (y != m_scrollY) // Update direction only if there is a change
  {
    m_scrollY = y;

    if (m_scrollY > m_lastScrollY)
    {
      m_scrollYDirection = 1;
    }
    else if (m_scrollY < m_lastScrollY)
    {
      m_scrollYDirection = -1;
    }

    m_lastScrollY = m_scrollY;
  }
}

double R2::Input::getScrollXDirection()
{
  return m_scrollXDirection;
}

double R2::Input::getScrollYDirection()
{
  return m_scrollYDirection;
}

void R2::Input::resetScrollDirection()
{
  m_scrollXDirection = 0;
  m_scrollYDirection = 0;
}