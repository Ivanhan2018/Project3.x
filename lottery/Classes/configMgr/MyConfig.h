#ifndef MYCONFIG_H
#define MYCONFIG_H

#include "cocos2d.h"
#include <string>
#include <map>
#include <vector>
#include "tinyxml2/tinyxml2.h"
#include "AnsSingleton.h"

using namespace cocos2d;
using namespace std;
using namespace tinyxml2;

#define MAX_COLUMN_NUM (5)
#define MAX_GAME_NUM (7)
#define MAX_PLATFORM_NUM (2)

/*
	读取xml文件中的登录服IP和端口、百家乐坐标配置
*/
class MyConfig
{
public:
	MyConfig(const char *xmlFileName=0);
	static MyConfig & Instance();
	int LoadData(const char *xmlFileName);
	void ParseHotUpdate(XMLElement* Node1d);
	void ParseBJL(XMLElement* Node1b);
	void ParseDZPK(XMLElement* Node1c);
	void ParsePPC(XMLElement* Node1e);
	void ParseFISHBEAT(XMLElement* Node1f);
	void SetString(XMLElement* Node,const char * Attr,string& m_Attr);
	void SetInt(XMLElement* Node,const char * Attr,int& m_Attr);
	void SetPoint(XMLElement* Node,const char * Attr,float* m_pAttr,float* pOffset=0);
	void SetRect(XMLElement* Node,const char * Attr,float* m_pAttr,bool isXYWH=false);
	void SetDouble(XMLElement* Node,const char * Attr,float& m_Attr);
public:
	bool m_HasLoadOK;//是否加载成功过

	string m_ip1;
	string m_ip2;
	string m_ip3;
	string m_ip4;
	int m_port;

	// HotUpdate
	int m_platform;
	int m_enable[MAX_PLATFORM_NUM][MAX_GAME_NUM];	
	string m_manifestUrl[MAX_PLATFORM_NUM][MAX_GAME_NUM];		
	string m_storagePath[MAX_PLATFORM_NUM][MAX_GAME_NUM];
	float m_loadingSuccessScale;
	float m_loadingErrorScale;
	float m_loadingPos[2];
	float m_loadingProgressBgPos[2];
	float m_loadingProgressPos[2];
	float m_ProgressLabelPos[2];
	float m_loadingSuccessPos[2];
	float m_loadingErrorPos[2];
	float m_bkPos[2];
	float m_ClosePos[2];
	float m_ltfTitlePos[2];
	float m_sptTipsPos[2];

	// bjl
	float m_LblSize[MAX_PLATFORM_NUM][2];
	float m_GameRecordPos[2];
	float m_BankerUserPos[2];
	float m_CloseDownPos[2];
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
	float m_LblOffset[2];
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

	// dzpk
	float m_ChipPotPos[MAX_PLATFORM_NUM][2];	
	float m_TotalChipsBGPos[2];
	float m_TotalChipValuePos[2];
	float m_SendCardMachinePos[2];
	float m_CloseRoundOperaLabelPos[2];
	float m_WinLosePos[2];
	float m_OtherCardScale;
	float m_SelfCardScale;
	float m_headscale;
	float m_levelscale;
	float m_chipValueScale;
	float m_playerChipScale;
	float m_playerChipValueScale;
	float m_nullHeadPos[8][2];
	float m_infoBGPos[8][2];
	float m_chipValueBGPos[8][2];
	float m_headPos[8][2];
	float m_levelPos[8][2];
	float m_offlinePos[8][2];
	float m_nextRoundPos[8][2];
	float m_playerNamePos[8][2];
	float m_actionNamePos[8][2];
	float m_chipValuePos[8][2];
	float m_CountDownPos[8][2];
	float m_ReadyPos[8][2];
	float m_MakersPos[8][2];
	float m_playerCard0Pos[8][2];
	float m_playerCard1Pos[8][2];
	float m_actionBGPos[8][2];
	float m_playerChipPos[8][2];
	float m_playerChipValuePos[8][2];
	float m_commonCardsScale;
	float m_commonCardsPos[5][2];
	float m_DZPKClosePos[2];
	float m_StartItemPos[2];
	float m_LeaveItemPos[2];
	float m_StartLeaveScale;
	float m_ExitScale;
	float m_DZPKOperaLayerScale;
	float m_DZPKOperaLayerPos[12][2];
	float m_t1;
	float m_t2;
	float m_test1;
	float m_test2;

	// ppc
	float m_FacePos[2];
	float m_InfoBarOffset[2][2];
	float m_EndingPos[4][2];

	// fishbeat
	float m_CannonNodePos[4][2];
};

#endif

