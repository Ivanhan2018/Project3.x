#include "TinyXmlReader.h"


//const char* TinyXMLDocument::readString(const char* key)
//{
//	// get root node
//	tinyxml2::XMLElement* rootNode = RootElement();
//	if (NULL == rootNode)
//	{
//		CCLOG("read root node error");
//		return "";
//	}
//	// find the node
//	tinyxml2::XMLElement* curNode = rootNode->FirstChildElement();
//	while (NULL != curNode)
//	{
//		const char* nodeName = curNode->Value();
//		if (!strcmp(nodeName, key))
//		{
//			break;
//		}
//
//		curNode = curNode->NextSiblingElement();
//	}
//
//	const char* value = NULL;
//
//	// find the node
//	if (curNode && curNode->FirstChild())
//	{
//		value = (const char*)(curNode->FirstChild()->Value());
//	}
//
//	return value;
//}


//SINGLETON_DEFINE_INST(TinyXmlReader);

//TinyXmlReader::TinyXmlReader(void)
//{
//
//}
//
//
//TinyXmlReader::~TinyXmlReader(void)
//{
//}
//
//TinyXMLDocument* TinyXmlReader::open(string& path)
//{
//	XmlIter iter = m_cache.find(path);
//	if (iter != m_cache.end()) 
//	{
//		return iter->second;
//	}
//
//	unsigned long size;
//	const char* pXmlBuffer = (const char*)FileUtils::getInstance()->getFileData(path.c_str(), "r", &size);
//
//	TinyXMLDocument* xmlDoc = new TinyXMLDocument();
//
//	if(path.length() == 0)
//	{
//		CCLOG("can not read xml file");
//		return NULL;
//	}
//
//	xmlDoc->Parse(pXmlBuffer);
//
//	delete pXmlBuffer;
//	m_cache.insert(make_pair(path,xmlDoc));
//
//	return xmlDoc;
//}
//
	//bool TinyXmlReader1::purge(string& path)
	//{
	//	XmlIter iter = m_cache.find(path);
	//	if (iter != m_cache.end())
	//	{
	//		delete iter->second;
	//		m_cache.erase(iter);
	//		return true;
	//	}
	//	return false;
	//}
	//
	//void TinyXmlReader1::purgeAll()
	//{
	//	XmlIter iter = m_cache.begin();
	//	while (iter != m_cache.end())
	//	{
	//		delete iter->second;
	//		iter++;
	//	}
	//	m_cache.clear();
	//}
