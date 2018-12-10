#include "ScriptData.h"
#include "ScriptXMLparse.h"


ScriptDataManager* ScriptDataManager::s_pInterface = NULL;

ScriptDataManager::ScriptDataManager(void)
{
	s_pInterface = this;
}
ScriptDataManager::ScriptDataManager(std::string kFileName)
{
	s_pInterface = this;
	cocos2d::ScriptXMLparse kXml1(kFileName);
}

ScriptDataManager::~ScriptDataManager(void)
{
}
ScriptDataManager* ScriptDataManager::getInterface()
{
	if(s_pInterface == NULL)
	{
		s_pInterface = new ScriptDataManager(); 
	}
	return s_pInterface;
}
void ScriptDataManager::clear()
{
	m_mapValueInt.clear();
	m_mapValueFloat.clear();
	m_mapValueStr.clear();
	m_mapValuePoint.clear();
	m_mapValueSize.clear();
}
template<> 
std::map<std::string,float>& ScriptDataManager::S_LIST<float>()
{
	return getInterface()->m_mapValueFloat;
}
template<> 
std::map<std::string,int>& ScriptDataManager::S_LIST<int>()
{
	return getInterface()->m_mapValueInt;
}

template<> 
std::map<std::string,std::string>& ScriptDataManager::S_LIST<std::string>()
{
	return getInterface()->m_mapValueStr;
}

template<> 
std::map<std::string,cocos2d::Vec2>& ScriptDataManager::S_LIST<cocos2d::Vec2>()
{
	return getInterface()->m_mapValuePoint;
}
template<> 
std::map<std::string,cocos2d::Size>& ScriptDataManager::S_LIST<cocos2d::Size>()
{
	return getInterface()->m_mapValueSize;
}

