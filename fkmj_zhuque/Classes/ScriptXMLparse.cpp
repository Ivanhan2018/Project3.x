#include "ScriptXMLparse.h"
#include "utility.h"
#include "ScriptData.h"
#include "utility.h"

USING_NS_CC;

ScriptXMLparse::ScriptXMLparse(  std::string xmlFileName )
{
	SAXParser parser;
	parser.setDelegator(this);
	std::string kFullPath = utility::FullPath(xmlFileName);
	parser.parse(kFullPath.c_str());
}


void ScriptXMLparse::startElement(void *ctx, const char *name, const char **atts)
{
	std::string elementName = (char*)name;
	std::map<std::string, std::string> attributeDict;
	utility::createAttributeDict(attributeDict,atts);

	if(elementName == "ScriptValueInt")
	{
		std::string kName = utility::valueForKey("Key", attributeDict);
		int iValue = (utility::valueForInt("Value", attributeDict));
		ScriptDataManager::addValue<int>(kName,iValue);
	}
	if(elementName == "ScriptValueStr")
	{
		std::string kName = utility::valueForKey("Key", attributeDict);
		std::string iValue = (utility::valueForKey("Value", attributeDict));
		ScriptDataManager::addValue<std::string>(kName,iValue);
	}
	if(elementName == "ScriptValueFloat")
	{
		std::string kName = utility::valueForKey("Key", attributeDict);
		float fValue = (utility::valueForFloat("Value", attributeDict));
		ScriptDataManager::addValue<float>(kName,fValue);
	}
	if(elementName == "ScriptValuePoint")
	{
		std::string kName = utility::valueForKey("Key", attributeDict);
		cocos2d::Vec2 kValue = (utility::valueForPoint("Value", attributeDict));
		ScriptDataManager::addValue<cocos2d::Vec2>(kName,kValue);
	}
	if(elementName == "ScriptValueSize")
	{
		std::string kName = utility::valueForKey("Key", attributeDict);
		cocos2d::Size kValue = (utility::valueForSize("Value", attributeDict));
		ScriptDataManager::addValue<cocos2d::Size>(kName,kValue);
	}
	if(elementName == "ReplaceStr")
	{
		std::string kName = utility::valueForKey("Key", attributeDict);
		std::string kValue = utility::valueForKey("Value", attributeDict);
		ScriptDataManager::setValue<std::string>(kName,kValue);
	}
	if(elementName == "LoadPic")
	{
		std::string kPicName1 = utility::valueForKey("Texture", attributeDict);
		cocos2d::TextureCache::getInstance()->addImage(kPicName1.c_str());
	}
	if(elementName == "LoadFrames")
	{
		std::string kListFile = utility::valueForKey("ListFile", attributeDict);
		std::string kImgFile = utility::valueForKey("ImgFile", attributeDict);
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(kListFile.c_str(),kImgFile.c_str());
	}
}
void ScriptXMLparse::endElement(void *ctx, const char *name)
{

}

void ScriptXMLparse::textHandler(void *ctx, const char *s, int len)
{

}
