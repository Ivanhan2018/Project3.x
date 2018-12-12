#ifndef MYCONFIG_H
#define MYCONFIG_H

#include "cocos2d.h"
#include <string>
#include <map>
#include <vector>
#include "external\tinyxml2\tinyxml2.h"
#include "AnsSingleton.h"

using namespace cocos2d;
using namespace std;
using namespace tinyxml2;

#define MAX_COLUMN_NUM (5)

/*
	读取xml文件中的登录服IP和端口、百家乐坐标配置
*/
class MyConfig
{
public:
	MyConfig(const char *xmlFileName=0);
	static MyConfig & Instance();
	int LoadData(const char *xmlFileName);
	void SetString(XMLElement* Node,const char * Attr,string& m_Attr);
	void SetInt(XMLElement* Node,const char * Attr,int& m_Attr);
	void SetPoint(XMLElement* Node,const char * Attr,float* m_pAttr);
	void SetRect(XMLElement* Node,const char * Attr,float* m_pAttr,bool isXYWH=false);
	void SetDouble(XMLElement* Node,const char * Attr,float& m_Attr);
public:
	bool m_HasLoadOK;//是否加载成功过

	string m_ip1;
	string m_ip2;
	string m_ip3;
	int m_port;

	float m_GameRecordPos[2];
	float m_BankerUserPos[2];
	float m_ClosePos[2];
	float m_UpBankerPos[2];
	float m_padding;
	float m_PaiZiPos[2];
	float m_YanwuParticlePos[2];
	float m_TimerPos[2];
	float m_NikePos[2];
	float m_GameFreeTipPos[2];
	float m_GameStartTipPos[2];
	float m_CallScoreTipPos[2];
	float m_GameEndTipPos[2];
	float m_ChangeBankerTipPos[2];

	float m_fOffsetX; // 横向间隔
	float m_fStartX; // 横向起始位置X
	float m_fOffsetY; // 纵向间隔
	float m_fStartY; // 庄纵向起始位置Y

	float m_BankerPicPos[2];
	float m_BankerNamePos[2];
	float m_BankerChengjiPos[2];
	float m_BankerGoldPos[2];
	float m_BankerJiNumberPos[2];

	float m_MyPicPos[2];
    float m_MyNamePos[2];
	float m_MyChengjiPos[2];
    float m_MyGoldPos[2];

	float m_XianZhuangBgPos[2];
	float m_UserPlayPos[2];
    float m_UserBankPos[2];
	float m_PlayerPos[2];
    float m_BankerPos[2];
	float m_AndBureauPos[2];
    float m_GAMEoverPos1[2];
	float m_GAMEoverPos2[2];
    float m_playerCardPos1[2];
	float m_PlayOneCardPos1[2];
	float m_PlayOneCardPos2[2];
	float m_BankOneCardPos1[2];
	float m_BankOneCardPos2[2];
	float m_CardOffsetX;

	float m_GAMEoverLblPos[11][2];
	float m_ChipManagerNunberPos[8][2];
	float m_OperaLayerMyNunberPos[8][2];
	float m_ChipPosRect[8][4];
	float m_ChipMenuScale;
	float m_ChipMenuPos[7][2];

	int m_NumberOfCells;
	float m_CellSize[2];
	float m_ViewSize[2];
	float m_ViewPos[2];
	int m_ColumnNum;
	float m_LblPos[MAX_COLUMN_NUM][2];//最多5列
};

#endif

