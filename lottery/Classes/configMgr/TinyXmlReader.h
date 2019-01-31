//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName     :   TinyXmlReader.h
//  Version      :   1.0
//  Creater      :   zhouwei
//  Date         :   2013-9-12   
//  Comment    	 :   xml½âÎöÆ÷
//
//////////////////////////////////////////////////////////////////////////////////////
#ifndef __XML_READER_H__
#define __XML_READER_H__

#include "cocos2d.h"
#include <string>
#include <map>
#include "tinyxml2/tinyxml2.h"
#include "AnsSingleton.h"


using namespace cocos2d;
using namespace std;

class TinyXMLDocument : public  tinyxml2::XMLDocument
{
public:
	const char* readString(const char* key)
	{
		// get root node
		tinyxml2::XMLElement* rootNode = RootElement();
		if (NULL == rootNode)
		{
			CCLOG("read root node error");
			return "";
		}
		// find the node
		tinyxml2::XMLElement* curNode = rootNode->FirstChildElement();
		while (NULL != curNode)
		{
			const char* nodeName = curNode->Value();
			if (!strcmp(nodeName, key))
			{
				break;
			}

			curNode = curNode->NextSiblingElement();
		}

		const char* value = NULL;

		// find the node
		if (curNode && curNode->FirstChild())
		{
			value = (const char*)(curNode->FirstChild()->Value());
		}

		return value;
	}
};

class TinyXmlReader
{
private:
	typedef map<string, TinyXMLDocument*> TinyXml;
	typedef TinyXml::iterator XmlIter;
public:
	TinyXmlReader(void)
	{
	}
	~TinyXmlReader(void)
	{
	}

	TinyXMLDocument* open(string& path)
	{
		XmlIter iter = m_cache.find(path);
		if (iter != m_cache.end()) 
		{
			return iter->second;
		}

		ssize_t size;
		const char* pXmlBuffer = (const char*)FileUtils::getInstance()->getFileData(path.c_str(), "r", &size);

		TinyXMLDocument* xmlDoc = new TinyXMLDocument();

		if(path.length() == 0)
		{
			CCLOG("can not read xml file");
			return NULL;
		}

		xmlDoc->Parse(pXmlBuffer);

		delete pXmlBuffer;
		m_cache.insert(make_pair(path,xmlDoc));

		return xmlDoc;
	}

	bool purge(string& path)
	{
		XmlIter iter = m_cache.find(path);
		if (iter != m_cache.end())
		{
			delete iter->second;
			m_cache.erase(iter);
			return true;
		}
		return false;
	}

	void purgeAll()
	{
		XmlIter iter = m_cache.begin();
		while (iter != m_cache.end())
		{
			delete iter->second;
			iter++;
		}
		m_cache.clear();
	}

private:
	TinyXml  m_cache;
};
#endif //__XML_READER_H__
