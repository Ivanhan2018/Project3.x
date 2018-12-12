#include "ConfigMgr.h"


SINGLETON_DEFINE_INST(ConfigMgr);


ConfigMgr::ConfigMgr()
{
	m_Config.m_nGolbalBankruptcy = 500;
	m_Config.m_isGameBankrupcy = false;
	m_Config.m_channelStr = "";
	m_Config.m_bToAndroid = false;
	m_Config.m_isGameRun=false;
	m_VersionInfo.m_nHighVersion = 0;
	m_VersionInfo.m_nMiddleVersion = 0;
	m_VersionInfo.m_nLowVersion = 0;
}

const char* ConfigMgr::text(const char* key)
{
    string path = "text.xml";
	TinyXMLDocument* doc = m_TinyXmlReader.open(path);
	return doc->readString(key);
}

const char* ConfigMgr::text(const char* filename, const char* key)
{
    string path = filename;
	TinyXMLDocument* doc = m_TinyXmlReader.open(path);
	return doc->readString(key);
}