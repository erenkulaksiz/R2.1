#include <R2/Config.h>
#include <R2/Utils.h>

R2::Config::Config(Application *p_application)
{
  std::cout << "Config::Config()" << std::endl;
  this->m_papplication = p_application;

  try
  {
    this->appXML = m_papplication->getUtils()->getFileContents(m_papplication->getUtils()->getFilePath("/app.xml"));

    if (this->appXML.empty())
    {
      std::cerr << "Error: app.xml is empty or no app.xml" << std::endl;
      throw std::runtime_error("app.xml is empty or no app.xml");
    }

    this->doc.parse<0>(&appXML[0]);
    this->p_rootNode = this->doc.first_node("application");
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

rapidxml::xml_document<> &R2::Config::getDoc()
{
  return this->doc;
}

rapidxml::xml_node<> *R2::Config::getRootNode()
{
  return this->p_rootNode;
}

std::vector<rapidxml::xml_node<>*> R2::Config::getSceneNodes()
{
  std::vector<rapidxml::xml_node<>*> sceneNodes;
  rapidxml::xml_node<> *rootNode = this->p_rootNode->first_node("scenes");

  for (rapidxml::xml_node<> *sceneNode = rootNode->first_node("scene"); sceneNode; sceneNode = sceneNode->next_sibling())
  {
    sceneNodes.push_back(sceneNode);
  }

  return sceneNodes;
}

rapidxml::xml_node<> *R2::Config::getSceneNode(std::string sceneName)
{
  rapidxml::xml_node<> *rootNode = this->p_rootNode->first_node("scenes");

  for (rapidxml::xml_node<> *sceneNode = rootNode->first_node("scene"); sceneNode; sceneNode = sceneNode->next_sibling())
  {
    if (std::string(sceneNode->first_attribute("name")->value()) == sceneName)
    {
      return sceneNode;
    }
  }

  return nullptr;
}

std::vector<rapidxml::xml_node<> *> R2::Config::getObjectNodes(std::string sceneName)
{
  std::vector<rapidxml::xml_node<> *> objectNodes;
  rapidxml::xml_node<> *sceneNode = getSceneNode(sceneName);

  for (rapidxml::xml_node<> *childNode = sceneNode->first_node(); childNode; childNode = childNode->next_sibling())
  {
    objectNodes.push_back(childNode);
  }

  return objectNodes;
}

std::string R2::Config::getConfigValue(std::string key)
{
  rapidxml::xml_node<> *rootNode = this->p_rootNode->first_node("config");

  for (rapidxml::xml_node<> *configNode = rootNode->first_node("item"); configNode; configNode = configNode->next_sibling())
  {
    if (std::string(configNode->first_attribute("name")->value()) == key)
    {
      return configNode->first_attribute("value")->value();
    }
  }

  return "";
}