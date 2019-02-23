//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName     :   ConfigMgr
//  Version      :   1.0
//  Creater      :   zhouwei
//  Date         :   2013.09.11
//  Comment    	 :   ��Դ��������
//
//////////////////////////////////////////////////////////////////////////////////////


#ifndef __CONFIGMGR_READER_H__
#define __CONFIGMGR_READER_H__
 
#include "AnsSingleton.h"
#include "TinyXmlReader.h"

struct SeverConfigInfo
{
	int			m_nGolbalBankruptcy;			//--�Ʋ�ֵ
	bool		m_isGameBankrupcy;				//--С��Ϸ�Ʋ�
	string		m_channelStr;					//��������

	int			nServerEnter[6];
	char		szTitle[64];					// �������
	char		szContent[512];					// ��������

	bool		m_bToAndroid;					//�Ƿ����android����
	bool     m_isGameRun;					//�Ƿ����С��Ϸ
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
	//�����ı���Դ
	const char* text(const char* key);
	const char* text(const char* filename, const char* key);
	SeverConfigInfo m_Config;
	VersionInfo m_VersionInfo;

private:
	TinyXmlReader m_TinyXmlReader;
};

#define STR_XDSJ ConfigMgr::instance()->text("display_text.xml", "t72")//下单时间
#define STR_FDSJ ConfigMgr::instance()->text("display_text.xml", "t74")//封单时间

#endif //#define __CONFIGMGR_READER_H__
