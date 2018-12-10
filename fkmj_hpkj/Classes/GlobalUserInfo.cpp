#include "GlobalUserInfo.h"

CGlobalUserInfo	__gGlobalUserInfo;	//用户信息

//////////////////////////////////////////////////////////////////////////////////

//静态变量
CGlobalUserInfo * CGlobalUserInfo::m_pGlobalUserInfo=0;				//用户信息
GlobalUserInfoSink * CGlobalUserInfo::m_pUserInfoSink=0;				//用户信息

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGlobalUserInfo::CGlobalUserInfo()
{
	//设置对象
	ASSERT(m_pGlobalUserInfo==0);
	if (m_pGlobalUserInfo==0) m_pGlobalUserInfo=this;

	//设置变量
	zeromemory(&m_GlobalUserData,sizeof(m_GlobalUserData));
	zeromemory(&m_IndividualUserData,sizeof(m_IndividualUserData));

	return;
}

//析构函数
CGlobalUserInfo::~CGlobalUserInfo()
{
	//释放对象
	ASSERT(m_pGlobalUserInfo==this);
	if (m_pGlobalUserInfo==this) m_pGlobalUserInfo=0;

	return;
}

//重置资料
void CGlobalUserInfo::ResetUserInfoData()
{
	//设置变量
	zeromemory(&m_GlobalUserData,sizeof(m_GlobalUserData));
	zeromemory(&m_IndividualUserData,sizeof(m_IndividualUserData));

	return;
}
void CGlobalUserInfo::setUserScore(SCORE lScore)
{
	m_pGlobalUserInfo->GetGlobalUserData()->lUserScore = lScore;
	upPlayerInfo();
}
void CGlobalUserInfo::setNickName(std::string kName)
{
	strncpy(m_pGlobalUserInfo->GetGlobalUserData()->szNickName, 
		(kName.c_str()), countarray(m_pGlobalUserInfo->GetGlobalUserData()->szNickName));
	upPlayerInfo();
}
void CGlobalUserInfo::setAccounts(std::string kAccounts)
{
	strncpy(m_pGlobalUserInfo->GetGlobalUserData()->szAccounts, 
		(kAccounts.c_str()), countarray(m_pGlobalUserInfo->GetGlobalUserData()->szAccounts));
	upPlayerInfo();
}
void CGlobalUserInfo::setUserInsure(SCORE lScore)
{
	m_pGlobalUserInfo->GetGlobalUserData()->lUserInsure = lScore;
	upPlayerInfo();
}
dword CGlobalUserInfo::getUserID()
{
	return m_pGlobalUserInfo->GetGlobalUserData()->dwUserID;
}
char* CGlobalUserInfo::getPassword()
{
	return m_pGlobalUserInfo->GetGlobalUserData()->szPassword;
}
byte CGlobalUserInfo::getGender()
{
	return m_pGlobalUserInfo->GetGlobalUserData()->cbGender;
}
std::string CGlobalUserInfo::getNickName()
{
	return m_pGlobalUserInfo->GetGlobalUserData()->szNickName;
}
std::string CGlobalUserInfo::getHeadHttp()
{
	return m_pGlobalUserInfo->GetGlobalUserData()->szHeadHttp;
}
std::string CGlobalUserInfo::getUserChannel()
{
	return m_pGlobalUserInfo->GetGlobalUserData()->szUserChannel;
}
std::string CGlobalUserInfo::getAccounts()
{
	return m_pGlobalUserInfo->GetGlobalUserData()->szAccounts;
}
SCORE CGlobalUserInfo::getUserScore()
{
	return m_pGlobalUserInfo->GetGlobalUserData()->lUserScore;
}
SCORE CGlobalUserInfo::getUserInsure()
{
	return m_pGlobalUserInfo->GetGlobalUserData()->lUserInsure;
}

void CGlobalUserInfo::setSink(GlobalUserInfoSink* pSink)
{
	m_pUserInfoSink = pSink;
}
void CGlobalUserInfo::upPlayerInfo()
{
	if (m_pUserInfoSink)
	{
		m_pUserInfoSink->upPlayerInfo();
	}
}
void CGlobalUserInfo::LoginSucess()
{
	if (m_pUserInfoSink)
	{
		m_pUserInfoSink->LoginSucess();
	}
}

std::string CGlobalUserInfo::getPhoneNumber()
{
	return m_IndividualUserData.szMobilePhone;
}

//////////////////////////////////////////////////////////////////////////////////
