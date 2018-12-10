#ifndef GLOBAL_USER_INFO_HEAD_FILE
#define GLOBAL_USER_INFO_HEAD_FILE

#pragma once

#include "Define.h"
#include "types.h"
#include "Struct.h"
#include "CMD_LogonServer.h"

//////////////////////////////////////////////////////////////////////////////////
//结构定义

//用户信息
struct tagGlobalUserData
{
	tagGlobalUserData()
	{
		zeromemory(szUserChannel,sizeof(szUserChannel));
		zeromemory(szLogonIP,sizeof(szLogonIP));
	}
	//基本资料
	dword							dwUserID;							//用户 I D
	dword							dwGameID;							//游戏 I D
	dword							dwUserMedal;						//用户奖牌
	dword							dwExperience;						//用户经验
	dword							dwLoveLiness;						//用户魅力
	dword							dwSpreaderID;						//推广ID
	char							szAccounts[LEN_ACCOUNTS];			//登录帐号
	char							szNickName[LEN_NICKNAME];			//用户昵称
	char							szPassword[LEN_PASSWORD];			//登录密码
	char							szDynamicPass[LEN_PASSWORD];		//动态密码
	char							szLogonIP[LEN_ACCOUNTS];			//登录IP
	char							szUserChannel[LEN_NICKNAME];		//渠道号

	//用户成绩
	SCORE							lUserScore;							//用户游戏币
	SCORE							lUserInsure;						//银行游戏币
	SCORE							lUserIngot;							//用户元宝
	double							dUserBeans;							//用户游戏豆

	//扩展资料
	byte							cbGender;							//用户性别
	byte							cbMoorMachine;						//锁定机器
	char							szUnderWrite[LEN_UNDER_WRITE];		//个性签名

	//社团资料
	dword							dwGroupID;							//社团索引
	char							szGroupName[LEN_GROUP_NAME];		//社团名字

	//会员资料
	byte							cbMemberOrder;						//会员等级
	systemtime						MemberOverDate;						//到期时间

	//头像信息
	word							wFaceID;							//头像索引
	dword							dwCustomID;							//自定标识
	tagCustomFaceInfo				CustomFaceInfo;						//自定头像
	char							szHeadHttp[1024];					//http头像

	//配置信息
	byte							cbInsureEnabled;					//银行使能

	dword							dwWinCount;							//胜利盘数
	dword							dwLostCount;						//失败盘数
	dword							dwDrawCount;						//和局盘数					
	dword							dwFleeCount;						//逃跑盘数
};

//扩展资料
struct tagIndividualUserData
{
	//用户信息
	char							szUserNote[LEN_USER_NOTE];			//用户说明
	char							szCompellation[LEN_COMPELLATION];	//真实名字

	//电话号码
	char							szSeatPhone[LEN_SEAT_PHONE];		//固定电话
	char							szMobilePhone[LEN_MOBILE_PHONE];	//移动电话

	//联系资料
	char							szQQ[LEN_QQ];						//Q Q 号码
	char							szEMail[LEN_EMAIL];					//电子邮件
	char							szDwellingPlace[LEN_DWELLING_PLACE];//联系地址
};

//银行信息
// struct tagUserInsureInfo
// {
// 	word							wRevenueTake;						//税收比例
// 	word							wRevenueTransfer;					//税收比例
// 	word							wServerID;							//房间标识
// 	SCORE							lUserScore;							//用户游戏币
// 	SCORE							lUserInsure;						//银行游戏币
// 	SCORE							lUserIngot;							//用户元宝
// 	double							dUserBeans;							//用户游戏逗
// 	SCORE							lTransferPrerequisite;				//转账条件
// 	byte							cbInsureEnabled;					//银行使能条件
// };

//银行信息
struct tagUserInsureInfo
{
	word							wRevenueTake;						//税收比例
	word							wRevenueTransfer;					//税收比例
	word							wRevenueTransferMember;				//税收比例
	word							wServerID;							//房间标识
	SCORE							lUserScore;							//用户游戏币
	SCORE							lUserInsure;						//银行游戏币
	SCORE							lTransferPrerequisite;				//转帐条件
};

class GlobalUserInfoSink
{
public:
	GlobalUserInfoSink(){}
	~GlobalUserInfoSink(){}
public:
	virtual void upPlayerInfo() = 0;
	virtual void LoginSucess() = 0;
};
//////////////////////////////////////////////////////////////////////////////////

//用户信息
class CGlobalUserInfo
{
	//用户信息
protected:
	tagGlobalUserData				m_GlobalUserData;						//用户资料
	tagIndividualUserData			m_IndividualUserData;					//扩展资料
	tagUserInsureInfo				m_UserInsureInfo;						//银行资料

	tagGrowLevelParameter           m_GrowLevelParameter;					//用户等级信息

	static GlobalUserInfoSink*		m_pUserInfoSink;
	//静态变量
protected:
	static CGlobalUserInfo *		m_pGlobalUserInfo;						//用户信息

	//函数定义
public:
	//构造函数
	CGlobalUserInfo();
	//析构函数
	virtual ~CGlobalUserInfo();

	//功能函数
public:
	//重置资料
	void ResetUserInfoData();
	//用户资料
	tagGlobalUserData * GetGlobalUserData() { return &m_GlobalUserData; }
	//扩展资料
	tagIndividualUserData * GetIndividualUserData() { return &m_IndividualUserData; }
	//银行资料
	tagUserInsureInfo * GetUserInsureInfo() { return &m_UserInsureInfo; }
	//用户等级
	tagGrowLevelParameter *GetUserGrowLevelParameter(){
		return &m_GrowLevelParameter;
	}
	std::string getPhoneNumber();
public:
	static void setNickName(std::string kName);
	static void setAccounts(std::string kAccounts);
	static void setUserScore(SCORE lScore);
	static void setUserInsure(SCORE lScore);
	static dword getUserID();
	static char* getPassword();
	static byte getGender();
	static std::string getNickName();
	static std::string getHeadHttp();
	static std::string getUserChannel();
	static std::string getAccounts();
	static SCORE getUserScore();
	static SCORE getUserInsure();

	static void setSink(GlobalUserInfoSink* pSink);
	static void upPlayerInfo();
	static void LoginSucess();
	//静态函数
public:
	//获取对象
	static CGlobalUserInfo * GetInstance() { return m_pGlobalUserInfo; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif