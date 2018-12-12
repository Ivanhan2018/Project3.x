#ifndef __CONFIGMGR_READER_H__
#define __CONFIGMGR_READER_H__
 
#include "AnsSingleton.h"
#include "TinyXmlReader.h"

struct SeverConfigInfo
{
	int			m_nGolbalBankruptcy;
	bool		m_isGameBankrupcy;
	string		m_channelStr;

	int			nServerEnter[6];
	char		szTitle[64];
	char		szContent[512];

	bool		m_bToAndroid;
	bool     m_isGameRun;
};

struct VersionInfo
{
	int m_nHighVersion;
	int m_nMiddleVersion;
	int m_nLowVersion;
};

class ConfigMgr  : public ans::Singleton<ConfigMgr> 
{
private:
	SINGLETON_FRIEND_SUB(ConfigMgr);
public:
	ConfigMgr();

public:
	const char* text(const char* key);
	const char* text(const char* filename, const char* key);
	SeverConfigInfo m_Config;
	VersionInfo m_VersionInfo;

private:
	TinyXmlReader m_TinyXmlReader;
};

#define pConfigMgr ConfigMgr::instance()

#endif
