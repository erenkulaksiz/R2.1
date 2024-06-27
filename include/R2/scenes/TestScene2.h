#ifndef TESTSCENE2_H
#define TESTSCENE2_H
#include <R2/Scene.h>
#include <R2/Application.h>

namespace R2
{
  class TestScene2 : public Scene
  {
  public:
    TestScene2(Application *p_application);
    void setup() override;

  private:
    Application *m_papplication;
  };
}

#endif