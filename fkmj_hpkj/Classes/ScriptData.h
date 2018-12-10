#pragma once

#include "cocos2d.h"
#include "StructType.h"

class ScriptDataManager
{
public:
	ScriptDataManager(void);
	ScriptDataManager(std::string kFileName);
	~ScriptDataManager(void);
public:
	static ScriptDataManager* getInterface();
	static ScriptDataManager* s_pInterface;

	void clear();
public:
	template<class T>
	static std::map<std::string,T>& S_LIST();

	template<class T>
	static void setValue(std::string kKey,const T& iValue)
	{
		std::map<std::string,T>& kList = S_LIST<T>();
		typename std::map<std::string,T>::iterator itor = kList.find(kKey);
		if(itor != kList.end())
		{
			itor->second = iValue;
			return;
		}
		kList.insert(std::pair<std::string,T>(kKey,iValue));
	}

	template<class T>
	static bool haveValue(std::string kKey)
	{
		std::map<std::string,T>& kList = S_LIST<T>();
		typename std::map<std::string,T>::iterator itor = kList.find(kKey);
		if(itor != kList.end())
		{
			return true;
		}
		return false;
	}

	template<class T>
	static void addValue(std::string kKey,const T& iValue)
	{
		std::map<std::string,T>& kList = S_LIST<T>();
		typename std::map<std::string,T>::iterator itor = kList.find(kKey);
		if(itor != kList.end())
		{
			cocos2d::log("have already Value %s",kKey.c_str());
			return;
		}
		kList.insert(std::pair<std::string,T>(kKey,iValue));
	}
protected:
	std::map<std::string,int> m_mapValueInt;
	std::map<std::string,float> m_mapValueFloat;
	std::map<std::string,std::string> m_mapValueStr;
	std::map<std::string,cocos2d::Vec2> m_mapValuePoint;
	std::map<std::string,cocos2d::Size> m_mapValueSize;
};

template<class T>
class ScriptData
{
public:
	ScriptData(std::string pcName)
	{
		std::map<std::string,T>& kList =ScriptDataManager::S_LIST<T>();
		typename std::map<std::string,T>::const_iterator iter = kList.find(pcName);
		if(iter != kList.end())
		{
			m_kValue = (*iter).second;
		}
		else
		{
			std::string kAssert = std::string(pcName) + " Can't Find ScriptConstValue";
			CCAssert(false,kAssert.c_str());
		}
	}
	const T& Value()const{return m_kValue;}
private:
	T m_kValue;
};

