#include <R2/Application.h>

int main(int argc, char** argv)
{
  R2::Application app;

  if (app.setup())
  {
    while (!glfwWindowShouldClose(glfwGetCurrentContext()))
    {
      app.loop();
    }
  }

  app.cleanup();
  return 0;
}