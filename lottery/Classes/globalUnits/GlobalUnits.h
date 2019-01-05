//
//  GlobalUnits.h
//
//  这个文件为了跟pc的保持一致，暂时这样，以后可能会干掉
//  Created by zhouwei on 13-6-9.
//
//

#ifndef __GlobalUnits__
#define __GlobalUnits__

#include <iostream>
#include "Define.h"
#include "GlobalDef.h"
#include <vector>
#include "AnsTaskPool.h"
#include "GlobalUnits.h"
#include "struct.h"
typedef std::vector<tagGameServer> GameServerList;
typedef std::vector<tagUserData*> GameUsrDateList;

//全局用户资料
struct tagGlobalUserData
{
	//属性资料
	//基本信息
	WORD								wFaceID;						//头像索引
	DWORD								dwUserID;						//用户 I D
	DWORD								dwGameID;						//用户 I D
	DWORD								dwGroupID;						//社团索引

	char								szAccounts[NAME_LEN];			//登录帐号
	char								szPassWord[PASS_LEN];			//登录密码
	char								szGroupName[GROUP_LEN];			//社团名字
	char								szNickName[NAME_LEN];			//玩家昵称
	DWORD								sdServerID;						//被锁定服务ID
	DWORD								sdKindID;						//被锁定游戏ID
	LONG								lWeekMingci;					//用户本周排名
	LONG								lWeekWinCount;					//用户本周距上一名局数

	//用户属性
	TGender								cbGender;						//用户性别
	BYTE								cbMember;						//会员等级
	BYTE								cbVisitor;						//游客属性
	int									nKeepDays;						//持续登陆天数
	int									nCanGet;						//是否能领取
	int									nTodayScore;					//今天金币
	int									nTomorrowScore;					//明天金币
	int									nRelieveCount;					//剩余领取救济金次数


	//积分信息
	SCORE								lScore;							//用户分数<by hxh>
	LONG								lStorage;
	LONG								lLostCount;						//输的盘数
	LONG								lWinCount;						//赢的盘数
	LONG								lTotalCount;					//总盘数
	LONG								lFleeCount;						//断线数目
	LONG								lExperience;					//用户经验
	LONG								lGoldEggs;						//用户金蛋
	LONG								lWeiWang;						//用户威望 
	char                               szZipCode[ZIP_CODE_LEN];        //邮政编码
	char                               szDwellingPlace[CITY_LEN];      //城市信息
	char								szDescribeString[DESCRIBE_LEN];	//描述消息

	tagGlobalUserData()
	{
		memset(this, 0, sizeof(*this));
	}
};

class CServerListMgr
{
public:
	CServerListMgr();
	~CServerListMgr();

	void sortItemByServerId(int ServerId);
	void sortItems();
	void insertItem(tagGameServer item[],unsigned int nCount);
	void removeItem(tagGameServer* pItem);
	void clearItem();

	//获取全服总人数
	DWORD getServerOnLineCount();
	// add by hxh 20161213
	GameServerList getGameServerList(unsigned int nKind);
	//获取当前服务器
	tagGameServer getCurrentServer();
	tagGameServer getGameServer(unsigned int nKind);
	tagGameServer getGameServerByServerId(int ServerId,int* pIdx);
	//获取第几个房间		add by wangjun
	tagGameServer getGameServerByIndex(int index);
private:
	GameServerList m_GameServerList;      //服务器列表
};

class CUserDateMgr
{
public:
	void insertItem(tagUserData* pItem);
	void removeItem(tagUserData* pItem);
private:
	GameUsrDateList m_GameUsrDateList;
};

//全局信息类
class CGlobalUnits
{
public:
	CGlobalUnits();
	~CGlobalUnits();
private:
	tagGlobalUserData*   m_GloblaUserData;
public:
	//CServerListMgr m_ServerListMgr;
	CUserDateMgr m_UserDateMgr;
public:
	void setGolbalUserData(tagGlobalUserData* tag);

	//获取用户信息
	tagGlobalUserData* getGolbalUserData() { return m_GloblaUserData; }

public:
	unsigned int m_nGabalUserLevel;
	unsigned int m_nOnlineCount;
	bool m_bDownVisible;

	unsigned int m_nHighVer;        //游戏客户端版本
	unsigned int m_nLowVer;
	char m_szText[128];             //描述信息
	char m_szTextTips[128];         //公告提示信息

};


class CGlobalMath
{
public:
	//获取用户头像
	static string getUserFace(unsigned int faceID,unsigned int sex)
	{
		faceID -=1;
		char buf[50];
		if (faceID <= 7 && sex == 1)        //为男
		{
			sprintf(buf, "dtgb_touxiangnan%d.png.png",faceID + 1);
			return  buf;
		}

		faceID -= 20;

		sprintf(buf, "dtgb_touxiangnv%d.png",faceID % 8 + 1);
		return buf;
	}

	//获取用户等级图片
	static string getUserLevel(int level)
	{
		std::string tmpStr;
		switch (level)
		{
		case 0:
			tmpStr = "dtgb_userlevel0.png";
			break;
		case 1:
			tmpStr = "dtgb_userlevel1.png";
			break;
		case 2:
			tmpStr = "dtgb_userlevel2.png";
			break;
		case 3:
			tmpStr = "dtgb_userlevel3.png";
			break;
		case 4:
			tmpStr = "dtgb_userlevel4.png";
			break;
		case 5:
			tmpStr = "dtgb_userlevel5.png";
			break;
		case 6:
			tmpStr = "dtgb_userlevel6.png";
			break;
		case 7:
			tmpStr = "dtgb_userlevel7.png";
			break;
		default:
			break;
		}
		return tmpStr;
	}
};

//全局信息
extern CGlobalUnits g_GlobalUnits;


//typedef ans::TaskPool<tagBuffer> TaskPool;
//extern TaskPool g_TaskPool;

#endif /* defined(__GlobalUnits__) */
