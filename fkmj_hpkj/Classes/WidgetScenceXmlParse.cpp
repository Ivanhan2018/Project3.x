#include "WidgetScenceXmlParse.h"
#include "utility.h"
#include "ScriptData.h"
#include "utility.h"
#include "WidgetManager.h"
#include "WidgetSkinXMLparse.h"
#include "SoundFun.h"
#include "WidgetFun.h"

USING_NS_CC;

WidgetScenceXMLparse::WidgetScenceXMLparse(  std::string xmlFileName,cocos2d::Node* pNode)
{
	m_pAddNodes.push_back(pNode);
	m_pRootNode = pNode;

	WidgetUserInfo* pUserInfo = new WidgetUserInfo;
	pUserInfo->bRoot = true;
	pNode->setUserObject(pUserInfo);
 
	SAXParser parser;
	parser.setDelegator(this);
	std::string kFullPath = utility::FullPath(xmlFileName);
	parser.parse(kFullPath.c_str());
}


bool bWidgetScenceXMLparseTest = true;
void WidgetScenceXMLparse::setTest(bool bValue)
{
	bWidgetScenceXMLparseTest = bValue;
}
void WidgetScenceXMLparse::startElement(void *ctx, const char *name, const char **atts)
{
	std::string elementName = (char*)name;
	std::map<std::string, std::string> attributeDict;
	utility::createAttributeDict(attributeDict,atts);
	if (!bWidgetScenceXMLparseTest)
	{
		return;
	}

	if(elementName == "LoadSkin")
	{
		std::string kSkin = utility::valueForKey("Skin", attributeDict);
		WidgetSkinXMLparse kXml(kSkin);
	}
	if(elementName == "ReloadSound")
	{
		std::string kName = utility::valueForKey("SoundName", attributeDict);
		SoundFun::Instance().playEffect(kName,true);
	}
	if (elementName == "Widget")
	{
		CCAssert(m_pAddNodes.size(),"m_pAddNodes");
		std::string kName = utility::valueForKey("Name",attributeDict);
		std::string kSkinTempName = utility::valueForKey("SkinTempName",attributeDict);
		WidgetInfo* pWidgetInfo = WidgetManager::Instance().getWidgetInfo(kSkinTempName,false);
		if (!pWidgetInfo)
		{
			pWidgetInfo = WidgetManager::Instance().getWidgetInfo("Error");
		}
		cocos2d::Node* pNode = WidgetManager::Instance().createWidget(pWidgetInfo,m_pAddNodes.back());
		WidgetUserInfo* pUserInfo = dynamic_cast<WidgetUserInfo*>(pNode->getUserObject());
		pUserInfo->bIsSkin = false;
		pUserInfo->kName = kName;
		pUserInfo->kSkinTempName = kSkinTempName;
		m_pAddNodes.push_back(pNode);
		m_pWidgetInfos.push_back(pWidgetInfo);
	}
	if (elementName == "ScenceXml")
	{
		CCAssert(m_pAddNodes.size(),"m_pAddNodes");
		std::string kScenceXml = utility::valueForKey("File",attributeDict);
		WidgetScenceXMLparse kXml(kScenceXml,m_pAddNodes.back());
	}
	if (elementName == "Property")
	{
		CCAssert(m_pAddNodes.size() > 1 && m_pWidgetInfos.size(),"m_pAddNodes");
		cocos2d::Node* pNode = m_pAddNodes.back();
		WidgetInfo* pWidgetInfo = m_pWidgetInfos.back();
		std::string kKey = utility::valueForKey("Key",attributeDict);
		std::string kValue = utility::valueForKey("Value",attributeDict);
		WidgetManager::Instance().changePropertyByType(pNode,pWidgetInfo->kType,kKey,kValue);
	}
	if (elementName == "UserInfo")
	{
		CCAssert(m_pAddNodes.size() > 1 && m_pWidgetInfos.size(),"m_pAddNodes");
		std::string kKey = utility::valueForKey("Key",attributeDict);
		std::string kValue = utility::valueForKey("Value",attributeDict);
		cocos2d::Node* pNode = m_pAddNodes.back();
		WidgetUserInfo* pUserInfo = dynamic_cast<WidgetUserInfo*>(pNode->getUserObject());
		StringMap::iterator itor = pUserInfo->kUserInfo.find(kKey);
		if (itor != pUserInfo->kUserInfo.end())
		{
			itor->second = kValue;
		}
		else
		{
			pUserInfo->kUserInfo.insert(std::make_pair(kKey,kValue));
		}
	}
	if (elementName == "Action")
	{
		CCAssert(m_pAddNodes.size() > 1 && m_pWidgetInfos.size(),"m_pAddNodes");
		std::string kKey = utility::valueForKey("Key",attributeDict);
		std::string kValue = utility::valueForKey("Value",attributeDict);
		cocos2d::Node* pNode = m_pAddNodes.back();
		WidgetUserInfo* pUserInfo = dynamic_cast<WidgetUserInfo*>(pNode->getUserObject());
		pUserInfo->kAction.push_back(stringKeyValue(kKey,kValue));
	}
	if (elementName == "ChildProperty")
	{
		cocos2d::Node* pNode = m_pAddNodes.back();
		std::string kName = utility::valueForKey("Name",attributeDict);
		std::string kKey = utility::valueForKey("Key",attributeDict);
		std::string kValue = utility::valueForKey("Value",attributeDict);
		WidgetFun::setWidgetProperty(pNode,kName,kKey,kValue);
	}
	if (elementName == "MoveWidget")
	{
		std::string kName = utility::valueForKey("Name",attributeDict);
		cocos2d::Node* pNode = WidgetFun::getChildWidget(m_pRootNode,kName);
		m_pAddNodes.push_back(pNode);
	}
}
void WidgetScenceXMLparse::endElement(void *ctx, const char *name)
{
	std::string elementName = (char*)name;
	if (elementName == "Widget" && m_pAddNodes.size() && m_pWidgetInfos.size())
	{
		m_pAddNodes.pop_back();
		m_pWidgetInfos.pop_back();
	}
	if (elementName == "MoveWidget" && m_pAddNodes.size() && m_pWidgetInfos.size())
	{
		m_pAddNodes.pop_back();
		m_pWidgetInfos.pop_back();
	}
}

void WidgetScenceXMLparse::textHandler(void *ctx, const char *s, int len)
{

}
