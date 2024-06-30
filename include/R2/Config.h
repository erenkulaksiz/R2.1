#pragma once
#ifndef CONFIG_H
#define CONFIG_H
#include <iostream>
#include <rapidxml/rapidxml.hpp>
#include <vector>
#include <R2/Application.h>

namespace R2
{
  class Application;

  class Config
  {
  public:
    Config(Application *p_application);
    rapidxml::xml_document<> &getDoc();
    rapidxml::xml_node<> *getRootNode();
    rapidxml::xml_node<> *getSceneNode(std::string sceneName);
    std::vector<rapidxml::xml_node<>*> getSceneNodes();
    std::vector<rapidxml::xml_node<> *> getObjectNodes(std::string sceneName);
    std::string getConfigValue(std::string key);

  private:
    Application *m_papplication;
    std::string appXML = "";
    rapidxml::xml_document<> doc;
    rapidxml::xml_node<> *p_rootNode;
  };
}

#endif