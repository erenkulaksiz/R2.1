#ifndef TESTSCENE_H
#define TESTSCENE_H
#include <R2/Scene.h>
#include <R2/Application.h>

namespace R2
{
  class TestScene : public Scene
  {
  public:
    TestScene(Application *p_application);
    void setup() override;

  private:
    Application *m_papplication;
  };
}

#endif