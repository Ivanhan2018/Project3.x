//
//  gameFrame.cpp
//  SDH
//
//  Created by zhouwei on 13-6-6.
//
//

#include "MsgDispatch.h"
#include "CMD_Game.h"
#include "GBEventIDs.h"
#include "GlobalUnits.h"
#include "CMD_Plaza.h"
#include "IDataArchive.h"
#include "GlobalUnits.h"
#include "GlobalField.h"
#include <string>
#include "Encrypt.h"
#include "GlobalFrame.h"
#include "CMD_Plaza.h"
#include "utf-8.h"
#include "ConfigMgr.h"
#include "packet.h"
#include "CMD_Commom.h"
#include "CMD_LogonServer.h"
#include "CMD_GameServer.h"
#include "platform.h"
#include "common.h"
#include "MyNSString.h"
#include "struct.h"
#include "comm.h"
#include "DBHandler.h"
#include "CocosUnits.h"
#include "HDLzma.h"
#include "MyConfig.h"//<by hxh>
#include "VersionControl.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "JniHelper.h"
#endif

#define GAMEKINDDDZ 300

#include "EntityMgr.h"

#ifdef WIN32
#include <Winsock2.h>
#endif

//#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))

time_t SystemTimeToTime_t( const SYSTEMTIME& st )

{

	tm temptm = {st.wSecond, 

		st.wMinute, 

		st.wHour, 

		st.wDay, 

		st.wMonth - 1, 

		st.wYear - 1900, 

		st.wDayOfWeek, 

		0, 

		0};



	return mktime(&temptm);

}

void MsgDispatch::GetTime(time_t &t)
{
	t = time(NULL) + m_nDeltaTime;//测试 
}

MsgDispatch::MsgDispatch()
	:m_loginState(enLogin_Server)
{
	GBEVENT_ARG_REGISTER(GBEVENT_NETWORK, MsgDispatch::onEventSocketRecv);
	m_dwRecvPacketCount = 0;
	m_wRecvSize = 0;
	m_bAnonymous = false;
	m_bRegLogin  = false;
	m_bGameStatus = false;
	m_wTouZhuSign = 1;
	memset(m_lUserLevel, 0, sizeof(m_lUserLevel));
	//add by wangjun 2014-12-10
	// œ˚œ¢◊¥Ã¨
	m_eDataType = CMD_Logon;
	m_bValidate=0;				// no need phone validation
	// ±£¥Ê”Œœ∑ID
	m_dwKindID = 200;
	//m_dwKindID = 27;
	// ∆Ù∂Ø±Í ∂
	m_bStartType = false;
	m_wBehaviorFlags = 0;

	m_pAccounts = "";
	m_pPassword = "";
	m_pBankWord = "";
	m_pNickname = "";
	m_pPhoneNum = "";

	m_dwUserID		= 0;
	m_wTableID		= 0;
	m_wChairID		= 0;
	m_wLastServerID	= 0;
	m_wLastKindID	= 0;
	m_dwGameID		= 0;
	m_cbStatus		= US_NULL;
	m_dwGoldScore   = 0;

	m_pClientLoginSock = new ClientSock();
	m_pClientGameSock  = new ClientSock();
	memset(szDescribeString, 0, sizeof(szDescribeString));
	memset(m_wArticleNum, 0, sizeof(m_wArticleNum));
	current_time_t = 0;
	m_nDeltaTime = 0;

	m_loseConTimes = 0;
	m_isLogin = 0;
	
	m_nSendGameOption = 0;
	//默认无连接
	netWorkType = 0;
	//初始化网络状态
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	netWorkType = 1;
#endif

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	netWorkType = getNetworkType();
#endif

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	Reachability *myConn = [Reachability reachabilityWithHostName:@"www"];//任何字符串都可以
	switch([myConn currentReachabilityStatus])
	{
	case NotReachable:
		netWorkType = 0;
		break;
	case ReachableViaWiFi:
		netWorkType = 1;
		break;
	case ReachableViaWWAN:
		netWorkType = 2;
		break;
	}
#endif
}
MsgDispatch::~MsgDispatch()
{	
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(MsgDispatch::update),this);

	CC_SAFE_DELETE(m_pClientLoginSock);
	CC_SAFE_DELETE(m_pClientGameSock);
}

void MsgDispatch::startSchedule()
{
	Director::getInstance()->getScheduler()->schedule(schedule_selector(MsgDispatch::update),this,0.0f,false);
}


void MsgDispatch::pauseSchedule()
{
	Director::getInstance()->getScheduler()->pauseTarget(this);
}

void MsgDispatch::resumeSchedule()
{
	Director::getInstance()->getScheduler()->resumeTarget(this);
}

bool MsgDispatch::init()
{	
	return true;
}
void MsgDispatch::update(float dt)
{	
	static float delta = 0;//发送心跳包
	static float send_delta = 0;//验证心跳包
	static float low_delta = 0;//低
	static int re_times = 0;//连接次数
	static int low_sigal = 0;//低频次数
	static bool isRun = false;

	if (m_loginState == enLogin_Server)
	{
		m_pClientLoginSock->update();	

		//登陆界面不检测
		if (m_isLogin )
		{
			delta = 0;//发送心跳包
			send_delta = 0;//验证心跳包
			low_delta = 0;//低
			re_times = 0;//连接次数
			low_sigal = 0;//低频次数
			return; 
		}

		delta += dt;
		send_delta += dt;
		low_delta += dt;

		//低频
		if(low_delta > 32)
		{
			low_delta = 0;
			low_sigal = 0;
		}

		//保证每秒发送一次心跳包
		if (send_delta > 1.5f)
		{			
			send_delta = 0;			
			SendPacketWithGetSystemTime();
		
			//只有当收不到心跳包的时候才去检测网络 不应该
			int hasNetwork = MyNSString::getNetworkType();
			//如果是网络环境发生改变，则需重连
			if(hasNetwork != 0 && hasNetwork != netWorkType && netWorkType != 0)
			{
				netWorkType = hasNetwork;				
				NotificationCenter::getInstance()->postNotification("NetWorkChange");	
				//重新连接
				closeLoginSocket();	
				SendReConnect(0);	
				return;
			}

			if (re_times > 2 || hasNetwork == 0)
			{
				re_times = 0;						
				//返回登陆				
				NotificationCenter::getInstance()->postNotification("BackToLogin",Integer::create(0));	
				return;
			} 
		}

		//每10秒检测一次需要4次
		if(!isRun && delta > 6.0f)
		{
			isRun = true;
			delta = 0;

			//大于0则不需要重连
			if(m_loseConTimes > 1)
			{	
				re_times = 0;
				//网络波动较大的情况在这里处理
				if(m_loseConTimes < 3)  //m_loseConTimes = 1 的时候
				{
					//不完全收到包						
					if(low_sigal > 2)  
					{
						low_sigal = 0;						
						NotificationCenter::getInstance()->postNotification("BackToLogin",Integer::create(1));	//1为网络不稳
					}else
					if(low_sigal > 1)
					{
						//重新连接
						closeLoginSocket();	
						SendReConnect(2);
					}

					low_sigal ++;
				} else
				{
					low_sigal = 0;
					//全部都收到了					
				}				
			} else
			{						
				//重新连接 静默
				closeLoginSocket();	
				SendReConnect(re_times ++);						
			}
			//每次检查完清空一次 重置 清零，即收到为0个 
			m_loseConTimes = 0;	
			isRun = false;
		}
	}else
	{
		delta = 0;//发送心跳包
		send_delta = 0;//验证心跳包
		low_delta = 0;//低
		re_times = 0;//连接次数
		low_sigal = 0;//低频次数

		m_pClientGameSock->update();
	}
}

void MsgDispatch::resetConTimes()
{
	//重置 成功接收则++
	m_loseConTimes ++;

	//CCLOG("==============================================  %d============", m_loseConTimes);
}

void MsgDispatch::setIsLogin(bool isLogin) 
{
	if(isLogin) m_isLogin++;
	else m_isLogin--;
}

void MsgDispatch::setGameStatus(bool bStatus)
{
	m_bGameStatus = bStatus;
}

void MsgDispatch::setGameKindId(int gameId)
{
	this->m_dwKindID = gameId;
}

void MsgDispatch::setServerId(int serverId)
{
	this->m_dwGameID = serverId;
}

void MsgDispatch::onEventSocketRecv(GBEventConstArg& arg)
{
	tagBuffer& tag = (tagBuffer&)arg;
	memmove(m_cbRecvBuff + m_wRecvSize, tag.dateBuffer, 81920 - m_wRecvSize);

	m_wRecvSize += tag.nDataSize;

	//±‰¡ø∂®“Â
	WORD wPacketSize=0;
	BYTE cbDataBuffer[SOCKET_PACKET+sizeof(CMD_Head)];
	memset(cbDataBuffer, 0, SOCKET_PACKET+sizeof(CMD_Head));
	CMD_Head * pHead=(CMD_Head*)m_cbRecvBuff;
	while (m_wRecvSize>=sizeof(CMD_Head))
	{
		//–ß—È≤Œ ˝
		wPacketSize = pHead->CmdInfo.wPacketSize;

		if (wPacketSize < sizeof(CMD_Head))
		{
			CCLOG("data error,small than head data");
			return;
		}

		if (pHead->CmdInfo.cbVersion != SOCKET_VER)
		{
			CCLOG("version error");
			//错误的头清空
			m_dwRecvPacketCount = 0;
			m_wRecvSize = 0;
			memset(m_cbRecvBuff, 0, sizeof(m_cbRecvBuff));
			return;
		}

		if (m_wRecvSize < wPacketSize)
		{		
			//这里较验失败直接舍弃			
 			return;
		}

		//øΩ±¥ ˝æ›
		m_dwRecvPacketCount++;

		memcpy(cbDataBuffer,m_cbRecvBuff,wPacketSize);
		m_wRecvSize-=wPacketSize;
		memmove(m_cbRecvBuff,m_cbRecvBuff + wPacketSize,m_wRecvSize);

		WORD wRealySize = wPacketSize;
		if (m_loginState == enLogin_Server)
		{
			//Ω‚√‹ ˝æ›
#ifdef VER_369
			wRealySize=m_pClientLoginSock->crevasseBuffer(cbDataBuffer,wPacketSize);
#else
			if(!Common_unMappedBuffer(cbDataBuffer,wPacketSize)){
				CCLOG("Common_unMappedBuffer FAILED");
			}
#endif
		}else
		{
			//Ω‚√‹ ˝æ›
#ifdef VER_369
			wRealySize=m_pClientGameSock->crevasseBuffer(cbDataBuffer,wPacketSize);
#else
			if(!Common_unMappedBuffer(cbDataBuffer,wPacketSize)){
				CCLOG("Common_unMappedBuffer FAILED");
			}
#endif
		}

		//Ω‚ Õ ˝æ›
		WORD wDataSize=wRealySize-sizeof(CMD_Head);
		void * pDataBuffer=cbDataBuffer + sizeof(CMD_Head);
		CMD_Command Command=((CMD_Head*)cbDataBuffer)->CommandInfo;

		if(Command.wMainCmdID == 0 &&Command.wSubCmdID == 1)
		{
			//出错
			//if(m_pClientLoginSock->getIsNeedConnect())
			//{
			//	m_pClientLoginSock->reconserver();
			//}
			return;
		}

		if (Command.wMainCmdID == MDM_KN_COMMAND)
		{
			switch (Command.wSubCmdID)
			{
			case SUB_KN_DETECT_SOCKET:	//Õ¯¬ÁºÏ≤‚
				{
					//∑¢ÀÕ ˝æ›

					if (m_loginState == enLogin_Server)
					{
						SendPacketWithCompilatio(0);
					}else
					{
						SendPacketWithCompilatio(1);
					}
					break;
				}
			}
			continue;
		}
		if (Command.wMainCmdID == 3 && Command.wSubCmdID == 503)
		{//不打印日志
			
		}
		else
		{//不是时间查询返回数据，打印日志  （心跳）
			CCLOG("xin::recv msg ::::  wMainCmdID = %d,wSubCmdID = %d",Command.wMainCmdID,Command.wSubCmdID);

			CCLOG("m_eDataType = %d", m_eDataType);
		}
		
		if (m_eDataType == CMD_Game)
		{
			OnEventTcpGameRead(0, &Command, pDataBuffer, wDataSize);
		}else if(m_eDataType == CMD_Room)
		{
			OnEventTcpRoomRead(0, &Command, pDataBuffer, wDataSize);
		}else{
			onEventTcpLoginRead(0, &Command, pDataBuffer, wDataSize);
		}
	}
}

void MsgDispatch::setTcpSink(BaseObject* obj)
{

}

//…Ë÷√µ«¬Ω◊¥Ã¨
void MsgDispatch::setLoginState(enLoginState state)
{
	m_loginState = state;
}


bool MsgDispatch::onEventTcpLoginRead(WORD wSocketId,CMD_Command* pCommand,void* pData,WORD wDataSize)
{
	/*switch (pCommand->wMainCmdID)
	{
	case MDM_GP_LOGON:          //µ«¬Ωœ˚œ¢
	return onSocketMainLogin(pCommand,pData,wDataSize);

	case MDM_GP_SERVER_LIST:    //¡–±Ìœ˚œ¢
	return onSocketMainServerList(pCommand,pData,wDataSize);

	case MDM_GP_SYSTEM:         //œµÕ≥œ˚œ¢
	return onSocketMainSystem(pCommand, pData,wDataSize);

	case MDM_GP_USER:           //”√ªßœ˚œ¢
	return onSocketMainUser(pCommand, pData,wDataSize);
	default:
	break;
	}*/


	switch (pCommand->wMainCmdID)
	{
	case MDM_GP_LOGON:          //µ«¬Ωœ˚œ¢ 100
		{
			switch (pCommand->wSubCmdID)
			{
			case SUB_MB_LOGON_SUCCESS:			// µ«¬º≥…π¶	100
				{
					return this->OnSocketSubGPLogonSuccess(pData, wDataSize);
				}
			case SUB_MB_LOGON_FAILURE:			// µ«¬º ß∞‹	101
				{
					return this->OnSocketSubGPLogonFailure(pData, wDataSize);
				}
			default:
				return true;
			}
		}
//	case MDM_MB_LOGON:          //µ«¬Ωœ˚œ¢ 100
//		return OnTCPNetworkMainMBLogon(pCommand->wSubCmdID,pData,wDataSize);
	case MDM_MB_SERVER_LIST:    //¡–±Ìœ˚œ¢ 101
		return OnTCPNetworkMainMBServerList(pCommand->wSubCmdID,pData,wDataSize);
	case MDM_GP_SERVER_LIST:
		return OnTCPNetworkMainMBServerList(pCommand->wSubCmdID,pData,wDataSize);
	case MDM_MB_USER_SERVICE:           //”√ªß∑˛ŒÒ 102
		return OnTCPNetworkMainMBIphoneUserService(pCommand->wSubCmdID, pData,wDataSize);
	case MDM_KN_COMMAND:				// 0
		return true;
		//case SUB_KN_DETECT_SOCKET:          // 1
		//	return false;
	case MDM_GP_USER_SERVICE:          // 3
		return onSocketMainUser(pCommand, pData,wDataSize);
	default:
		//CCAssert(false, "¥ÛÃ¸:√ª”–¥¶¿Ìµƒcase");
		return false;
	}
	return true;
}

//-----------÷˜√¸¡Ó¬Î∑÷¿‡-------------------
//-------------¥ÛÃ¸µ«¬º--------------
//µ«¬ºœ˚œ¢		add by wangjun 2014-12-10
//bool MsgDispatch::OnTCPNetworkMainMBLogon(int wSubCmdID, void *buffer, WORD size)
//{
//	//CCLOG("OnTCPNetworkMainMBLogon wSubCmdID=%d", wSubCmdID);
//	switch (wSubCmdID)
//	{
//	case SUB_MB_LOGON_SUCCESS:			// µ«¬º≥…π¶	100
//		{
//			return this->OnSocketSubLogonSuccess(buffer, size);
//		}
//	case SUB_MB_LOGON_FAILURE:			// µ«¬º ß∞‹	101
//		{
//			return this->OnSocketSubLogonFailure(buffer, size);
//		}
//		//      case SUB_GR_LOGON_NOTIFY:			// µ«¬ΩÃ· æTIPS	104
//		//		{
//		//            return [self OnSocketSubGRLogonNotify:buffer bufferSize:size];
//		//		}
//	default:
//		//CCAssert(false, "µ«¬º¥¶¿Ì:≥¨≥ˆ∑∂Œß");
//		return true;
//	}
//	return true;
//}

//¡–±Ìœ˚œ¢		add by wangjun 2014-12-10
bool MsgDispatch::OnTCPNetworkMainMBServerList(int wSubCmdID, void *buffer, WORD size)
{
	switch (wSubCmdID)
	{
		//        case SUB_MB_LIST_KIND:				// ÷÷¿‡¡–±Ì	100
		//        {
		//            return this->OnSocketSubListKind(buffer, size);
		//        }
		//        case SUB_MB_LIST_SERVER:			// ∑øº‰¡–±Ì	101
		//        {
		//            return this->OnSocketSubListServer(buffer, size);
		//        }
		//        case 102:
		//        {
		//            return this->OnSocketSubListFinish(buffer, size);
		//        }
		//        case 103:
		//        {
		//            return this->OnSocketSubListFinish(buffer, size);
		//        }
		//        case SUB_MB_LIST_FINISH:			// ¡–±ÌÕÍ≥…	200
		//        {
		//            return this->OnSocketSubListFinish(buffer, size);
		//        }

	case SUB_MB_LIST_KIND:				// ÷÷¿‡¡–±Ì	100
		{
			return this->OnSocketSubListKind(buffer, size);
		}
	case SUB_MB_LIST_SERVER:			// ∑øº‰¡–±Ì	101
		{
			return this->OnSocketSubListServer(buffer, size);
		}
	case SUB_MB_LIST_FINISH:			// ¡–±ÌÕÍ≥…	200
		{
			return this->OnSocketSubListFinish(buffer, size);
		}
	case SUB_MB_LIST_LAST_GAME_SERVER:	// 104
		{
			return this->OnSocketSubListLastGameServer(buffer, size);
		}
	}
	return true;
}

//∑˛ŒÒœ˚œ¢		add by wangjun 2014-12-10
bool MsgDispatch::OnTCPNetworkMainMBIphoneUserService(int wSubCmdID, void *buffer, WORD size)
{
	return true;
}

//bool MsgDispatch::OnSocketSubLogonSuccess(void *buffer, WORD size)
//{
//	CCLOG("OnSocketSubLogonSuccess");
//
//	// –ß—È¥Û–°
//	assert(size == sizeof(CMD_MB_LogonSuccess));
//	if (size != sizeof(CMD_MB_LogonSuccess)) return false;
//
//	// ±‰¡ø∂®“Â
//	BYTE cbBuffer[sizeof(CMD_MB_LogonSuccess)];
//	memset(cbBuffer, 0 , sizeof(CMD_MB_LogonSuccess));
//	CMD_MB_LogonSuccess *logonSuccess = (CMD_MB_LogonSuccess *)cbBuffer;
//	memcpy(logonSuccess, buffer, sizeof(CMD_MB_LogonSuccess));
//
//	// µ«¬ºÍ«≥∆
//	char nicknameArray[164]={0};
//	MyNSString::wc2mbs(logonSuccess->szNickName, sizeof(logonSuccess->szNickName), nicknameArray);
//	m_pNickname = string(nicknameArray);
//
//	// º«¬º∏ˆ»À–≈œ¢
//	m_cbGender = logonSuccess->cbGender;
//	m_wFaceID = logonSuccess->wFaceID;
//	m_dwUserID = logonSuccess->dwUserID;
//	m_dwGoldScore = logonSuccess->f_yue;    // ”√ªß”µ”–µƒ∂π∂π
//	m_dwExperience = logonSuccess->dwExperience;
//
//	//±£¥Ê–≈œ¢
//	tagGlobalUserData* pUserData = new tagGlobalUserData;
//	memset(pUserData, 0, sizeof(tagGlobalUserData));
//	memcpy(pUserData->szNickName, nicknameArray, sizeof(pUserData->szNickName));
//	//MyNSString::wc2mbs(logonSuccess->szNickName, sizeof(logonSuccess->szNickName), pUserData->szNickName);
//	pUserData->cbGender=logonSuccess->cbGender;
//	pUserData->wFaceID=logonSuccess->wFaceID;
//	pUserData->dwUserID=logonSuccess->dwUserID;
//	pUserData->lScore=logonSuccess->f_yue;    // ”√ªß”µ”–µƒ∂π∂π
//	pUserData->lExperience=logonSuccess->dwExperience;
//	g_GlobalUnits.setGolbalUserData(pUserData);
//	CCLOG("wFaceID:%d",pUserData->wFaceID);
//
//	CCLOG("login success2! file(%s) line(%d)",__FILE__, __LINE__);
//
//
//
//	//EntityMgr::instance()->getDispatch()->closeLoginSocket();
//
//	UserDefault::getInstance()->setStringForKey("NEWZJD_ACCOUNT", m_szAccount);
//	UserDefault::getInstance()->setStringForKey("NEWZJD_PASSWORD", m_szPassword);
//
//
//
//	//Œ™◊¢≤·µ«¬Ω
//	if ( EntityMgr::instance()->getDispatch()->isRegLogin())
//	{
//		CCLOG("register success! file(%s) line(%d)",__FILE__, __LINE__);
//
//		int count = UserDefault::getInstance()->getIntegerForKey("KEY_ACCOUNT_COUNT") + 1;
//		UserDefault::getInstance()->setIntegerForKey("KEY_ACCOUNT_COUNT", count);
//
//		EntityMgr::instance()->getDispatch()->setRegLogin(false);
//		//GBEvent::instance()->fire(GBEVENT_UI_REGISTERSUCC);
//
//		NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_REGSUCC);
//		//this->SendPacketWithQueryBankInfo();
//		return true;
//	}
//
//	CCLOG("login success! file(%s) line(%d)",__FILE__, __LINE__);
//	return true;
//}

bool MsgDispatch::OnSocketSubGPLogonSuccess(void *buffer, WORD size)
{
	CCLOG("OnSocketSubLogonSuccess");

	// –ß—È¥Û–°
	//assert(size == sizeof(CMD_GP_LogonSuccess));
	//if (size != sizeof(CMD_GP_LogonSuccess)) return false;

	// ±‰¡ø∂®“Â
	BYTE cbBuffer[sizeof(CMD_GP_LogonSuccess)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_LogonSuccess));
	CMD_GP_LogonSuccess *logonSuccess = (CMD_GP_LogonSuccess *)cbBuffer;
	memcpy(logonSuccess, buffer, sizeof(CMD_GP_LogonSuccess));

	// 用户妮称和用户名 妮称改成手机号码
	char nicknameArray[1000]={0};
	MyNSString::wc2mbs(logonSuccess->szPhoneNum, sizeof(logonSuccess->szPhoneNum), nicknameArray);
	m_pPhoneNum = string(nicknameArray);
	char accountArray[1000] = {0};
	MyNSString::wc2mbs(logonSuccess->szAccounts, sizeof(logonSuccess->szAccounts), accountArray);
	m_szAccount = string(accountArray);

	//新加验证
	m_bValidate = logonSuccess->cIsYanZheng;

	//全局变量
	m_cbGender = logonSuccess->cbGender;
	m_wFaceID = logonSuccess->wFaceID;
	m_dwUserID = logonSuccess->dwUserID;
	m_dwGoldScore = logonSuccess->f_yue;    // ”√ªß”µ”–µƒ∂π∂π
	m_dwExperience = logonSuccess->dwExperience;
	m_cbType = logonSuccess->n_type;
	f_fandian = logonSuccess->f_fandian;

	f_qipai_yue = logonSuccess->lScore;
	f_yue = logonSuccess->f_yue;
	//新的
	tagGlobalUserData* pUserData = new tagGlobalUserData;
	memset(pUserData, 0, sizeof(tagGlobalUserData));
	memcpy(pUserData->szNickName, accountArray, sizeof(pUserData->szNickName));  //用户名就是妮称
	memcpy(pUserData->szAccounts, accountArray, sizeof(pUserData->szAccounts));

	pUserData->cbGender=logonSuccess->cbGender;
	pUserData->wFaceID=logonSuccess->wFaceID;
	pUserData->dwUserID=logonSuccess->dwUserID;
	pUserData->lScore=logonSuccess->f_yue;    // ”√ªß”µ”–µƒ∂π∂π
	pUserData->lScore=logonSuccess->lScore;    // 棋牌余额<by hxh>
	pUserData->lExperience=logonSuccess->dwExperience;
	g_GlobalUnits.setGolbalUserData(pUserData);
	CCLOG("wFaceID:%d",pUserData->wFaceID);

	CCLOG("login success2! file(%s) line(%d)",__FILE__, __LINE__);



	//EntityMgr::instance()->getDispatch()->closeLoginSocket();

	UserDefault::getInstance()->setStringForKey("NEWZJD_ACCOUNT", m_szAccount);
	bool isSave = UserDefault::getInstance()->getBoolForKey("SAVE_PSW",false);
	if(isSave)
	{	
		UserDefault::getInstance()->setStringForKey("NEWZJD_PASSWORD", m_szPassword);
	} else
	{
		UserDefault::getInstance()->setStringForKey("NEWZJD_PASSWORD", "");
	}



	//Œ™◊¢≤·µ«¬Ω
	if ( EntityMgr::instance()->getDispatch()->isRegLogin())
	{
	//	return true;
		CCLOG("register success! file(%s) line(%d)",__FILE__, __LINE__);

		int count = UserDefault::getInstance()->getIntegerForKey("KEY_ACCOUNT_COUNT") + 1;
		UserDefault::getInstance()->setIntegerForKey("KEY_ACCOUNT_COUNT", count);

		EntityMgr::instance()->getDispatch()->setRegLogin(false);
		//GBEvent::instance()->fire(GBEVENT_UI_REGISTERSUCC);

		NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_REGSUCC);
		//this->SendPacketWithQueryBankInfo();
		return true;
	}
	CCLOG("login success! file(%s) line(%d)",__FILE__, __LINE__);
	return true;
}

//µ«¬º ß∞‹      add by wangjun 2014-12-10
//bool MsgDispatch::OnSocketSubLogonFailure(void *buffer, WORD size)
//{
//	CCLOG("OnSocketSubLogonFailure");
//	EntityMgr::instance()->getDispatch()->closeLoginSocket();
//
//	//–ß—È≤Œ ˝
//	CMD_MB_LogonFailure* pLogonError = (CMD_MB_LogonFailure *)buffer;
//
//	NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_LOGINFAIL);
//	return true;
//}

bool MsgDispatch::OnSocketSubGPLogonFailure(void *buffer, WORD size)
{
	CCLOG("OnSocketSubLogonFailure");
	EntityMgr::instance()->getDispatch()->closeLoginSocket();

	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_MB_LogonFailure* info = (CMD_MB_LogonFailure *)cbBuffer;		
	memcpy(info, buffer, size);

	Dictionary* dic = Dictionary::create();
	dic->setObject(Integer::create(info->lErrorCode),"lErrorCode");

	char szDescribeString[500] = {0};
	MyNSString::wc2mbs(info->szDescribeString, sizeof(info->szDescribeString), szDescribeString);
	dic->setObject(String::create(szDescribeString),"szDescribeString");	

	NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_LOGINFAIL,dic);
	return true;
}

//---------------------¡–±Ì¥¶¿Ì◊”√¸¡Ó¬Î¥¶¿Ì--------------------------
//¡–±ÌªÒ»°      add by wangjun 2014-12-10
bool MsgDispatch::OnSocketSubListKind(void *buffer, WORD size)
{
	CCLOG("列表获取");
	return this->OnSocketWithCaiZhongList(buffer, size);
	//return true;
}

//∑øº‰¡–±Ì      add by wangjun 2014-12-10
bool MsgDispatch::OnSocketSubListServer(void *buffer, WORD size)
{
	// loaing进度提示 这里暂时不加


	// ±‰¡ø∂®“Â
	int structSize = sizeof(tagGameServer);

	/*********************************************************************************
	* important 游戏房间数永运等于3，如果server.wSortID相同，则认为在同一个房间有两个入口，
	* 把它保存在数据，快速加入时，搜寻这个数据，保证玩家能登录。
	**********************************************************************************/
	int count = size/structSize;
	int index = 0;

	//这是之前保存数据的地方，先留着
	BYTE cbBuffer[20480];
	memset(cbBuffer, 0 , 20480);
	tagGameServer* pGameServer = (tagGameServer*)cbBuffer;
	memcpy(pGameServer, buffer, size);
	unsigned int nItemCount = count;
	g_GlobalUnits.m_nOnlineCount = nItemCount;
	NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_REFRESHTOTALNUM);
	m_ServerListMgr.clearItem();
	m_ServerListMgr.insertItem(pGameServer,nItemCount);
	m_ServerListMgr.sortItems();
	//ios牛牛中保存数据的方法

	/*
	CCLOG("游戏房间数 count=%d, 可使用服务器房间总数=%d", count, size/structSize);

	Array *pageRoom = Array::create();
	Array *roomArray = NULL;
	CFrameOneRoom* pRoomData[3];
	for (int i = 0; i < count; i++)
	pRoomData[i] = NULL;

	// 生成游戏房间列表  顺序为 普通房、初级房、PK房 
	for (int i = 0; i < count; i++)
	{
	if (index%3 == 0)
	{
	roomArray = Array::create();
	pageRoom->addObject(roomArray);
	}
	tagGameServer server;
	memset(&server, 0, sizeof(tagGameServer));
	memcpy(&server, ((tagGameServer *)buffer+i), structSize);
	char tempServerName[64];
	MyNSString::wc2mbs(server.szServerName, sizeof(server.szServerName), tempServerName);
	CCLOG("lBeginScore=%lld, lMinScore=%lld server.wSortID=%d, roomName=%s", server.lServerScore , server.lMinServerScore, server.wSortID, tempServerName);


	if (count == 3)
	{
	if (server.wSortID == 1)
	{   // 初级房
	pRoomData[1] = COneRoom::ViewWithOneRoomView(&server, 1%m_pFrameDate.m_nRoomCount);
	}
	else if (server.wSortID == 2)
	{
	// 普通房
	pRoomData[0] = COneRoom::ViewWithOneRoomView(&server, 0%m_pFrameDate.m_nRoomCount);
	}
	else if (server.wSortID == 3)
	{
	// PK房
	pRoomData[2] = COneRoom::ViewWithOneRoomView(&server, 2%m_pFrameDate.m_nRoomCount);
	}
	}
	else if (count == 1)
	{
	// 只有一个房间
	pRoomData[0] = COneRoom::ViewWithOneRoomView(&server, 0%m_pFrameDate.m_nRoomCount);
	}
	else if (count == 2)
	{
	if (server.wSortID == 1)
	{
	pRoomData[1] = COneRoom::ViewWithOneRoomView(&server, 1%m_pFrameDate.m_nRoomCount);
	}
	else
	{
	pRoomData[0] = COneRoom::ViewWithOneRoomView(&server, 2%m_pFrameDate.m_nRoomCount);
	}
	}

	index++;
	}

	for (int i = 0; i < count; i++)
	[roomArray addObject:pRoomData[i]];

	//NSLog(@"pageRoom string=%@", pageRoom);

	// 设置列表key
	[m_pPlazaDic setObject:pageRoom forKey:@"roomlist"];
	*/
	return true;
}

//列表完成      add by wangjun 2014-12-10
bool MsgDispatch::OnSocketSubListFinish(void *buffer, WORD size)
{
	CCLOG("OnSocketSubListFinish");

	// 拷贝房间信息，并刷新显示
	NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_REFRESHTOTALNUM, NULL);
	GBEvent::instance()->fire(GBEVENT_UI_LOGINFINISH);
	NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_LOGINFINISH);
	/*
	// ºÏ≤‚∫Ø ˝
	if([m_pPlaza respondsToSelector:@selector(FrameDateDelegateWithListFinish:size:)])
	[m_pPlaza FrameDateDelegateWithListFinish:buffer size:size];
	else
	NSAssert(0,@"(FrameDateDelegateWithListFinish:size:)");

	// 标识数据状态
	m_eDataType = CMD_Room;

	// 关闭当前连接
	[m_pStream ClosedStream];     // modi by mojunxin   important
	*/
	return true;
}

//被卡游戏房间  add by hxh 2016-11-23
bool MsgDispatch::OnSocketSubListLastGameServer(void *buffer, WORD size)
{
	CMD_GP_LastGameServer* pRsp = (CMD_GP_LastGameServer*)buffer;
	m_wLastServerID=pRsp->wServerID;
	if(m_wLastServerID>0)
	{
		int idx=-1;
		tagGameServer server=m_ServerListMgr.getGameServerByServerId(pRsp->wServerID,&idx);
		if(idx>-1)
		{
			m_wLastKindID=server.wKindID;
			m_ServerListMgr.sortItemByServerId(pRsp->wServerID);
		}
	}
	return true;
}

//登陆房间信息
bool  MsgDispatch::OnEventTcpRoomRead(WORD wSocketId,CMD_Command* pCommand,void* pData,WORD wDataSize)
{
	dispEntityMsg(pCommand,pData,wDataSize,GBEVENT_NETWORK_ROOM);
	return true;
}

//游戏服务消息
bool  MsgDispatch::OnEventTcpGameRead(WORD wSocketId,CMD_Command* pCommand,void* pData,WORD wDataSize)
{
	dispEntityMsg(pCommand,pData,wDataSize,GBEVENT_NETWORK_GAME);
	return true;
}

//登录消息
bool MsgDispatch::onSocketMainLogin(CMD_Command* pCommand, void * pBuffer, WORD wDataSize)
{
	dispEntityMsg(pCommand,pBuffer,wDataSize,GBEVENT_NETWORK_LOGIN);

	return true;
}

void MsgDispatch::dispEntityMsg(CMD_Command* pCommand, void * pBuffer, WORD wDataSize,GBEVENT_ID eid)
{
	tagCmdBuffer tag;
	memset(tag.dateBuffer, 0, 40960);

	tag.nDataSize = wDataSize;
	memcpy(tag.dateBuffer, pBuffer,wDataSize);
	memcpy(&tag.command, pCommand,sizeof(CMD_Command));

	GBEvent::instance()->fire(eid,tag);
}
//列表消息
bool MsgDispatch::onSocketMainServerList(CMD_Command* pCommand, void * pBuffer, WORD wDataSize)
{
	switch (pCommand->wSubCmdID)
	{
	case SUB_GP_LIST_TYPE:			//类型信息
		{
			CCLOG("get game list type! file(%s) line(%d)",__FILE__, __LINE__);
			return true;
		}
	case SUB_GP_LIST_KIND:			//种类消息
		{
			CCLOG("get game list kind! file(%s) line(%d)",__FILE__, __LINE__);
			return true;
		}
	case SUB_GP_LIST_STATION:		//站点消息
		{
			CCLOG("get game list station! file(%s) line(%d)",__FILE__, __LINE__);
			return true;
		}
	case SUB_GP_LIST_SERVER:		//创建服务器房间列表
		{
			//效验参数
			if (wDataSize % sizeof(tagGameServer) != 0)
			{
				return false;
			}


			//获取服务器列表，添加到服务器管理控制类，同时获取房间在线人数
			//tagGameServer* pGameServer = new tagGameServer;

			tagGameServer* pGameServer = (tagGameServer*)pBuffer;
			unsigned int nItemCount = wDataSize / sizeof(tagGameServer);

			g_GlobalUnits.m_nOnlineCount = nItemCount;

			NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_REFRESHTOTALNUM);

			m_ServerListMgr.clearItem();
			m_ServerListMgr.insertItem(pGameServer,nItemCount);
			CCLOG("get game list server! file(%s) line(%d)",__FILE__, __LINE__);
			return true;
		}
	case SUB_GP_LIST_FINISH:		//登录服务器列表发送完成
		{
			//统计游戏总人数			
			return true;
		}
	case SUB_GP_LIST_CONFIG:		//列表配置
		{
			assert(wDataSize == sizeof(CMD_GP_ListConfig));
			if (wDataSize != sizeof(CMD_GP_ListConfig))
			{
				return false;
			}
			CMD_GP_ListConfig *pListConfig = (CMD_GP_ListConfig *)pBuffer;
			// 破产标准
			ConfigMgr::instance()->m_Config.m_nGolbalBankruptcy = pListConfig->nBankruptcy;
			memcpy(ConfigMgr::instance()->m_Config.szTitle, pListConfig->szTitle, sizeof(ConfigMgr::instance()->m_Config.szTitle));
			memcpy(ConfigMgr::instance()->m_Config.szContent, pListConfig->szContent, sizeof(ConfigMgr::instance()->m_Config.szContent));

			//memcpy(TaskInfo::ShareTaskInfo()->m_nTaskConfig, pListConfig->nTaskData, sizeof(pListConfig->nTaskData));

			// 拷贝房间信息，并刷新显示
			memcpy(ConfigMgr::instance()->m_Config.nServerEnter, pListConfig->nRoomEnter, sizeof(pListConfig->nRoomEnter));
			NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_REFRESHTOTALNUM, NULL);
			return true;
		}

		// 获取游戏服地址
	case SUB_GP_QUICK_GAMESERVER_ADR:
		{
			EntityMgr::instance()->getDispatch()->closeLoginSocket();

			assert(wDataSize == (sizeof(DWORD)<<1));
			if (wDataSize != (sizeof(DWORD)<<1))
			{
				return false;
			}

			long *lpValue = (long *)pBuffer;

			if (0 == lpValue[0] || 0 == lpValue[1])
			{
				// 没有可以连接的服务器
				NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_GAMELINK, Integer::create(2));
			}
			else
			{
				bool ret = EntityMgr::instance()->getDispatch()->connectGameServerAdr(lpValue[0], lpValue[1]);			//0为连接失败 1为连接成功  2为没有服务器			
				NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_GAMELINK,Integer::create(ret?1:0));
			}

			break;
		}
	}

	return true;
}
//œµÕ≥œ˚œ¢
bool MsgDispatch::onSocketMainSystem(CMD_Command* pCommand, void * pBuffer, WORD wDataSize)
{
	return true;
}
//用户消息
bool MsgDispatch::onSocketMainUser(CMD_Command* pCommand, void * pBuffer, WORD wDataSize)
{
	switch (pCommand->wSubCmdID)
	{
	case SUB_GP_OPERATE_SUCCESS:
		{
			//closeLoginSocket();
			CMD_GP_OperateSuccess *operateSuccess = (CMD_GP_OperateSuccess *)pBuffer;
			if(userServiceSubCmd == SUB_GP_MODIFY_LOGON_PASS)
			{
				if(operateSuccess->lResultCode == 0)
				{
					int result = 0;
					Integer* pPasswordResult = Integer::create(result);
					NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_PASSWORDUPDATE,pPasswordResult);
				}
			}
			break;
		}
	case SUB_GP_OPERATE_FAILURE:
		{
			break;
		}
	case SUB_GP_QUERY_INSURE_INFO:
		{
			break;
		}
	case SUB_GP_USER_INSURE_INFO:					// 保险柜资料     403
		return this->OnSocketWithInsureInfo(pBuffer, wDataSize);
	case SUB_GP_USER_INSURE_SUCCESS:				// 操作成功     405
		return this->OnSocketWithBankSucceed(pBuffer, wDataSize);
	case SUB_GP_USER_INSURE_FAILURE:				// 操作失败     406
		return this->OnSocketWithBankFailure(pBuffer, wDataSize);
		//case SUB_GP_SEARCH_GOODS_INFO_RET:				//     412
		//	return this->OnSocketWithSearchGoods(pBuffer, wDataSize);
		//case SUB_GR_BUY_GOODS_INFO_RET:				// π∫¬Ú∑µªÿ     416
		//	return this->OnSocketWithBuyGoods(pBuffer, wDataSize);
		//        case SUB_GR_GIVE_GOODS_INFO_RET:				// ‘˘ÀÕ∑µªÿ     421
		//            return this->OnSocketWithGiveGoods(pBuffer, wDataSize);
	case SUB_GR_GET_USER_PACKET_RET:				// 418     ≤È—Ø∑µªÿ
		return this->OnSocketWithGetUserPacket(pBuffer, wDataSize);
	case SUB_GP_GUESS_COIN_RET:				// 418     ≤È—Ø∑µªÿ
		return this->OnSocketWithGuessCoinRet(pBuffer, wDataSize);
	case SUB_GP_TURN_TALBE_RET:				// 418     ≤È—Ø∑µªÿ
		return this->OnSocketWithTurnTableRet(pBuffer, wDataSize);
	case SUB_GP_QUERY_GAME_RET:				// SUB_GP_QUERY_GAME_RET     ≤È—Ø∑µªÿ
		return this->OnSocketWithQueryGameRet(pBuffer, wDataSize);
	case SUB_GP_GET_MORE_RECORD_RET:				// SUB_GP_QUERY_GAME_RET     ≤È—Ø∑µªÿ
		return this->OnSocketWithQueryMoreGameRet(pBuffer, wDataSize);
	case SUB_GP_GET_USER_FANDIAN_RET:
		return this->OnSocketWithQueryMyFanDianRet(pBuffer, wDataSize);
	case SUB_GP_GET_QIHAO_CHA_RET:
		return this->OnSocketWithQueryHGYDWFDelta(pBuffer, wDataSize);
	case SUB_GP_GET_CANADA_START_QIHAO_RET: //加拿大3.5分
		return this->OnSocketWithQueryCanadaSDWFQiHao(pBuffer, wDataSize);
	case SUB_GP_QUERY_STATUS_LOTTERY_RET:
		return this->OnSocketWithQueryKuaNianQiHao(pBuffer,wDataSize);
	case SUB_GP_GET_USER_QIANDAO_RET: //签到
		return this->OnSocketWithSignUp(pBuffer, wDataSize);
	case SUB_GP_GET_USER_HUANLESONG_RET: //欢乐
		return this->OnSocketWithQueryHuanLeSong(pBuffer, wDataSize);
	case SUB_GP_TOUZHU_CQSSC_RET:
		return this->OnSocketWithCQSSCRET(pBuffer, wDataSize);
	case SUB_GR_GET_TOUZHU_LOG_COUNT_RET	:
		return this->OnSocketWithGetTouZhuLogCountRet(pBuffer, wDataSize);
	case SUB_GP_GET_TOUZHU_LOG_RET	:
		return this->OnSocketWithGetTouZhuLogRet(pBuffer, wDataSize);
	case SUB_GR_GET_HYXX_LOG_COUNT_RET	:
		return this->OnSocketWithGetHYXXLogCountRet(pBuffer, wDataSize);
	case SUB_GP_GET_HYXX_LOG_RET	:
		return this->OnSocketWithGetHYXXLogRet(pBuffer, wDataSize);
	case SUB_GR_GET_XJTZH_LOG_COUNT_RET	:
		return this->OnSocketWithGetXJTZHLogCountRet(pBuffer, wDataSize);
	case SUB_GP_GET_XJTZH_LOG_RET	:
		return this->OnSocketWithGetXJTZHLog(pBuffer, wDataSize);
	case SUB_GR_CHK_XJTZH_LOG_COUNT_RET	:
		return this->OnSocketWithCHKXJTZHLogCountRet(pBuffer, wDataSize);
	case SUB_GP_CHK_XJTZH_LOG_RET	:
		return this->OnSocketWithCHKXJTZHLog(pBuffer, wDataSize);
	case SUB_GR_GET_XJYX_TJ_COUNT_RET	:
		return this->OnSocketWithGetXJYXTjCountRet(pBuffer, wDataSize);
	case SUB_GP_GET_XJYX_TJ_RET	:
		return this->OnSocketWithGetXJYXTj(pBuffer, wDataSize);
	case SUB_GR_GET_XJYK_LOG_COUNT_RET	:
		return this->OnSocketWithGetXJYKLogCountRet(pBuffer, wDataSize);
	case SUB_GP_GET_XJYK_LOG_RET	:
		return this->OnSocketWithGetXJYKLogRet(pBuffer, wDataSize);
	case SUB_GR_GET_XJYK_TJ_COUNT_RET	:
		return this->OnSocketWithGetXJYKTjCountRet(pBuffer, wDataSize);
	case SUB_GR_GET_PARENT_RET://父子关系
		return this->OnSocketWithGetParentRelativeRet(pBuffer, wDataSize);
	case SUB_GP_GET_QIPAI_KIND_RET://游戏列表
		return this->OnSocketWithGetGameKindRet(pBuffer, wDataSize);
	case SUB_GP_GET_XJYK_TJ_RET	:
		return this->OnSocketWithGetXJYKTjRet(pBuffer, wDataSize);
	case SUB_GR_GET_XJCHZH_LOG_COUNT_RET	:
		return this->OnSocketWithGetXjChzhLogCountRet(pBuffer, wDataSize);
	case SUB_GP_GET_XJCHZH_LOG_RET	:
		return this->OnSocketWithGetXJCHZHLogRet(pBuffer, wDataSize);
	case SUB_GR_GET_XJTX_LOG_COUNT_RET	:
		return this->OnSocketWithGetXjTxLogCountRet(pBuffer, wDataSize);
	case SUB_GP_GET_XJTX_LOG_RET	:
		return this->OnSocketWithGetXJTxLogRet(pBuffer, wDataSize);
	case SUB_GR_GET_TIXIAN_LOG_COUNT_RET	:
		return this->OnSocketWithGetTiXianLogCountRet(pBuffer, wDataSize);
	case SUB_GP_GET_TIXIAN_LOG_RET	:
		return this->OnSocketWithGetTiXianLogRet(pBuffer, wDataSize);
	case SUB_GR_GET_CHONGZHI_LOG_COUNT_RET	:
		return this->OnSocketWithGetChongzhiLogCountRet(pBuffer, wDataSize);
	case SUB_GP_GET_CHONGZHI_LOG_RET	:
		return this->OnSocketWithGetChongzhiLogRet(pBuffer, wDataSize);
	case SUB_GR_GET_YINGKUI_LOG_COUNT_RET	:
		return this->OnSocketWithGetYingkuiLogCountRet(pBuffer, wDataSize);
	case SUB_GP_GET_YINGKUI_LOG_RET	:
		return this->OnSocketWithGetYingkuiLogRet(pBuffer, wDataSize);
	case SUB_GR_GET_QP_YINGKUI_COUNT_RET	:
		return this->OnSocketWithGetQiPaiYingkuiLogCountRet(pBuffer, wDataSize);
	case SUB_GP_GET_QP_YINGKUI_RET	:
		return this->OnSocketWithGetQiPaiYingkuiLogRet(pBuffer, wDataSize);
	case SUB_GP_TOUZHU_CONTINUE_RET:
		return this->OnSocketWithTouZhuContinueRet(pBuffer, wDataSize);
	case SUB_GP_GET_TOU_ZHU_XX_RET:
	case SUB_MB_GET_TOU_ZHU_XX_RET:
		return this->OnSocketWithGetTouZhuXXRet(pBuffer, wDataSize);
	case SUB_GR_KEFU_URL_RET:
		return this->OnSocketWithGetKeFuURLRet(pBuffer, wDataSize);
	case SUB_GP_SEND_CHECKYANZHENGMA_RET: //登录用验证码返回
	case SUB_GP_SEND_CHECKYANZHENGMA_TRANS_RET: //转帐用验证码返回
		return this->OnSocketWithGetValidateCodeRet(pBuffer, wDataSize);
	case SUB_GP_GET_PHONE_TRASFER_FENHONG_RET://转帐是否需要验证码
		return this->OnSocketWithGetTransPhoneRet(pBuffer, wDataSize);
	case SUB_GR_BIND_PHONE_RET:
		return this->OnSocketWithBindPhoneRet(pBuffer, wDataSize);
	case SUB_GR_BIND_PHONE_INFO_RET:
		return this->OnSocketWithGetValidateTypeRet(pBuffer, wDataSize);
	case SUB_GR_UNBIND_PHONE_RET:
		return this->OnSocketWithUnBindPhoneRet(pBuffer, wDataSize);
	case SUB_GP_QUERY_TIME_RET:
		return this->OnSocketWithGetSystemTimeRet(pBuffer, wDataSize);
	case SUB_GR_CANCEL_TOUZHU_RET:
		return this->OnSocketWithCancelTouZhuRet(pBuffer, wDataSize);
	case SUB_GP_CHONG_ZHI_TYPE_RET:
		return this->OnSocketWithGetChongzhiTypeRet(pBuffer, wDataSize);
	case SUB_GP_QU_KUAN_INFO_RET:
		return this->OnSocketWithGetQukuanInfoRet(pBuffer, wDataSize);
	case SUB_GP_DO_QU_KUAN_RET:
		return this->OnSocketWithDoQukuanRet(pBuffer, wDataSize);
	case SUB_GP_GET_QUKUAN_LIMIT_RET:
		return this->onSocketWithQuKuanLimitRet(pBuffer, wDataSize);
	case SUB_GR_SET_QUKUAN_ZHANGHAO_RET:
		return this->OnSocketWithSetQukuanZhanghaoRet(pBuffer, wDataSize);
	case SUB_GR_QUERY_MY_YINHANG_RET:
		return this->OnSocketWithQueryMyYinHangRet(pBuffer, wDataSize);
	case SUB_GP_QUERY_YINHANG_RET: //910
		return this->OnSocketWithQueryYinHangRet(pBuffer, wDataSize);
	case SUB_GP_SET_USER_BONUS_RET:
		return this->OnSocketWithSetusedBonusRet(pBuffer, wDataSize);
	case SUB_GP_GET_NOTICE_RET:
		return this->OnSocketWithGetNews(pBuffer, wDataSize);
	case SUB_GP_GET_ACTIONINFO_RET:
		return this->OnSocketWithGetActive(pBuffer, wDataSize);
	case SUB_GP_GET_INBOX_MESSAGE_RET:
		return this->OnSocketWithGetInboxMessage(pBuffer, wDataSize);
	case SUB_GP_CHK_INBOX_MESSAGE_RET:
		return this->OnSocketWithChkInboxMessage(pBuffer, wDataSize);
	case SUB_GP_GET_ALL_USERINFO_RET:
		return this->OnSocketWithGetXJUserInfo(pBuffer, wDataSize);
	case SUB_GP_SEND_MESSAGE_RET:
		return this->OnSocketWithSendMessage(pBuffer, wDataSize);
	case SUB_GP_GET_ZNX_COUNT_RET:
		return this->OnSocketWithGetMessageCount(pBuffer, wDataSize);
	case SUB_GR_XG_LOGIN_PASS_RET:
	case SUB_GR_XG_QUKUAN_PASS_RET:
		{
			//closeLoginSocket();
			CMD_GR_XG_Loginpass_RET *operateSuccess = (CMD_GR_XG_Loginpass_RET *)pBuffer;
			Integer* pPasswordResult = Integer::create(operateSuccess->lResult);
			NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_PASSWORDUPDATE,pPasswordResult);
			return true;
		}
	case SUB_GR_ADD_HY_RET:
		return this->OnSocketWithAddHuiYuanRet(pBuffer, wDataSize);
	case SUB_GR_GET_Peie_RET:
		return this->OnSocketWithPeiERet(pBuffer, wDataSize);
	case SUB_GP_KILL_SOCKET:
		return this->OnSocketWithForceOut(pBuffer, wDataSize);
	case SUB_GR_SET_WEB_FANDIAN_RET:
		return this->OnSocketWithWebFanDianRet(pBuffer, wDataSize);
	case SUB_GR_GET_REG_URL_RET:
		return this->OnSocketWithQueryWebFanDianRet(pBuffer, wDataSize);
	case SUB_GR_GET_HYSHJ_RET:
		return this->OnSocketWithGetHyShjRet(pBuffer, wDataSize);
	case SUB_GP_ZHUANHUAN_RET:
		return this->OnSocketWithZhuanHuanRet(pBuffer, wDataSize);
	case SUB_GR_GET_USER_INFO_RET:
		return this->OnSocketWithGetUserInfoRet(pBuffer, wDataSize);
	case SUB_GP_GET_LAST_YUE_RET:
		return this->OnSocketWithGetLastYueRet(pBuffer, wDataSize);
	case SUB_GR_GET_LHC_QIHAO_RET:
		return this->OnSocketWithGetLiuHeFenDan(pBuffer, wDataSize);
	case SUB_GP_GET_NEWS_RET:
		return this->OnSocketWithGetRankListRet(pBuffer, wDataSize);
	case SUB_GP_DAILI_LJ_RET:
		return this->OnSocketWithDaiLiQianDaoRet(pBuffer, wDataSize);
	case SUB_GP_GET_ChongZhiSong_RET://充值送 
		return this->OnSocketWithChongZhiSongGetRet(pBuffer, wDataSize);
	case SUB_GP_SET_ChongZhiSong_RET:
		return this->OnSocketWithChongZhiSongSetRet(pBuffer, wDataSize);
	case SUB_GP_DEL_MESSAGE_RET:
		return this->OnSocketWithDeleteMessageRet(pBuffer, wDataSize);
	case SUB_GP_GET_DAILI_HUIKUI_RET:
		return this->OnSocketWithQuerryDailiQianDaoRet(pBuffer, wDataSize);
	case SUB_GR_HYXX_ZHUANZHANG_RET:
		return this->OnSocketWithZhuanZhangRet(pBuffer, wDataSize);
	case SUB_GR_GET_XJ_Peie_RET:
		return this->OnSocketWithXiajiPeiERet(pBuffer, wDataSize);
	case SUB_GR_SET_XJ_Peie_RET:
		return this->OnSocketWithSetXiaJiPeiERet(pBuffer, wDataSize);
	case SUB_GR_XG_HY_FANDIAN_RET:
		return this->OnSocketWithSetXiaJiFanDian(pBuffer, wDataSize);
	case SUB_GP_QUIT_GAME_RET:
		return this->OnSocketWithExit(pBuffer, wDataSize);
	default:
		break;
	}

	return true;
}

/*************±≥∞¸≤Ÿ◊˜*************/
//π∫¬ÚŒÔ∆∑      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithSearchGoods(void *buffer, WORD size)
{
	BYTE cbBuffer[sizeof(CMD_GR_GoodsInfoRet)];
	memset(cbBuffer, 0 , sizeof(CMD_GR_GoodsInfoRet));
	CMD_GR_GoodsInfoRet *goodsInfo = (CMD_GR_GoodsInfoRet *)cbBuffer;
	memcpy(goodsInfo, buffer, sizeof(CMD_GR_GoodsInfoRet));
	//    m_dwlUserScore = userInsureInfo->lUserScore;				//”√ªß∂π∂π
	//    m_dwlUserInsure = userInsureInfo->lUserInsure;				//±£œ’πÒ∂π∂π
	//    g_GlobalUnits.getGolbalUserData()->lScore = m_dwlUserScore;
	//    g_GlobalUnits.getGolbalUserData()->lGoldEggs = 0;
	//    NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_UPDATESCOREANDGOLDEGG,NULL);  //À¢–¬«Æ∫Õª®∑—µ„
	//    MyNSString::wc2mbs(userInsureInfo->szDescribeString, sizeof(userInsureInfo->szDescribe
	return true;
}

bool MsgDispatch::OnSocketWithBuyGoods(void *buffer, WORD size)
{
	BYTE cbBuffer[sizeof(CMD_GP_UserInsureFailure)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_UserInsureFailure));
	CMD_GP_UserInsureFailure *goodsInfo = (CMD_GP_UserInsureFailure *)cbBuffer;
	memcpy(goodsInfo, buffer, sizeof(CMD_GP_UserInsureFailure));
	//    m_dwlUserScore = userInsureInfo->lUserScore;				//”√ªß∂π∂π
	//    m_dwlUserInsure = userInsureInfo->lUserInsure;				//±£œ’πÒ∂π∂π
	//    g_GlobalUnits.getGolbalUserData()->lScore = m_dwlUserScore;
	//    g_GlobalUnits.getGolbalUserData()->lGoldEggs = 0;
	//    NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_UPDATESCOREANDGOLDEGG,NULL);  //À¢–¬«Æ∫Õª®∑—µ„
	MyNSString::wc2mbs(goodsInfo->szDescribeString, sizeof(goodsInfo->szDescribeString), szDescribeString);
	NotificationCenter::getInstance()->postNotification("BuyGoodsInfoRet");
	return true;
}

//‘˘ÀÕŒÔ∆∑      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithGiveGoods(void *buffer, WORD size)
{
	return true;
}

//      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithGetUserPacket(void *buffer, WORD size)
{
	BYTE cbBuffer[2048];
	memset(cbBuffer, 0 , 2048);
	CMD_GR_GetUserPacketRet *userPacket = (CMD_GR_GetUserPacketRet *)cbBuffer;
	memcpy(userPacket, buffer, size);
	unsigned int num = size/sizeof(CMD_GR_GetUserPacketRet);
	Array *data = Array::create();
	for(unsigned int i = 0 ; i < num ; i++){
		CMD_GR_GetUserPacketRet tempUserPacket = userPacket[i];
		data->addObject(Integer::create(tempUserPacket.dwPropertyID));
		data->addObject(Integer::create(tempUserPacket.dwPropertyCount));
	}
	NotificationCenter::getInstance()->postNotification("UpdateUserPacket", data);
	return true;
}

//猜硬币返回      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithGuessCoinRet(void *buffer, WORD size)
{
	BYTE cbBuffer[2048];
	memset(cbBuffer, 0 , 2048);
	CMD_GP_GuessCoinRet *info = (CMD_GP_GuessCoinRet *)cbBuffer;
	memcpy(info, buffer, size);
	Array *data = Array::create();
	data->addObject(Bool::create(info->bCoinSide));
	data->addObject(Integer::create(info->lScore));
	NotificationCenter::getInstance()->postNotification("GuessCoinRet", data);
	return true;
}

//大转盘返回      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithTurnTableRet(void *buffer, WORD size)
{
	BYTE cbBuffer[2048];
	memset(cbBuffer, 0 , 2048);
	CMD_GP_TurnTableRet *info = (CMD_GP_TurnTableRet *)cbBuffer;
	memcpy(info, buffer, size);
	info = (CMD_GP_TurnTableRet *)buffer;
	Array *data = Array::create();
	data->addObject(Integer::create(info->cbGoodType));
	data->addObject(Integer::create(info->lScore));

	NotificationCenter::getInstance()->postNotification("TurnTableRet", data);
	return true;
}

//游戏开奖查询返回      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithQueryGameRet(void *buffer, WORD size)
{
	int count = size/sizeof(DBO_GP_QueryLotResult);
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	DBO_GP_QueryLotResult *info = (DBO_GP_QueryLotResult *)cbBuffer;
	memcpy(info, buffer, size);

	char	period[500] = {0};						//期
	char	lotNum[500] = {0};						//开奖数据
	char	shijian[500] = {0};	

	Array* data = Array::create();
	for(int i = 0 ; i < count ; i++)
	{
		memset(period, 0, sizeof(period));
		memset(lotNum, 0, sizeof(lotNum));
		memset(shijian, 0, sizeof(shijian));
		MyNSString::wc2mbs((info+i)->wPeriod, sizeof((info+i)->wPeriod), period);
		MyNSString::wc2mbs((info+i)->szLotNum, sizeof((info+i)->szLotNum), lotNum);
		MyNSString::wc2mbs((info+i)->szShijian, sizeof((info+i)->szShijian), shijian);

		if(strlen(period) == 0) continue;
		
		data->addObject(Integer::create((info+i)->wKindID));
		data->addObject(String::createWithFormat("%s", period));
		data->addObject(String::createWithFormat("%s", lotNum));
		data->addObject(String::createWithFormat("%s", shijian));
	}

	
	NotificationCenter::getInstance()->postNotification("QueryGameRet", data);


	return true;
}

//获取更多开奖结果返回      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithQueryMoreGameRet(void *buffer, WORD size)
{
	int count = size/sizeof(CMD_GP_GetMoreRecordRet);
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GP_GetMoreRecordRet *info = (CMD_GP_GetMoreRecordRet *)cbBuffer;
	memcpy(info, buffer, size);

	char	period[300] = {0};						//期
	char	lotNum[300] = {0};						//开奖数据
	char	shijian[300] = {0};
	Array *data = Array::create();
	for(int i = 0 ; i < count ; i++){

		memset(period, 0, sizeof(period));
		memset(lotNum, 0, sizeof(lotNum));
		memset(shijian, 0, sizeof(shijian));
		MyNSString::wc2mbs((info+i)->wPeriod, sizeof((info+i)->wPeriod), period);
		MyNSString::wc2mbs((info+i)->szLotNum, sizeof((info+i)->szLotNum), lotNum);
		MyNSString::wc2mbs((info+i)->szShijian, sizeof((info+i)->szShijian), shijian);
		data->addObject(Integer::create((info+i)->wKindID));
		data->addObject(String::createWithFormat("%s", period));
		data->addObject(String::createWithFormat("%s", lotNum));
		data->addObject(String::createWithFormat("%s", shijian));
	}

	NotificationCenter::getInstance()->postNotification("QueryMoreGameRet", data);

	return true;
}

bool MsgDispatch::OnSocketWithQueryMyFanDianRet(void *buffer, WORD size)
{
	int count = size/sizeof(CMD_GR_GetUserFandianRet);
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetUserFandianRet *info = (CMD_GR_GetUserFandianRet *)cbBuffer;
	memcpy(info, buffer, size);
	Array *data = Array::create();
	for(int i = 0 ; i < count ; i++)
	{
		data->addObject(Integer::create((info+i)->n_t_kindid));
		data->addObject(String::createWithFormat("%lf", (info+i)->f_t_bonus));
		data->addObject(String::createWithFormat("%lf", (info+i)->f_t_bonusPercent));
		data->addObject(String::createWithFormat("%lf", (info+i)->f_t_fandian));
	}
	NotificationCenter::getInstance()->postNotification("QueryMyFanDianRet", data);
	return true;

}

bool MsgDispatch::OnSocketWithQueryHGYDWFDelta(void *buffer, WORD size)
{
	int count = size/sizeof(CMD_GP_GetQihaoCha);
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GP_GetQihaoCha *info = (CMD_GP_GetQihaoCha *)cbBuffer;
	memcpy(info, buffer, size);

	Dictionary* data = new Dictionary();
	data->setObject(Integer::create(info->n_t_qishu), "n_t_qishu");
	NotificationCenter::getInstance()->postNotification("HgydwfcDelta", data);	

	return true;
};

bool MsgDispatch::OnSocketWithQueryCanadaSDWFQiHao(void *buffer, WORD size)
{
	int count = size/sizeof(CMD_GP_GetCanadaQihaoRet);
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GP_GetCanadaQihaoRet *info = (CMD_GP_GetCanadaQihaoRet *)cbBuffer;
	memcpy(info, buffer, size);

	Dictionary* data = new Dictionary();
	data->setObject(Integer::create(info->n_t_start_qihao), "n_t_start_qihao");

	time_t local_time = info->n_t_start_time;
	tm* tm_local = localtime(&local_time);

	data->setObject(String::createWithFormat("%04d-%02d-%02d %02d:%02d:%02d", tm_local->tm_year + 1900,  
		tm_local->tm_mon + 1, tm_local->tm_mday, tm_local->tm_hour, tm_local->tm_min,
		tm_local->tm_sec), "n_t_start_time");
	NotificationCenter::getInstance()->postNotification("CanadaSDWFQiHao", data);	

	return true;
}

//跨年期号调整;返回的结果
bool MsgDispatch::OnSocketWithQueryKuaNianQiHao(void *buffer, WORD size)
{
	//CMD_GP_QueryStatusLotteryRet;
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GP_QueryStatusLotteryRet *info = (CMD_GP_QueryStatusLotteryRet *)cbBuffer;
	memcpy(info, buffer, size);

	Dictionary* data = new Dictionary();
	data->setObject(Integer::create(info->n_t_kindid), "n_t_kindid");
	data->setObject(Integer::create(info->c_t_ifts),"c_t_ifts");
	char expect[100] = {0};
	MyNSString::wc2mbs(info->s_t_expect, sizeof(info->s_t_expect), expect);
	data->setObject(String::create(expect),"s_t_expect");

	data->setObject(String::createWithFormat("%04d-%02d-%02d %02d:%02d:%02d", info->s_t_shijian.wYear,
		info->s_t_shijian.wMonth, info->s_t_shijian.wDay, info->s_t_shijian.wHour, info->s_t_shijian.wMinute,
		info->s_t_shijian.wSecond), "s_t_shijian");
	
	//by david, 2018.8.28
	//NotificationCenter::getInstance()->postNotification("KuaNian", data);	

	return true;
}

//签到
bool MsgDispatch::OnSocketWithSignUp(void *buffer, WORD size)
{
	int count = size/sizeof(CMD_GP_UserQiandaoRet);
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GP_UserQiandaoRet *info = (CMD_GP_UserQiandaoRet *)cbBuffer;
	memcpy(info, buffer, size);

	Dictionary* data = new Dictionary();
	data->setObject(Integer::create(info->n_t_res), "n_t_res");
	NotificationCenter::getInstance()->postNotification("SignUpRet", data);	

	return true;
}

//欢乐颂
bool MsgDispatch::OnSocketWithQueryHuanLeSong(void *buffer, WORD size)
{
	int count = size/sizeof(CMD_GP_GetUserHuanlesongRet);
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GP_GetUserHuanlesongRet *info = (CMD_GP_GetUserHuanlesongRet *)cbBuffer;
	memcpy(info, buffer, size);

	Dictionary* data = new Dictionary();
	data->setObject(Integer::create(info->nRes), "nRes");
	data->setObject(Integer::create(info->n_t_count), "n_t_count");
	data->setObject(Integer::create(info->n_t_XXcount), "n_t_XXcount");
	data->setObject(Double::create(info->f_t_xiaofei), "f_t_xiaofei");
	data->setObject(Double::create(info->f_t_prize), "f_t_prize");
	NotificationCenter::getInstance()->postNotification("HuanLeSongRet", data);	

	return true;
}

//CQSSC 下注返回      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithCQSSCRET(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_TouzhuRes *info = (CMD_GR_TouzhuRes *)cbBuffer;
	memcpy(info, buffer, size);

	char	resultDesc[2048] = {0};
	MyNSString::wc2mbs(info->szDesc, sizeof(info->szDesc), resultDesc);
	Array *data = Array::create();
	data->addObject(Integer::create(info->lResult));
	data->addObject(String::createWithFormat("%s", resultDesc));
	data->addObject(Integer::create(info->nSign));

	NotificationCenter::getInstance()->postNotification("CQSSCRet", data);

	return true;
}

//获取投注记录数返回      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithGetTouZhuLogCountRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetTouzhuLogCountRet *info = (CMD_GR_GetTouzhuLogCountRet *)cbBuffer;
	memcpy(info, buffer, size);

	NotificationCenter::getInstance()->postNotification("TouZhuLogCount", Integer::create(info->dwCount));

	return true;
}

//获取投注信息返回      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithGetTouZhuLogRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetTouzhuLogRet *info = (CMD_GR_GetTouzhuLogRet *)cbBuffer;
	memcpy(info, buffer, size);
	char qiShu[120];
	int count = size/sizeof(CMD_GR_GetTouzhuLogRet);
	Array *data = Array::create();
	for(int i = 0 ; i < count ; i++){
		memset(qiShu, 0, sizeof(qiShu));
		Dictionary *dictionary = Dictionary::create();
		dictionary->setObject(String::createWithFormat("%04d-%02d-%02d %02d:%02d:%02d", (info+i)->n_t_time.wYear,
			(info+i)->n_t_time.wMonth, (info+i)->n_t_time.wDay, (info+i)->n_t_time.wHour, (info+i)->n_t_time.wMinute,
			(info+i)->n_t_time.wSecond), "time");
		dictionary->setObject(Integer::create((info+i)->n_t_id), "tid");
		dictionary->setObject(Integer::create((info+i)->n_t_type), "gameType");
		dictionary->setObject(Integer::create((info+i)->n_t_kind), "gameKind");
		MyNSString::wc2mbs((info+i)->szQishu, 30, qiShu);
		dictionary->setObject(String::createWithFormat("%s", qiShu), "qiShu");
		dictionary->setObject(String::create(""), "betNumber");
		dictionary->setObject(Integer::create((info+i)->n_t_zhushu), "zhuShu");
		dictionary->setObject(Integer::create((info+i)->n_t_beishu), "beiTou");
		dictionary->setObject(Integer::create((info+i)->n_t_moshi), "moshi");
		dictionary->setObject(Integer::create((info+i)->n_t_state), "state");
		dictionary->setObject(Double::create((info+i)->f_t_yingkui), "yingkui");
		dictionary->setObject(Integer::create((info + i)->n_t_winzhushu), "winZhuShu");
		data->addObject(dictionary);
	}

	NotificationCenter::getInstance()->postNotification("TouZhuLog", data);
	return true;
}

//查询会员信息日志数返回       add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithGetHYXXLogCountRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetHYXXLogCountRet *info = (CMD_GR_GetHYXXLogCountRet *)cbBuffer;
	memcpy(info, buffer, size);
	NotificationCenter::getInstance()->postNotification("GetLogCount", Integer::create(info->dwCount));

	return true;
}

//查询会员信息返回      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithGetHYXXLogRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetHYXXLogRet *info = (CMD_GR_GetHYXXLogRet *)cbBuffer;
	memcpy(info, buffer, size);
	char s_t_account[120];
	char s_t_qq[120];
	char s_t_online[120];
	int count = size/sizeof(CMD_GR_GetHYXXLogRet);
	Array *data = Array::create();
	for(int i = 0 ; i < count ; i++){
		memset(s_t_account, 0, sizeof(s_t_account));
		memset(s_t_qq, 0, sizeof(s_t_qq));
		memset(s_t_online, 0, sizeof(s_t_online));
		Dictionary *dictionary = Dictionary::create();
		dictionary->setObject(Integer::create((info+i)->n_t_id), "n_t_id");
		MyNSString::wc2mbs((info+i)->s_t_account, 30, s_t_account);
		dictionary->setObject(String::createWithFormat("%s", s_t_account), "s_t_account");
		dictionary->setObject(String::createWithFormat("%04d-%02d-%02d %02d:%02d:%02d", (info+i)->n_t_time.wYear,
			(info+i)->n_t_time.wMonth, (info+i)->n_t_time.wDay, (info+i)->n_t_time.wHour, (info+i)->n_t_time.wMinute,
			(info+i)->n_t_time.wSecond), "time");
		//最后登陆时间  modify by hd
		dictionary->setObject(String::createWithFormat("%04d-%02d-%02d %02d:%02d:%02d", (info+i)->n_t_lasttime.wYear,
			(info+i)->n_t_lasttime.wMonth, (info+i)->n_t_lasttime.wDay, (info+i)->n_t_lasttime.wHour, (info+i)->n_t_lasttime.wMinute,
			(info+i)->n_t_lasttime.wSecond), "lasttime");
		dictionary->setObject(Integer::create((info+i)->n_t_type), "n_t_type");
		dictionary->setObject(Double::create((info+i)->f_t_fandian), "f_t_fandian");
		dictionary->setObject(Double::create((info+i)->f_t_yue), "f_t_yue");
		dictionary->setObject(Integer::create((info+i)->n_t_online), "n_t_online");
		MyNSString::wc2mbs((info+i)->s_t_qq, 30, s_t_qq);
		dictionary->setObject(String::createWithFormat("%s", s_t_qq), "s_t_qq");
		//下级在线人数 
		MyNSString::wc2mbs((info+i)->s_t_online, 30, s_t_online);
		dictionary->setObject(String::createWithFormat("%s", s_t_online), "s_t_online");
		data->addObject(dictionary);
	}

	NotificationCenter::getInstance()->postNotification("GetLog", data);
	return true;
}

//获取下级投注统计记录数返回      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithGetXJTZHLogCountRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetXJTZHLogCountRet *info = (CMD_GR_GetXJTZHLogCountRet *)cbBuffer;
	memcpy(info, buffer, size);

	NotificationCenter::getInstance()->postNotification("XJTZHLogCountRet", Integer::create(info->dwCount));

	return true;
}

//获取下级投注统计信息返回      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithGetXJTZHLog(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetXJTZHLogRet *info = (CMD_GR_GetXJTZHLogRet *)cbBuffer;
	memcpy(info, buffer, size);
	char s_t_account[120];
	int count = size/sizeof(CMD_GR_GetXJTZHLogRet);
	Array *data = Array::create();
	for(int i = 0 ; i < count ; i++){
		memset(s_t_account, 0, sizeof(s_t_account));
		Dictionary *dictionary = Dictionary::create();
		dictionary->setObject(Integer::create((info+i)->n_t_id), "n_t_id");
		MyNSString::wc2mbs((info+i)->s_t_account, 30, s_t_account);
		dictionary->setObject(String::createWithFormat("%s", s_t_account), "s_t_account");
		dictionary->setObject(Integer::create((info+i)->n_t_type), "n_t_type");
		dictionary->setObject(Double::create((info+i)->f_t_touzhu_ze), "f_t_touzhu_ze");
		dictionary->setObject(Double::create((info+i)->f_t_yingkui_ze), "f_t_yingkui_ze");
		data->addObject(dictionary);
	}

	NotificationCenter::getInstance()->postNotification("XJTZHLog", data);
	return true;
}

//查看下级投注记录数返回      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithCHKXJTZHLogCountRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_CHKXJTZHLogCountRet *info = (CMD_GR_CHKXJTZHLogCountRet *)cbBuffer;
	memcpy(info, buffer, size);
	
	//NotificationCenter::getInstance()->postNotification("CHKXJTZHLogCountRet", Integer::create(info->dwCount));
	NotificationCenter::getInstance()->postNotification("GetLogCount", Integer::create(info->dwCount));

	return true;
}

//查看下级投注返回      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithCHKXJTZHLog(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_CHKXJTZHLogRet *info = (CMD_GR_CHKXJTZHLogRet *)cbBuffer;
	memcpy(info, buffer, size);
	char s_t_account[120];
	char s_t_qishu[120];
	int count = size/sizeof(CMD_GR_CHKXJTZHLogRet);
	Array *data = Array::create();
	for(int i = 0 ; i < count ; i++){
		memset(s_t_account, 0, sizeof(s_t_account));
		memset(s_t_qishu, 0, sizeof(s_t_qishu));
		Dictionary *dictionary = Dictionary::create();
		dictionary->setObject(Integer::create((info+i)->n_t_id), "n_t_id");
		MyNSString::wc2mbs((info+i)->s_t_account, 33, s_t_account);
		dictionary->setObject(String::createWithFormat("%s", s_t_account), "s_t_account");
		dictionary->setObject(Integer::create((info+i)->n_t_type), "n_t_type");
		dictionary->setObject(Integer::create((info+i)->n_t_kind), "n_t_kind");
		MyNSString::wc2mbs((info+i)->s_t_qishu, 30, s_t_qishu);
		dictionary->setObject(String::createWithFormat("%s", s_t_qishu), "s_t_qishu");
		dictionary->setObject(Integer::create((info+i)->n_t_zhushu), "n_t_zhushu");
		dictionary->setObject(Integer::create((info+i)->n_t_moshi), "n_t_moshi");
		dictionary->setObject(Integer::create((info+i)->n_t_beishu), "n_t_beishu");
		dictionary->setObject(String::createWithFormat("%04d-%02d-%02d %02d:%02d:%02d", (info+i)->n_t_time.wYear,
			(info+i)->n_t_time.wMonth, (info+i)->n_t_time.wDay, (info+i)->n_t_time.wHour, (info+i)->n_t_time.wMinute,
			(info+i)->n_t_time.wSecond), "n_t_time");
		dictionary->setObject(Integer::create((info+i)->n_t_state), "n_t_state");
		dictionary->setObject(Integer::create((info+i)->n_t_winzhushu), "n_t_winzhushu");
		dictionary->setObject(Double::create((info+i)->f_t_yingkui), "f_t_yingkui");
		dictionary->setObject(Integer::create((info+i)->n_t_zhuihao), "n_t_zhuihao");
		data->addObject(dictionary);
	}
	
	NotificationCenter::getInstance()->postNotification("GetLog", data);

//	NotificationCenter::getInstance()->postNotification("CHKXJTZHLog", data);
	return true;
}

//查看下级游戏盈亏记录数返回      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithGetXJYXTjCountRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetXJYXTjCountRet *info = (CMD_GR_GetXJYXTjCountRet *)cbBuffer;
	memcpy(info, buffer, size);

	NotificationCenter::getInstance()->postNotification("GetXJYXTjCountRet", Integer::create(info->dwCount));

	return true;
}

//查看下级游戏盈亏返回      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithGetXJYXTj(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetXJYXTjRet *info = (CMD_GR_GetXJYXTjRet *)cbBuffer;
	memcpy(info, buffer, size);
	char s_t_account[120];
	int count = size/sizeof(CMD_GR_GetXJYXTjRet);
	Array *data = Array::create();
	for(int i = 0 ; i < count ; i++){
		memset(s_t_account, 0, sizeof(s_t_account));
		Dictionary *dictionary = Dictionary::create();
		dictionary->setObject(Integer::create((info+i)->n_t_id), "n_t_id");
		MyNSString::wc2mbs((info+i)->s_t_account, 33, s_t_account);
		dictionary->setObject(String::createWithFormat("%s", s_t_account), "s_t_account");
		dictionary->setObject(Integer::create((info+i)->n_t_type), "n_t_type");
		dictionary->setObject(Integer::create((info+i)->n_t_daili_id), "n_t_daili_id");
		dictionary->setObject(Double::create((info+i)->f_t_fandian_ze), "f_t_fandian_ze");
		dictionary->setObject(Double::create((info+i)->f_t_qipai_ze), "f_t_qipai_ze");
		dictionary->setObject(Double::create((info+i)->f_t_tuandui_ye), "f_t_tuandui_ye");
		data->addObject(dictionary);
	}

	NotificationCenter::getInstance()->postNotification("GetXJYXTjRet", data);
	return true;
}

//查询下级盈亏信息日志数返回      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithGetXJYKLogCountRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetXJYKLogCountRet *info = (CMD_GR_GetXJYKLogCountRet *)cbBuffer;
	memcpy(info, buffer, size);

	NotificationCenter::getInstance()->postNotification("GetXJYKLogCountRet", Integer::create(info->dwCount));

	return true;
}

//查询下级盈亏返回      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithGetXJYKLogRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetXJYKLogRet *info = (CMD_GR_GetXJYKLogRet *)cbBuffer;
	memcpy(info, buffer, size);
	char s_t_account[100];
	char s_t_comment[127];
	int count = size/sizeof(CMD_GR_GetXJYKLogRet);
	Array *data = Array::create();
	for(int i = 0 ; i < count ; i++){
		memset(s_t_account, 0, sizeof(s_t_account));
		memset(s_t_comment, 0, sizeof(s_t_comment));
		Dictionary *dictionary = Dictionary::create();
		dictionary->setObject(Integer::create((info+i)->n_t_id), "n_t_id");
		dictionary->setObject(Integer::create((info+i)->n_t_userid), "n_t_userid");
		MyNSString::wc2mbs((info+i)->s_t_account, 33, s_t_account);
		dictionary->setObject(String::createWithFormat("%s", s_t_account), "s_t_account");
		dictionary->setObject(Double::create((info+i)->f_t_yingkui), "f_t_yingkui");
		dictionary->setObject(Double::create((info+i)->f_t_at_yue), "f_t_at_yue");
		dictionary->setObject(String::createWithFormat("%04d-%02d-%02d %02d:%02d:%02d", (info+i)->n_t_time.wYear,
			(info+i)->n_t_time.wMonth, (info+i)->n_t_time.wDay, (info+i)->n_t_time.wHour, (info+i)->n_t_time.wMinute,
			(info+i)->n_t_time.wSecond), "n_t_time");
		int type = (info+i)->n_t_type;
		dictionary->setObject(Integer::create((info+i)->n_t_type), "n_t_type");
		MyNSString::wc2mbs((info+i)->s_t_comment, 126, s_t_comment);
		dictionary->setObject(String::createWithFormat("%s", s_t_comment), "s_t_comment");
		data->addObject(dictionary);
	}

	NotificationCenter::getInstance()->postNotification("GetXJYKLogRet", data);
	return true;
}

//查询下级盈亏统计日志数返回      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithGetXJYKTjCountRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetXJYKTjCountRet *info = (CMD_GR_GetXJYKTjCountRet *)cbBuffer;
	memcpy(info, buffer, size);

	NotificationCenter::getInstance()->postNotification("GetXJYKTjCountRet", Integer::create(info->dwCount));

	return true;
}

//关系
bool MsgDispatch::OnSocketWithGetParentRelativeRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GetParentRet *info = (CMD_GetParentRet *)cbBuffer;
	memcpy(info, buffer, size);
	
	Array *data = Array::create();
	for(int i = 0 ; i < 10 ; i++){
		//没有值
		if(info->n_t_userid[i] == 0) break;

		Dictionary *dictionary = Dictionary::create();	
		dictionary->setObject(Integer::create(info->n_t_userid[i]), "n_t_userid");
		char s_t_desc[127];
		MyNSString::wc2mbs(info->s_t_desc[i], 126, s_t_desc);
		dictionary->setObject(String::createWithFormat("%s", s_t_desc), "s_t_desc");

		data->addObject(dictionary);
	}

	NotificationCenter::getInstance()->postNotification("GetParentRelativeRet", data);
	return true;
}

//游戏
bool MsgDispatch::OnSocketWithGetGameKindRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetQipaiKindRet *info = (CMD_GR_GetQipaiKindRet *)cbBuffer;
	memcpy(info, buffer, size);

	int count = size/sizeof(CMD_GR_GetQipaiKindRet);
	Array *data = Array::create();
	for(int i = 0 ; i < count ; i++){
		Dictionary *dictionary = Dictionary::create();
		dictionary->setObject(Integer::create((info+i)->n_t_kindid), "n_t_kindid");

		char s_t_Name[34];
		MyNSString::wc2mbs((info+i)->s_t_Name, 33, s_t_Name);
		dictionary->setObject(String::createWithFormat("%s", s_t_Name), "s_t_Name");		

		data->addObject(dictionary);
	}

	NotificationCenter::getInstance()->postNotification("GetGameKindRet", data);
	return true;
}

//查询下级盈亏统计返回     add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithGetXJYKTjRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetXJYKTjRet *info = (CMD_GR_GetXJYKTjRet *)cbBuffer;
	memcpy(info, buffer, size);
	char s_t_account[120];
	int count = size/sizeof(CMD_GR_GetXJYKTjRet);
	Array *data = Array::create();
	for(int i = 0 ; i < count ; i++){
		memset(s_t_account, 0, sizeof(s_t_account));
		Dictionary *dictionary = Dictionary::create();
		dictionary->setObject(Integer::create((info+i)->n_t_id), "n_t_id");
		MyNSString::wc2mbs((info+i)->s_t_account, 33, s_t_account);
		dictionary->setObject(String::createWithFormat("%s", s_t_account), "s_t_account");
		dictionary->setObject(Integer::create((info+i)->n_t_type), "n_t_type");
		dictionary->setObject(Integer::create((info+i)->n_t_permillage), "n_t_permillage");
		dictionary->setObject(Double::create((info+i)->f_t_chongzhi_ze), "f_t_chongzhi_ze");
		dictionary->setObject(Double::create((info+i)->f_t_qukuan_ze), "f_t_qukuan_ze");
		dictionary->setObject(Double::create((info+i)->f_t_touzhu_ze), "f_t_touzhu_ze");
		dictionary->setObject(Double::create((info+i)->f_t_zhongjiang_ze), "f_t_zhongjiang_ze");
		dictionary->setObject(Double::create((info+i)->f_t_fandian_ze), "f_t_fandian_ze");
		//代理的个人返点 和 下级返点累加
		dictionary->setObject(Double::create((info + i)->f_t_xjfandian_ze + (info + i)->f_t_fandian_ze), "f_t_xjfandian_ze");
		dictionary->setObject(Double::create((info+i)->f_t_huodong_ze), "f_t_huodong_ze");
		dictionary->setObject(Double::create((info+i)->f_t_tuandui_ye), "f_t_tuandui_ye");
		double dbYingkui = 0.0f;
		dbYingkui += (info + i)->f_t_touzhu_ze;
		dbYingkui += (info + i)->f_t_zhongjiang_ze;
		dbYingkui += (info + i)->f_t_fandian_ze;
		dbYingkui += (info + i)->f_t_xjfandian_ze;
		dbYingkui += (info + i)->f_t_huodong_ze;
		dictionary->setObject(Double::create(dbYingkui), "f_t_yingkui_ye");
		//新加
		dictionary->setObject(Double::create((info + i)->f_t_qipai_yk),"f_t_qipai_yk");
		dictionary->setObject(Double::create((info + i)->f_t_qipai_fd),"f_t_qipai_fd");

		data->addObject(dictionary);
	}

	NotificationCenter::getInstance()->postNotification("GetXJYKTjRet", data);
	return true;
}

//查询下级充值日志数返回      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithGetXjChzhLogCountRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetXjChzhLogCountRet *info = (CMD_GR_GetXjChzhLogCountRet *)cbBuffer;
	memcpy(info, buffer, size);

	NotificationCenter::getInstance()->postNotification("GetLogCount", Integer::create(info->dwCount));

	return true;
}

//查询下级充值返回      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithGetXJCHZHLogRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetXJCHZHLogRet *info = (CMD_GR_GetXJCHZHLogRet *)cbBuffer;
	memcpy(info, buffer, size);
	char s_t_account[120];
	char s_t_pingtai[120];
	int count = size/sizeof(CMD_GR_GetXJCHZHLogRet);
	Array *data = Array::create();
	for(int i = 0 ; i < count ; i++){
		memset(s_t_account, 0, sizeof(s_t_account));
		memset(s_t_pingtai, 0, sizeof(s_t_pingtai));
		Dictionary *dictionary = Dictionary::create();

		dictionary->setObject(Integer::create((info+i)->n_t_id), "n_t_id");
		dictionary->setObject(Integer::create((info + i)->n_t_user_id), "n_t_user_id");
		MyNSString::wc2mbs((info+i)->s_t_account, 33, s_t_account);
		dictionary->setObject(String::createWithFormat("%s", s_t_account), "s_t_account");
		dictionary->setObject(Double::create((info+i)->f_t_jine), "f_t_jine");
		dictionary->setObject(String::createWithFormat("%04d-%02d-%02d %02d:%02d:%02d", (info+i)->n_t_time.wYear,
			(info+i)->n_t_time.wMonth, (info+i)->n_t_time.wDay, (info+i)->n_t_time.wHour, (info+i)->n_t_time.wMinute,
			(info+i)->n_t_time.wSecond), "time");
		dictionary->setObject(Integer::create((info+i)->n_t_state), "n_t_state");
		MyNSString::wc2mbs((info+i)->s_t_pingtai, 30, s_t_pingtai);
		dictionary->setObject(String::createWithFormat("%s", s_t_pingtai), "s_t_pingtai");
		data->addObject(dictionary);
	}

	NotificationCenter::getInstance()->postNotification("GetXJCZLogRet", data);
	return true;
}

//查询下级提现日志数返回      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithGetXjTxLogCountRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetXjTxLogCountRet *info = (CMD_GR_GetXjTxLogCountRet *)cbBuffer;
	memcpy(info, buffer, size);

	NotificationCenter::getInstance()->postNotification("GetXjTxLogCountRet", Integer::create(info->dwCount));

	return true;
}

//查询下级提现返回      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithGetXJTxLogRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetXJTxLogRet *info = (CMD_GR_GetXJTxLogRet *)cbBuffer;
	memcpy(info, buffer, size);
	char s_t_account[120];
	char s_t_yinhang[120];
	char s_t_kaihuiren[120];
	char s_t_note[120];
	int count = size/sizeof(CMD_GR_GetXJTxLogRet);
	Array *data = Array::create();
	for(int i = 0 ; i < count ; i++){
		memset(s_t_account, 0, sizeof(s_t_account));
		memset(s_t_yinhang, 0, sizeof(s_t_yinhang));
		memset(s_t_kaihuiren, 0, sizeof(s_t_kaihuiren));
		memset(s_t_note, 0, sizeof(s_t_note));
		Dictionary *dictionary = Dictionary::create();
		dictionary->setObject(Integer::create((info+i)->n_t_id), "n_t_id");
		dictionary->setObject(Integer::create((info + i)->n_t_user_id), "n_t_user_id");
		MyNSString::wc2mbs((info+i)->s_t_account, sizeof((info+i)->s_t_account), s_t_account);
		dictionary->setObject(String::createWithFormat("%s", s_t_account), "s_t_account");
		dictionary->setObject(String::createWithFormat("%04d-%02d-%02d %02d:%02d:%02d", (info+i)->n_t_time.wYear,
			(info+i)->n_t_time.wMonth, (info+i)->n_t_time.wDay, (info+i)->n_t_time.wHour, (info+i)->n_t_time.wMinute,
			(info+i)->n_t_time.wSecond), "time");
		MyNSString::wc2mbs((info+i)->s_t_yinhang, sizeof((info+i)->s_t_yinhang), s_t_yinhang);
		dictionary->setObject(String::createWithFormat("%s", s_t_yinhang), "s_t_yinhang");
		dictionary->setObject(Double::create((info+i)->f_t_jine), "f_t_jine");
		MyNSString::wc2mbs((info+i)->s_t_kaihuiren, sizeof((info+i)->s_t_kaihuiren), s_t_kaihuiren);
		dictionary->setObject(String::createWithFormat("%s", s_t_kaihuiren), "s_t_kaihuiren");
		dictionary->setObject(Integer::create((info+i)->n_t_state), "n_t_state");
		MyNSString::wc2mbs((info+i)->s_t_note, sizeof((info+i)->s_t_note), s_t_note);
		dictionary->setObject(String::createWithFormat("%s", s_t_note), "s_t_note");
		data->addObject(dictionary);
	}

	NotificationCenter::getInstance()->postNotification("GetXJTxLogRet", data);
	return true;
}

//获取提现记录数返回      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithGetTiXianLogCountRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetTixianLogCountRet *info = (CMD_GR_GetTixianLogCountRet *)cbBuffer;
	memcpy(info, buffer, size);

	NotificationCenter::getInstance()->postNotification("GetLogCount", Integer::create(info->dwCount));

	return true;
}

//获取提现信息返回      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithGetTiXianLogRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetTixianLogRet *info = (CMD_GR_GetTixianLogRet *)cbBuffer;
	memcpy(info, buffer, size);
	char n_t_yinhang[120] = {0};
	char s_t_kaihuren[120] = {0};
	char s_t_note[120] = {0};
	int count = size/sizeof(CMD_GR_GetTixianLogRet);
	Array *data = Array::create();
	for(int i = 0 ; i < count ; i++){
		Dictionary *dictionary = Dictionary::create();
		dictionary->setObject(String::createWithFormat("%04d-%02d-%02d %02d:%02d:%02d", (info+i)->n_t_time.wYear,
			(info+i)->n_t_time.wMonth, (info+i)->n_t_time.wDay, (info+i)->n_t_time.wHour, (info+i)->n_t_time.wMinute,
			(info+i)->n_t_time.wSecond), "time");
		dictionary->setObject(Integer::create((info+i)->n_t_id), "n_t_id");
		MyNSString::wc2mbs((info+i)->n_t_yinhang, 30, n_t_yinhang);
		dictionary->setObject(String::createWithFormat("%s", n_t_yinhang), "n_t_yinhang");
		dictionary->setObject(Double::create((info+i)->f_t_jine), "f_t_jine");
		MyNSString::wc2mbs((info+i)->s_t_kaihuren, 30, s_t_kaihuren);
		dictionary->setObject(String::createWithFormat("%s", s_t_kaihuren), "s_t_kaihuren");
		dictionary->setObject(Integer::create((info+i)->n_t_state), "n_t_state");
		MyNSString::wc2mbs((info+i)->s_t_note, 30, s_t_note);
		dictionary->setObject(String::createWithFormat("%s", s_t_note), "s_t_note");
		data->addObject(dictionary);
	}

	NotificationCenter::getInstance()->postNotification("GetLog", data);
	return true;
}

//获取充值记录数返回      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithGetChongzhiLogCountRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetChongzhiLogCountRet *info = (CMD_GR_GetChongzhiLogCountRet *)cbBuffer;
	memcpy(info, buffer, size);

	NotificationCenter::getInstance()->postNotification("GetLogCount", Integer::create(info->dwCount));

	return true;
}

//获取充值信息返回      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithGetChongzhiLogRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetChongzhiLogRet *info = (CMD_GR_GetChongzhiLogRet *)cbBuffer;
	memcpy(info, buffer, size);
	char s_t_pingtai[120] = {0};
	int count = size/sizeof(CMD_GR_GetChongzhiLogRet);
	Array *data = Array::create();
	for(int i = 0 ; i < count ; i++){
		Dictionary *dictionary = Dictionary::create();
		dictionary->setObject(String::createWithFormat("%04d-%02d-%02d %02d:%02d:%02d", (info+i)->n_t_time.wYear,
			(info+i)->n_t_time.wMonth, (info+i)->n_t_time.wDay, (info+i)->n_t_time.wHour, (info+i)->n_t_time.wMinute,
			(info+i)->n_t_time.wSecond), "time");
		dictionary->setObject(Integer::create((info+i)->n_t_id), "n_t_id");
		dictionary->setObject(Double::create((info+i)->f_t_jine), "f_t_jine");
		dictionary->setObject(Integer::create((info+i)->n_t_state), "n_t_state");
		MyNSString::wc2mbs((info+i)->s_t_pingtai, 30, s_t_pingtai);
		dictionary->setObject(String::createWithFormat("%s", s_t_pingtai), "s_t_pingtai");
		data->addObject(dictionary);
	}

	NotificationCenter::getInstance()->postNotification("GetLog", data);
	return true;
}

//获取盈亏记录数返回      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithGetYingkuiLogCountRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetYingkuiLogCountRet *info = (CMD_GR_GetYingkuiLogCountRet *)cbBuffer;
	memcpy(info, buffer, size);

	NotificationCenter::getInstance()->postNotification("GetLogCount", Integer::create(info->dwCount));

	return true;
}

//获取盈亏信息返回      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithGetYingkuiLogRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetYingkuiLogRet *info = (CMD_GR_GetYingkuiLogRet *)cbBuffer;
	memcpy(info, buffer, size);
	char s_t_pingtai[120] = {0};
	int count = size/sizeof(CMD_GR_GetYingkuiLogRet);
	Array *data = Array::create();
	for(int i = 0 ; i < count ; i++){
		Dictionary *dictionary = Dictionary::create();
		dictionary->setObject(Double::create((info+i)->f_t_chongzhi), "f_t_chongzhi");
		dictionary->setObject(Double::create((info+i)->f_t_qukuan), "f_t_qukuan");
		dictionary->setObject(Double::create((info+i)->f_t_touzhu), "f_t_touzhu");
		dictionary->setObject(Double::create((info+i)->f_t_fandian), "f_t_fandian");
		dictionary->setObject(Double::create((info+i)->f_t_zhongjiang), "f_t_zhongjiang");
		dictionary->setObject(Double::create((info+i)->f_t_huodong), "f_t_huodong");
		dictionary->setObject(Double::create((info+i)->f_t_chedan), "f_t_chedan");
		data->addObject(dictionary);
	}

	NotificationCenter::getInstance()->postNotification("GetLog", data);
	return true;
}

//获取棋牌盈亏记录数返回      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithGetQiPaiYingkuiLogCountRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetQiPaiYingkuiCountRet *info = (CMD_GR_GetQiPaiYingkuiCountRet *)cbBuffer;
	memcpy(info, buffer, size);

	NotificationCenter::getInstance()->postNotification("GetLogCount", Integer::create(info->dwCount));

	return true;
}

//获取棋牌盈亏信息返回      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithGetQiPaiYingkuiLogRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetQiPaiYingkuiRet *info = (CMD_GR_GetQiPaiYingkuiRet *)cbBuffer;
	memcpy(info, buffer, size);
	char s_t_servername[120] = {0};
	char s_t_accounts[120] = {0};
	int count = size/sizeof(CMD_GR_GetQiPaiYingkuiRet);
	Array *data = Array::create();
	for(int i = 0 ; i < count ; i++){
		Dictionary *dictionary = Dictionary::create();
		dictionary->setObject(Integer::create((info+i)->n_t_userid), "n_t_userid");
		dictionary->setObject(Integer::create((info+i)->n_t_drawid), "n_t_drawid");
		dictionary->setObject(Double::create((info+i)->f_t_score), "f_t_score");
		dictionary->setObject(Integer::create((info+i)->n_t_playcount), "n_t_playcount");
		dictionary->setObject(Double::create((info+i)->f_t_revenue), "f_t_revenue");
		dictionary->setObject(String::createWithFormat("%04d-%02d-%02d %02d:%02d:%02d", (info+i)->n_t_playtime.wYear,
			(info+i)->n_t_playtime.wMonth, (info+i)->n_t_playtime.wDay, (info+i)->n_t_playtime.wHour,
			(info+i)->n_t_playtime.wMinute,(info+i)->n_t_playtime.wSecond), "time");
		MyNSString::wc2mbs((info+i)->s_t_servername, 30, s_t_servername);
		dictionary->setObject(String::createWithFormat("%s", s_t_servername), "s_t_servername");
		MyNSString::wc2mbs((info+i)->s_t_accounts, 30, s_t_accounts);
		dictionary->setObject(String::createWithFormat("%s", s_t_accounts), "s_t_accounts");
		data->addObject(dictionary);
	}

	NotificationCenter::getInstance()->postNotification("GetLog", data);
	return true;
}


// 下注返回      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithTouZhuContinueRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_TouzhuRes *info = (CMD_GR_TouzhuRes *)cbBuffer;
	memcpy(info, buffer, size);

	char	resultDesc[2048] = {0};
	MyNSString::wc2mbs(info->szDesc, sizeof(info->szDesc), resultDesc);
	Array *data = Array::create();
	data->addObject(Integer::create(info->lResult));
	data->addObject(String::createWithFormat("%s", resultDesc));
	data->addObject(Integer::create(info->nSign));

	NotificationCenter::getInstance()->postNotification("CQSSCRetZhuiHao", data);

	return true;
}

//获取投注返回      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithGetTouZhuXXRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GP_GetTouzhuRet *info = (CMD_GP_GetTouzhuRet *)cbBuffer;
	memcpy(info, buffer, size);

	char qiShu[140] = {0};
	char kjhm[160] = {0};	
	MyNSString::wc2mbs(info->s_t_qishu, sizeof(info->s_t_qishu), qiShu);
	if(info->n_t_state == 1){
		MyNSString::wc2mbs(info->s_t_kaijianghaoma, sizeof(info->s_t_kaijianghaoma), kjhm);
	}

	/************************************************************************/
	/* 注释部分为解压算法       总共显示字符不超过1W个，没必要浪费时间在这上面                                                           */
	/************************************************************************/
	//int pos = 13312 - 1; //总长度13312 info->s_t_touzhuhaoma
	//while(info->s_t_touzhuhaoma[pos --] == 0); pos += 2;

	//BYTE cbBuffer2[800000];
	//memset(cbBuffer2, 0, 800000);
	//size_t tempSize = 5;
	//size_t inSize = pos;
	//size_t outSize = 800000;

	//HDLzma::UncompressBuffer(info->s_t_touzhuhaoma,&inSize, cbBuffer2,&outSize,&tempSize);

	Array *data = Array::create();
	//data->addObject(String::createWithData(cbBuffer2, tempSize));
	data->addObject(String::createWithFormat("%s",info->s_t_touzhuhaoma));
	data->addObject(String::createWithFormat("%d", info->n_t_dingdanhao));
	data->addObject(String::create(qiShu));										//2
	data->addObject(String::createWithFormat("%d", info->n_t_gametype));			//3
	data->addObject(String::createWithFormat("%d", info->n_t_gamekind));			//4
	data->addObject(String::create(kjhm));										//index=5
	data->addObject(String::createWithFormat("%02d-%02d-%02d %02d:%02d:%02d", info->n_t_time.wYear,
		info->n_t_time.wMonth, info->n_t_time.wDay, info->n_t_time.wHour, info->n_t_time.wMinute,
		info->n_t_time.wSecond));
	data->addObject(String::createWithFormat("%d", info->n_t_state));

	data->addObject(String::createWithFormat("%d", info->n_t_zhushu));
	data->addObject(String::createWithFormat("%d", info->n_t_moshi));
	data->addObject(String::createWithFormat("%d", info->n_t_beishu));			//index=10
	
	//单注金额：
	float danZhuJine = 2;
	double danJiangJine = info->f_t_danzhujine;
	if (info->n_t_moshi == 1){
		danZhuJine = 0.2;
		danJiangJine *= 0.1;
	}
	else if (info->n_t_moshi == 2){
		danZhuJine = 0.02;
		danJiangJine *= 0.01;
	}
	else if (info->n_t_moshi == 3)
	{
		danZhuJine = 0.002;
		danJiangJine *= 0.001;
	}

	if(info->n_t_gametype == CZ_LUCKY_28 || info->n_t_gametype == CZ_HK6HeCai)
	{
		danZhuJine = 1;	
		danJiangJine = info->f_t_danzhujine;
	}

	data->addObject(String::createWithFormat("%.3f", info->n_t_zhushu*info->n_t_beishu*danZhuJine)); //投注总额
	data->addObject(String::createWithFormat("%d", info->n_t_winzhushu));
	data->addObject(String::createWithFormat("%.3lf", danJiangJine));
	data->addObject(String::createWithFormat("%.3lf", info->f_t_yingkui));
	data->addObject(String::createWithFormat("%.3lf", info->f_t_fandian));			//index=15 返点
	//index=16: 返点金额

	data->addObject(String::createWithFormat("%.3f", info->n_t_zhushu*info->n_t_beishu* danZhuJine *info->f_t_fandian*0.01f));
	
	data->addObject(String::createWithFormat("%.3lf", info->f_t_yingkui));


	NotificationCenter::getInstance()->postNotification("GetTouZhuXXRet", data);

	return true;
}

//获取客服ULR
bool MsgDispatch::OnSocketWithGetKeFuURLRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GetKefuUrlRet *info = (CMD_GetKefuUrlRet *)cbBuffer;
	memcpy(info, buffer, size);	

	Dictionary *data = Dictionary::create();
	data->setObject(Integer::create(info->n_t_result), "result");
	char kfurl[100] = {0};	
	MyNSString::wc2mbs(info->s_t_desc, sizeof(info->s_t_desc), kfurl);	
	data->setObject(String::createWithFormat("%s", kfurl), "desc");

	NotificationCenter::getInstance()->postNotification("GetKeFuUrl", data);

	return true;
}

bool MsgDispatch::OnSocketWithGetValidateCodeRet(void *buffer,WORD size)
{
	//CMD_GR_SendYanZhengmaRet
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_SendYanZhengmaRet *info = (CMD_GR_SendYanZhengmaRet *)cbBuffer;
	memcpy(info, buffer, size);

	Dictionary *data = Dictionary::create();
	data->setObject(Integer::create(info->nResult), "nResult");
	char sDesc[200] = {0};	
	MyNSString::wc2mbs(info->sDesc, sizeof(info->sDesc), sDesc);	
	data->setObject(String::createWithFormat("%s", sDesc), "sDesc");

	NotificationCenter::getInstance()->postNotification("ValidateCodeReback", data);	

	return true;
}

bool MsgDispatch::OnSocketWithGetTransPhoneRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetPhoneTransferFenhongRet *info = (CMD_GR_GetPhoneTransferFenhongRet *)cbBuffer;
	memcpy(info, buffer, size);

	Dictionary *data = Dictionary::create();
	data->setObject(Integer::create(info->n_t_state), "n_t_state");
	char sPhoneNum[200] = {0};	
	MyNSString::wc2mbs(info->sPhoneNum, sizeof(info->sPhoneNum), sPhoneNum);	
	data->setObject(String::createWithFormat("%s", sPhoneNum), "sPhoneNum");

	NotificationCenter::getInstance()->postNotification("GetTransPhoneRet", data);	

	return true;
}

bool MsgDispatch::OnSocketWithBindPhoneRet(void *buffer,WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_BindPhone_RET *info = (CMD_GR_BindPhone_RET *)cbBuffer;
	memcpy(info, buffer, size);	

	NotificationCenter::getInstance()->postNotification("BindPhoneNumberRet", Integer::create(info->lResult));

	return true;
}

bool MsgDispatch::OnSocketWithUnBindPhoneRet(void *buffer,WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_UnBindPhone_RET *info = (CMD_GR_UnBindPhone_RET *)cbBuffer;
	memcpy(info, buffer, size);	

	Dictionary *data = Dictionary::create();
	data->setObject(Integer::create(info->nResult), "nResult");		
	char sDesc[128] = {0};	
	MyNSString::wc2mbs(info->sDesc, sizeof(info->sDesc), sDesc);	
	data->setObject(String::createWithFormat("%s", sDesc), "sDesc");

	NotificationCenter::getInstance()->postNotification("UnBindPhoneNumberRet", data);

	return true;
}

bool MsgDispatch::OnSocketWithGetValidateTypeRet(void *buffer,WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_BindPhoneInfo_RET *info = (CMD_GR_BindPhoneInfo_RET *)cbBuffer;
	memcpy(info, buffer, size);	

	Dictionary *data = Dictionary::create();
	data->setObject(Integer::create(info->nResult), "nResult");	
	data->setObject(Integer::create(info->cfenhong), "cfenhong");
	data->setObject(Integer::create(info->cLoginverify), "cLoginverify");
	data->setObject(Integer::create(info->cTranferAccount),"cTranferAccount");
	char phone[100] = {0};	
	MyNSString::wc2mbs(info->sPhoneNum, sizeof(info->sPhoneNum), phone);	
	data->setObject(String::createWithFormat("%s", phone), "phone");

	NotificationCenter::getInstance()->postNotification("ValidateTypeRet", data);

	return true;
}

//获取系统时间返回      add by wangjun 2015-05-19
bool MsgDispatch::OnSocketWithGetSystemTimeRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetSystemTime *info = (CMD_GR_GetSystemTime *)cbBuffer;
	memcpy(info, buffer, size);

	//Dictionary *data = Dictionary::create();
	//data->setObject(Integer::create(info->ServerTime.wYear), "wYear");
	//data->setObject(Integer::create(info->ServerTime.wMonth), "wMonth");
	//data->setObject(Integer::create(info->ServerTime.wDay), "wDay");
	//data->setObject(Integer::create(info->ServerTime.wHour), "wHour");
	//data->setObject(Integer::create(info->ServerTime.wMinute), "wMinute");
	//data->setObject(Integer::create(info->ServerTime.wSecond), "wSecond");
	//data->setObject(Integer::create(info->ServerTime.wMilliseconds), "wMilliseconds");

	NotificationCenter::getInstance()->postNotification("GetSystemTimeRet");

	current_time_t = SystemTimeToTime_t(info->ServerTime);

	time_t t_local = time(NULL);
	m_nDeltaTime = current_time_t - t_local ;				//获取差值。
	return true;
}


//撤单返回      add by wangjun 2015-08-19
bool MsgDispatch::OnSocketWithCancelTouZhuRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_CancelTouzhuRet *info = (CMD_GR_CancelTouzhuRet *)cbBuffer;
	memcpy(info, buffer, size);


	NotificationCenter::getInstance()->postNotification("CancelTouZhuRet", Integer::create(info->nResult));

	return true;
}

//充值信息查询返回      add by wangjun 2015-08-19
bool MsgDispatch::OnSocketWithGetChongzhiTypeRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GP_GetChongzhiTypeRet *info = (CMD_GP_GetChongzhiTypeRet *)cbBuffer;
	memcpy(info, buffer, size);

	int count = size/sizeof(CMD_GP_GetChongzhiTypeRet);
	char	s_t_yinhang[256];				//银行
	char	s_t_kaihuren[256];				//开户人
	char	s_t_zhanghao[256];				//账号
	char	s_t_web[256];					//网页
	char    s_t_desc[500];					//描述
	int		n_t_typeid=0;						//typeid
	Array *data = Array::create();
	for(int i = 0 ; i < count ; i++){
		memset(s_t_yinhang, 0, sizeof(s_t_yinhang));
		memset(s_t_kaihuren, 0, sizeof(s_t_kaihuren));
		memset(s_t_zhanghao, 0, sizeof(s_t_zhanghao));
		memset(s_t_web, 0, sizeof(s_t_web));
		memset(s_t_desc, 0, sizeof(s_t_desc));
		int size = sizeof((info+i)->s_t_yinhang);
		MyNSString::wc2mbs((info+i)->s_t_yinhang, sizeof((info+i)->s_t_yinhang), s_t_yinhang);
		MyNSString::wc2mbs((info+i)->s_t_kaihuren, sizeof((info+i)->s_t_kaihuren), s_t_kaihuren);
		MyNSString::wc2mbs((info+i)->s_t_zhanghao, sizeof((info+i)->s_t_zhanghao), s_t_zhanghao);
		MyNSString::wc2mbs((info+i)->s_t_web, sizeof((info+i)->s_t_web), s_t_web);
		MyNSString::wc2mbs((info+i)->s_t_remark, sizeof((info+i)->s_t_remark), s_t_desc);
		n_t_typeid = (info+i)->n_t_typeid;

		if(strcmp(s_t_yinhang, "") == 0){
			continue;
		}
		Dictionary *dic = Dictionary::create();
		dic->setObject(String::create(s_t_yinhang), "s_t_yinhang");
		dic->setObject(String::create(s_t_kaihuren), "s_t_kaihuren");
		dic->setObject(String::create(s_t_zhanghao), "s_t_zhanghao");
		dic->setObject(String::create(s_t_web), "s_t_web");
		dic->setObject(String::create(s_t_desc), "s_t_desc");
		dic->setObject(Integer::create(n_t_typeid), "n_t_typeid");
		data->addObject(dic);
	}

	NotificationCenter::getInstance()->postNotification("GetChongzhiTypeRet", data);

	return true;
}

//提款信息查询返回      add by wangjun 2015-08-19
bool MsgDispatch::OnSocketWithGetQukuanInfoRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GP_GetQukuanInfoRet *info = (CMD_GP_GetQukuanInfoRet *)cbBuffer;
	memcpy(info, buffer, size);

	//NotificationCenter::getInstance()->postNotification("GetChongzhiTypeRet", data);

	return true;
}

//提款返回      add by wangjun 2015-08-19
bool MsgDispatch::OnSocketWithDoQukuanRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GP_DoQukuanRet *info = (CMD_GP_DoQukuanRet *)cbBuffer;
	memcpy(info, buffer, size);

	Dictionary* dic = Dictionary::create();

	dic->setObject(Integer::create(info->nResult), "nResult");

	char szDesc[400] = {0};
	MyNSString::wc2mbs(info->szDesc, CountArray(info->szDesc), szDesc);

	dic->setObject(String::create(szDesc),"szDesc");

	NotificationCenter::getInstance()->postNotification("DoQukuanRet", dic);

	return true;
}

//取款限制
bool MsgDispatch::onSocketWithQuKuanLimitRet(void *buffer, WORD size)
{
	//CMD_GP_GetQukuanLimitRet
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GP_GetQukuanLimitRet *info = (CMD_GP_GetQukuanLimitRet *)cbBuffer;
	memcpy(info, buffer, size);

	char szQuKuanShuoMing[256] = {0};
	MyNSString::wc2mbs(info->s_t_remark, CountArray(info->s_t_remark), szQuKuanShuoMing);

	Dictionary *data = Dictionary::create();
	data->setObject(Double::create(info->f_t_nowxiaofei),"f_t_nowxiaofei");
	data->setObject(Double::create(info->f_t_xiaofei),"f_t_xiaofei");
	data->setObject(Double::create(info->f_t_lowjine),"f_t_lowjine");
	data->setObject(Double::create(info->f_t_highjine),"f_t_highjine");
	data->setObject(Integer::create(info->n_t_sxfpercent), "n_t_sxfpercent");
	data->setObject(Double::create(info->f_t_lowsxf),"f_t_lowsxf");
	data->setObject(Double::create(info->f_t_highsxf),"f_t_highsxf");
	data->setObject(Integer::create(info->n_t_limitcount), "n_t_limitcount");
	data->setObject(Integer::create(info->n_t_lastcount), "n_t_lastcount");
	data->setObject(Integer::create(info->n_t_freelastcount), "n_t_freelastcount");
	data->setObject(Integer::create(info->n_t_allcount), "n_t_allcount");
	data->setObject(String::create(szQuKuanShuoMing), "s_t_remark");
	data->setObject(Integer::create(info->t_starttime), "t_starttime");
	data->setObject(Integer::create(info->t_endtime), "t_endtime");
	
	NotificationCenter::getInstance()->postNotification("GetQuKuanLimitRet", data);
	
	return true;
}

//绑卡返回      add by wangjun 2015-08-19
bool MsgDispatch::OnSocketWithSetQukuanZhanghaoRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_SetQukuanZhanghao_RET *info = (CMD_GR_SetQukuanZhanghao_RET *)cbBuffer;
	memcpy(info, buffer, size);

	NotificationCenter::getInstance()->postNotification("SetQukuanZhanghaoRet", Integer::create(info->lResult));

	return true;
}

//绑卡查询返回      add by wangjun 2015-08-19
bool MsgDispatch::OnSocketWithQueryMyYinHangRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_QueryMyYinHang_RET *info = (CMD_GR_QueryMyYinHang_RET *)cbBuffer;
	memcpy(info, buffer, size);

	char szKaihuYinghang[167] = {0};
	char szKaihuRen[167] = {0};
	char szYinhangZhanghu[167] = {0};
	MyNSString::wc2mbs(info->szKaihuYinghang, CountArray(info->szKaihuYinghang), szKaihuYinghang);
	MyNSString::wc2mbs(info->szKaihuRen, CountArray(info->szKaihuRen), szKaihuRen);
	MyNSString::wc2mbs(info->szYinhangZhanghu, CountArray(info->szYinhangZhanghu), szYinhangZhanghu);

		
	Dictionary *data = Dictionary::create();
	data->setObject(Integer::create(info->lResult), "lResult");
	data->setObject(String::create(szKaihuYinghang), "szKaihuYinghang");
	data->setObject(String::create(szKaihuRen), "szKaihuRen");
	data->setObject(String::create(szYinhangZhanghu), "szYinhangZhanghu");
	data->setObject(Integer::create(info->m_lock), "m_lock");
	NotificationCenter::getInstance()->postNotification("QueryMyYinHangRet", data);

	return true;
}
//设置返点返回结果
bool MsgDispatch::OnSocketWithQueryYinHangRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_QueryYinhang_RET *info = (CMD_GR_QueryYinhang_RET *)cbBuffer;
	memcpy(info, buffer, size);

	int count = size/sizeof(CMD_GR_QueryYinhang_RET);
	char	bankType[256];				//开户银行			
	Array *data = Array::create();
	for(int i = 0 ; i < count ; i++){
		memset(bankType, 0, sizeof(bankType));

		int size = sizeof((info+i)->szYinHangName);
		MyNSString::wc2mbs((info+i)->szYinHangName, sizeof((info+i)->szYinHangName), bankType);
		
		if(strcmp(bankType, "") == 0){
			continue;
		}
		Dictionary *dic = Dictionary::create();
		dic->setObject(String::create(bankType), "bankType");		
		data->addObject(dic);
	}

	NotificationCenter::getInstance()->postNotification("QueryBankTypeRet", data);

	return true;
}

bool MsgDispatch::OnSocketWithSetusedBonusRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_SetUserBonusRet  *info = (CMD_GR_SetUserBonusRet  *)cbBuffer;
	memcpy(info, buffer, size);
	NotificationCenter::getInstance()->postNotification("SetusedBonusRet", Integer::create(info->n_t_result));
	return true;
}
//转换返回
bool MsgDispatch::OnSocketWithZhuanHuanRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GP_ZhuanHuanRet  *info = (CMD_GP_ZhuanHuanRet  *)cbBuffer;
	memcpy(info, buffer, size);
	f_qipai_yue = info->f_t_score;
	f_yue = info->f_t_yue;
	Dictionary *dic = Dictionary::create();
	dic->setObject(Double::create(info->f_t_score),"f_t_score");
	dic->setObject(Double::create(info->f_t_yue),"f_t_yue");
	dic->setObject(Integer::create(info->nResult),"nResult");
	NotificationCenter::getInstance()->postNotification("ZhuanHuanRet", dic);
	return true;
}

//获取个人基本信息返回
bool MsgDispatch::OnSocketWithGetUserInfoRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetUserInfoRet  *info = (CMD_GR_GetUserInfoRet  *)cbBuffer;
	memcpy(info, buffer, size);
	char	s_t_qq[256] = {0};			
	char	s_t_sjqq[256] = {0};					
	Dictionary *data = Dictionary::create();
	MyNSString::wc2mbs(info->s_t_qq, sizeof(info->s_t_qq), s_t_qq);
	data->setObject(String::create(s_t_qq), "s_t_qq");
	MyNSString::wc2mbs(info->s_t_sjqq, sizeof(info->s_t_sjqq), s_t_sjqq);
	data->setObject(String::create(s_t_sjqq), "s_t_sjqq");

	data->setObject(Double::create(info->f_t_yongjin), "f_t_yongjin");
	data->setObject(Double::create(info->f_t_yongjinze), "f_t_yongjinze");
	data->setObject(Double::create(info->f_t_fenhong), "f_t_fenhong");
	data->setObject(Double::create(info->f_t_fenhongze), "f_t_fenhongze");
	data->setObject(Double::create(info->f_t_qipaiye), "f_t_qipaiye");
	data->setObject(Double::create(info->f_t_kuisunyongjin), "f_t_kuisunyongjin");
	data->setObject(Double::create(info->f_t_kuisunyongjinze), "f_t_kuisunyongjinze");

	data->setObject(String::createWithFormat("%04d-%02d-%02d %02d:%02d:%02d", info->t_ksyj_time.wYear,
		info->t_ksyj_time.wMonth, info->t_ksyj_time.wDay, info->t_ksyj_time.wHour,
		info->t_ksyj_time.wMinute,info->t_ksyj_time.wSecond), "t_ksyj_time");

	data->setObject(String::createWithFormat("%04d-%02d-%02d %02d:%02d:%02d", info->t_fh_time.wYear,
		info->t_fh_time.wMonth, info->t_fh_time.wDay, info->t_fh_time.wHour,
		info->t_fh_time.wMinute,info->t_fh_time.wSecond), "t_fh_time");

	data->setObject(String::createWithFormat("%04d-%02d-%02d %02d:%02d:%02d", info->t_yj_time.wYear,
		info->t_yj_time.wMonth, info->t_yj_time.wDay, info->t_yj_time.wHour,
		info->t_yj_time.wMinute,info->t_yj_time.wSecond), "t_yj_time");

	NotificationCenter::getInstance()->postNotification("GetUserInfoRet", data);
	return true;
}

//获取最新余额信息返回
bool MsgDispatch::OnSocketWithGetLastYueRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GP_GetLastYueRet  *info = (CMD_GP_GetLastYueRet  *)cbBuffer;
	memcpy(info, buffer, size);
	if(info->f_t_yue > 0){
		f_yue = info->f_t_yue;
	}
	if(info->f_t_score > 0){
		f_qipai_yue = info->f_t_score;
	}
	f_qipai_yue = info->f_t_score;
	Dictionary *dic = Dictionary::create();
	dic->setObject(Double::create(info->f_t_yue), "f_t_yue");
	dic->setObject(Double::create(info->f_t_dongjie), "f_t_dongjie");
	dic->setObject(Double::create(info->f_t_score), "f_t_score");
	NotificationCenter::getInstance()->postNotification("GetLastYueRet", dic);
	return true;
}

bool MsgDispatch::OnSocketWithGetLiuHeFenDan(void *buffer,WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GetLhcQihaoRet  *info = (CMD_GetLhcQihaoRet  *)cbBuffer;
	memcpy(info, buffer, size);
		
	Dictionary *dic = Dictionary::create();
	dic->setObject(Integer::create(info->n_t_qihao), "n_t_qihao");
	//时间只能这么传
	dic->setObject(Integer::create(info->n_t_fdshijian.wYear), "wYear");
	dic->setObject(Integer::create(info->n_t_fdshijian.wMonth), "wMonth");
	dic->setObject(Integer::create(info->n_t_fdshijian.wDay), "wDay");
	dic->setObject(Integer::create(info->n_t_fdshijian.wHour), "wHour");
	dic->setObject(Integer::create(info->n_t_fdshijian.wMinute), "wMinute");
	dic->setObject(Integer::create(info->n_t_fdshijian.wSecond), "wSecond");
	dic->setObject(Integer::create(info->n_t_fdshijian.wMilliseconds), "wMilliseconds");

	NotificationCenter::getInstance()->postNotification("UpdateFenDan", dic);
	return true;
}

bool MsgDispatch::OnSocketWithGetRankListRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GP_GetNewsRet  *info = (CMD_GP_GetNewsRet  *)cbBuffer;
	memcpy(info, buffer, size);	

	int count = size / sizeof(CMD_GP_GetNewsRet);

	Array *dataArray = Array::create();
	for(int i = 0 ; i < count ; i++){
		char s_t_news[512] = {0};

		Dictionary *data = Dictionary::create();
	
		MyNSString::wc2mbs((info + i)->s_t_news, sizeof((info+i)->s_t_news), s_t_news);
		
		data->setObject(String::create(s_t_news), "s_t_news");

		dataArray->addObject(data);
	}

	NotificationCenter::getInstance()->postNotification("UpdateRankList", dataArray);
	return true;
}

//代理领奖
bool MsgDispatch::OnSocketWithDaiLiQianDaoRet(void *buffer,WORD size)
{
	// 
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GP_DailiLingjiangRet  *info = (CMD_GP_DailiLingjiangRet  *)cbBuffer;
	memcpy(info, buffer, size);

	Dictionary* data = Dictionary::create();
	data->setObject(Integer::create(info->n_t_state),"n_t_state");
	data->setObject(Double::create(info->f_t_jine), "f_t_jine");

	NotificationCenter::getInstance()->postNotification("QianDaoRet", data);
	return true;
}

//充值送查询
bool MsgDispatch::OnSocketWithChongZhiSongGetRet(void *buffer,WORD size)
{
	// 
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GP_GetChongZhiSongRet  *info = (CMD_GP_GetChongZhiSongRet  *)cbBuffer;
	memcpy(info, buffer, size);

	Dictionary* data = Dictionary::create();
	data->setObject(Integer::create(info->n_t_state),"n_t_state");
	data->setObject(Double::create(info->f_t_jine), "f_t_jine");

	NotificationCenter::getInstance()->postNotification("ChongZhiSongGetRet", data);
	return true;
}

//充值送领奖
bool MsgDispatch::OnSocketWithChongZhiSongSetRet(void *buffer,WORD size)
{
	// 
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GP_SetChongZhiSongRet  *info = (CMD_GP_SetChongZhiSongRet  *)cbBuffer;
	memcpy(info, buffer, size);

	Dictionary* data = Dictionary::create();
	data->setObject(Integer::create(info->n_t_state),"n_t_state");
	data->setObject(Double::create(info->f_t_jine), "f_t_jine");

	NotificationCenter::getInstance()->postNotification("ChongZhiSongSetRet", data);
	return true;
}

//代理领奖查询
bool MsgDispatch::OnSocketWithQuerryDailiQianDaoRet(void *buffer,WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GP_GetDailiHuikuiRet *info = (CMD_GP_GetDailiHuikuiRet *)cbBuffer;
	memcpy(info, buffer, size);

	Dictionary* data = Dictionary::create();
	data->setObject(Integer::create(info->cbNewRegUser),"cbNewRegUser");
	data->setObject(Double::create(info->f_t_touzhu), "f_t_touzhu");
	data->setObject(Double::create(info->f_t_chongzhi), "f_t_chongzhi");

	NotificationCenter::getInstance()->postNotification("QianDaoInfoRet", data);
	return true;
}

//下级转帐
bool MsgDispatch::OnSocketWithZhuanZhangRet(void *buffer, WORD size)
{
	//CMD_GP_HYXX_ZhuanZhang_RET 直接显示内容 无需判断ret值
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GP_HYXX_ZhuanZhang_RET *info = (CMD_GP_HYXX_ZhuanZhang_RET *)cbBuffer;
	memcpy(info, buffer, size);

	char s_t_Desc[126] = {0};
	MyNSString::wc2mbs(info->s_t_desc, sizeof(info->s_t_desc), s_t_Desc);

	Dictionary *data = Dictionary::create();
	data->setObject(Integer::create(info->lResult), "lResult");
	data->setObject(String::create(s_t_Desc), "s_t_Desc");
	NotificationCenter::getInstance()->postNotification("ZhuanZhangRet", data);

	return true;
}

//下级配额
bool MsgDispatch::OnSocketWithXiajiPeiERet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GetXjPeieRet *info = (CMD_GetXjPeieRet *)cbBuffer;
	memcpy(info, buffer, size);

	//注释掉配额
	return true;



	Dictionary* data = Dictionary::create();

#ifdef VER_QIANBO
	data->setObject(Integer::create(info->n_t_peie_1), "total30");
	data->setObject(Integer::create(info->n_t_peie_2), "total29");
	data->setObject(Integer::create(info->n_t_peie_3), "total28");
	data->setObject(Integer::create(info->n_t_peie_4), "total27");
	data->setObject(Integer::create(info->n_t_peie_5), "total26");
	data->setObject(Integer::create(info->n_t_peie_s_1), "pere30");
	data->setObject(Integer::create(info->n_t_peie_s_2), "pere29");
	data->setObject(Integer::create(info->n_t_peie_s_3), "pere28");
	data->setObject(Integer::create(info->n_t_peie_s_4), "pere27");
	data->setObject(Integer::create(info->n_t_peie_s_5), "pere26");
#endif

#ifdef VER_369
	data->setObject(Integer::create(info->n_t_peie_1), "total30");
	data->setObject(Integer::create(info->n_t_peie_2), "total29");
	data->setObject(Integer::create(info->n_t_peie_3), "total28");
	data->setObject(Integer::create(info->n_t_peie_4), "total27");
	data->setObject(Integer::create(info->n_t_peie_5), "total26");
	data->setObject(Integer::create(info->n_t_peie_s_1), "pere30");
	data->setObject(Integer::create(info->n_t_peie_s_2), "pere29");
	data->setObject(Integer::create(info->n_t_peie_s_3), "pere28");
	data->setObject(Integer::create(info->n_t_peie_s_4), "pere27");
	data->setObject(Integer::create(info->n_t_peie_s_5), "pere26");
#endif // end

#ifdef VER_9YI
	data->setObject(Integer::create(info->n_t_peie_1), "total30");
	data->setObject(Integer::create(info->n_t_peie_2), "total28");
	data->setObject(Integer::create(info->n_t_peie_3), "total27");
	data->setObject(Integer::create(info->n_t_peie_s_1), "pere30");
	data->setObject(Integer::create(info->n_t_peie_s_2), "pere28");
	data->setObject(Integer::create(info->n_t_peie_s_3), "pere27");
#endif //

	NotificationCenter::getInstance()->postNotification("XiaJiPeiERet", data);
	return true;
}

//设置下级配额
bool MsgDispatch::OnSocketWithSetXiaJiPeiERet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_SetXjPeieRet *info = (CMD_SetXjPeieRet *)cbBuffer;
	memcpy(info, buffer, size);

	char s_t_Desc[126] = {0};
	MyNSString::wc2mbs(info->s_t_desc, sizeof(info->s_t_desc), s_t_Desc);

	Dictionary *data = Dictionary::create();
	data->setObject(Integer::create(info->n_t_result), "n_t_result");
	data->setObject(String::create(s_t_Desc), "s_t_Desc");
	NotificationCenter::getInstance()->postNotification("XiaJiPeiESetRet", data);

	return true;
}

//设置下级返点
bool MsgDispatch::OnSocketWithSetXiaJiFanDian(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GP_XGHYFandian_RET *info = (CMD_GP_XGHYFandian_RET *)cbBuffer;
	memcpy(info, buffer, size);

	char s_t_Desc[34] = {0};
	MyNSString::wc2mbs(info->s_Account, sizeof(info->s_Account), s_t_Desc);

	Dictionary *data = Dictionary::create();
	data->setObject(Integer::create(info->lResult), "n_t_result");
	data->setObject(String::create(s_t_Desc), "s_t_Account");
	data->setObject(Double::create(info->f_Fandian), "f_fandian");
	NotificationCenter::getInstance()->postNotification("XiaJiFanDianRet", data);

	return true;
}

bool MsgDispatch::OnSocketWithExit(void *buffer, WORD size)
{
	NotificationCenter::getInstance()->postNotification("ExitReturn");
	return true;
}

bool MsgDispatch::OnSocketWithCaiZhongList(void *buffer, WORD size)
{
	//tagGameType

	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	tagGameKind  *info = (tagGameKind  *)cbBuffer;
	memcpy(info, buffer, size);	

	int count = size / sizeof(tagGameKind);

	Array *dataArray = Array::create();
	for(int i = 0 ; i < count ; i++){	

		Dictionary *data = Dictionary::create();

		//1为彩票
		if ((info +i)->wTypeID != 1) continue;

		data->setObject(Integer::create((info + i)->wSortID),"wSortID");
		data->setObject(Integer::create((info +i)->wKindID), "wTypeID");

		//CCLOG("sortid-->%d   typeid-->%d", (info + i)->wSortID,(info +i)->wKindID);
		
		dataArray->addObject(data);
	}

	NotificationCenter::getInstance()->postNotification("GetCaiZhongList", dataArray);

	return true;
}

//添加下级返回      add by wangjun 2015-08-19
bool MsgDispatch::OnSocketWithAddHuiYuanRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_AddHuiYuanRet *info = (CMD_GR_AddHuiYuanRet *)cbBuffer;
	memcpy(info, buffer, size);

	char s_t_Desc[333] = {0};
	MyNSString::wc2mbs(info->s_t_Desc, sizeof(info->s_t_Desc), s_t_Desc);

	Dictionary *data = Dictionary::create();
	data->setObject(Integer::create(info->n_t_res), "lResult");
	data->setObject(String::create(s_t_Desc), "s_t_Desc");
	NotificationCenter::getInstance()->postNotification("AddHuiYuanRet", data);

	return true;
}

//配额返回   
bool MsgDispatch::OnSocketWithPeiERet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GetPeieRet *info = (CMD_GetPeieRet *)cbBuffer;
	memcpy(info, buffer, size);	

	Dictionary *data = Dictionary::create();

#ifdef VER_QIANBO
	data->setObject(Integer::create(info->n_t_peie_1), "total30");
	data->setObject(Integer::create(info->n_t_peie_2), "total29");
	data->setObject(Integer::create(info->n_t_peie_3), "total28");
	data->setObject(Integer::create(info->n_t_peie_4), "total27");
	data->setObject(Integer::create(info->n_t_peie_5), "total26");
	data->setObject(Integer::create(info->n_t_peie_s_1), "pere30");
	data->setObject(Integer::create(info->n_t_peie_s_2), "pere29");
	data->setObject(Integer::create(info->n_t_peie_s_3), "pere28");
	data->setObject(Integer::create(info->n_t_peie_s_4), "pere27");
	data->setObject(Integer::create(info->n_t_peie_s_5), "pere26");
#endif // _DEBUG

#ifdef VER_369
	data->setObject(Integer::create(info->n_t_peie_1), "total30");
	data->setObject(Integer::create(info->n_t_peie_2), "total29");
	data->setObject(Integer::create(info->n_t_peie_3), "total28");
	data->setObject(Integer::create(info->n_t_peie_4), "total27");
	data->setObject(Integer::create(info->n_t_peie_5), "total26");
	data->setObject(Integer::create(info->n_t_peie_s_1), "pere30");
	data->setObject(Integer::create(info->n_t_peie_s_2), "pere29");
	data->setObject(Integer::create(info->n_t_peie_s_3), "pere28");
	data->setObject(Integer::create(info->n_t_peie_s_4), "pere27");
	data->setObject(Integer::create(info->n_t_peie_s_5), "pere26");
#endif

#ifdef VER_9YI
	data->setObject(Integer::create(info->n_t_peie_1), "total30");
	data->setObject(Integer::create(info->n_t_peie_2), "total28");
	data->setObject(Integer::create(info->n_t_peie_3), "total27");
	data->setObject(Integer::create(info->n_t_peie_s_1), "pere30");
	data->setObject(Integer::create(info->n_t_peie_s_2), "pere28");
	data->setObject(Integer::create(info->n_t_peie_s_3), "pere27");
#endif // 
	
	NotificationCenter::getInstance()->postNotification("PeiERet", data);

	return true;
}

bool MsgDispatch::OnSocketWithForceOut(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GP_KillSocket *info = (CMD_GP_KillSocket *)cbBuffer;
	memcpy(info, buffer, size);	

	char szMessage[256] = {0};

	MyNSString::wc2mbs(info->szMessage, CountArray(info->szMessage), szMessage);

	Node* layer = Node::create();
	Vec2 pos = ccp(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	Sprite* spriteBack0 = Sprite::createWithSpriteFrame(spriteFrame("moving_label_001.png"));
	Sprite* spriteBack1 = Sprite::createWithSpriteFrame(spriteFrame("moving_label_001.png"));
	spriteBack0->setPosition(ccp(pos.x - spriteBack0->getContentSize().width * 0.5,pos.y));
	spriteBack1->setPosition(ccp(pos.x + spriteBack1->getContentSize().width * 0.5,pos.y));
	spriteBack1->setFlipX(true);
	layer->addChild(spriteBack0);
	layer->addChild(spriteBack1);
	LabelTTF* label = LabelTTF::create(szMessage,"",22);
	label->setColor(ccc3(255,255,0));
	label->setPosition(pos);
	layer->addChild(label);

	MoveTo* moveto0 = MoveTo::create(0.2f,ccp(spriteBack0->getPositionX(),spriteBack0->getPositionY() + 10));
	MoveTo* moveto1 = MoveTo::create(0.2f,ccp(spriteBack1->getPositionX(),spriteBack1->getPositionY() + 10));
	MoveTo* moveto2 = MoveTo::create(0.2f,ccp(label->getPositionX(),label->getPositionY() + 10));
	DelayTime* delay0 = DelayTime::create(2.0f);
	DelayTime* delay1 = DelayTime::create(2.0f);
	DelayTime* delay2 = DelayTime::create(2.0f);
	CallFuncN* call0 = CallFuncN::create(this,callfuncN_selector(MsgDispatch::onExitProgram));
	spriteBack0->runAction(Sequence::create(moveto0,delay0,call0,NULL));
	spriteBack1->runAction(Sequence::create(moveto1,delay1,NULL));
	label->runAction(Sequence::create(moveto2,delay2,NULL));

	Director::getInstance()->getRunningScene()->addChild(layer,255);

	return true;
}

//网页返点
bool MsgDispatch::OnSocketWithWebFanDianRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_SetWebFandianRet *info = (CMD_GR_SetWebFandianRet *)cbBuffer;
	memcpy(info, buffer, size);	

	Dictionary *data = Dictionary::create();	
	data->setObject(Integer::create(info->n_t_res), "res");//0成功 2超过限额  其它失败
	data->setObject(Integer::create(info->f_t_fandian), "fandian");

	NotificationCenter::getInstance()->postNotification("SetWebFanDianRet", data);

	return true;
}

//查询网页返点返回
bool MsgDispatch::OnSocketWithQueryWebFanDianRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GetRegUrl *info = (CMD_GetRegUrl *)cbBuffer;
	memcpy(info, buffer, size);	

	char szRegUrl[126] = {0};

	MyNSString::wc2mbs(info->s_reg_url, CountArray(info->s_reg_url), szRegUrl);

	Dictionary *data = Dictionary::create();
	
	data->setObject(String::create(szRegUrl), "regurl");
	data->setObject(Float::create(info->f_t_fandian), "fandian");

	NotificationCenter::getInstance()->postNotification("GetWebFanDianRet", data);

	return true;
}

//查询会员数据返回      add by wangjun 2015-08-19
bool MsgDispatch::OnSocketWithGetHyShjRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetHyShjRet *info = (CMD_GR_GetHyShjRet *)cbBuffer;
	memcpy(info, buffer, size);

	Dictionary *data = Dictionary::create();
	data->setObject(Integer::create(info->n_t_xjrs), "n_t_xjrs");
	data->setObject(Integer::create(info->n_t_xjzx), "n_t_xjzx");
	data->setObject(Integer::create(info->n_t_qianfenlv), "n_t_qianfenlv");
	data->setObject(Double::create(info->f_t_hyye), "f_t_hyye");
	data->setObject(Double::create(info->f_t_jrfd), "f_t_jrfd");
	data->setObject(Double::create(info->f_t_jrchz), "f_t_jrchz");
	data->setObject(Double::create(info->f_t_jrqk), "f_t_jrqk");//今日取款
	data->setObject(Double::create(info->f_t_jrtzh), "f_t_jrtzh");//今日投注
	//data->setObject(Double::create(info->f_t_jrhd), "f_t_jrhd");
	data->setObject(Integer::create(info->n_t_jinrizhuce), "n_t_jinrizhuce");
	data->setObject(Double::create(info->f_chedan_zonge), "f_chedan_zonge");
	data->setObject(Double::create(info->f_zhongjiang_zonge), "f_zhongjiang_zonge");
	data->setObject(Double::create(info->f_wodefandian), "f_wodefandian");
	data->setObject(Double::create(info->f_caipiao_yingkui), "f_caipiao_yingkui");
	data->setObject(Double::create(info->f_t_xianma), "f_t_xianma");
	data->setObject(Double::create(info->f_qp_fandian), "f_qp_fandian");
	data->setObject(Double::create(info->f_qp_yingkui), "f_qp_yingkui");
	data->setObject(Double::create(info->f_my_qp_fandian), "f_my_qp_fandian");
	data->setObject(Double::create(info->f_touzhuyongjin), "f_touzhuyongjin");
	data->setObject(Double::create(info->f_kuisunyongjin), "f_kuisunyongjin");
	data->setObject(Integer::create(info->n_t_newchongzhi),"n_t_newchongzhi");//今日注册并充值的人数
	data->setObject(Double::create(info->f_t_userfandian),"f_t_userfandian");//团队返点

	NotificationCenter::getInstance()->postNotification("GetHyShjRet", data);

	return true;
}

//±£œ’πÒ√¸¡Ó
bool MsgDispatch::OnTCPNetworkBankInsure(WORD wSubCmdID, void* pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_GP_USER_INSURE_INFO:					// 保险柜资料     403
		return this->OnSocketWithInsureInfo(pBuffer, wDataSize);
	case SUB_GP_USER_INSURE_SUCCESS:				// 操作成功     405
		return this->OnSocketWithBankSucceed(pBuffer, wDataSize);
	case SUB_GP_USER_INSURE_FAILURE:				// 操作失败     406
		return this->OnSocketWithBankFailure(pBuffer, wDataSize);
	default:
		break;
	}
	return true;
}

//保险柜信息      add by wangjun 2014-12-10
bool MsgDispatch::OnSocketWithInsureInfo(void *buffer, WORD size)
{
	BYTE cbBuffer[sizeof(CMD_GP_UserInsureInfo)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_UserInsureInfo));
	CMD_GP_UserInsureInfo *userInsureInfo = (CMD_GP_UserInsureInfo *)cbBuffer;
	memcpy(userInsureInfo, buffer, sizeof(CMD_GP_UserInsureInfo));
	m_wRevenueTake = userInsureInfo->wRevenueTake;				//税收
	m_wRevenueTransfer = userInsureInfo->wRevenueTransfer;			//税收
	m_wServerID = userInsureInfo->wServerID;					//房间标识
	m_dwlUserScore = userInsureInfo->lUserScore;				//用户豆豆
	m_dwGoldScore = userInsureInfo->lUserScore;				//用户豆豆
	m_dwlUserInsure = userInsureInfo->lUserInsure;				//保险柜豆豆
	m_dwlTransferPrerequisite = userInsureInfo->lTransferPrerequisite;		//转账条件
	g_GlobalUnits.getGolbalUserData()->lScore = m_dwlUserScore;
	g_GlobalUnits.getGolbalUserData()->lGoldEggs = 0;
	NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_UPDATESCOREANDGOLDEGG,NULL);  //刷新钱和花费点
	NotificationCenter::getInstance()->postNotification("UserInsureInfo");
	return true;
}

//查询保险柜成功      add by wangjun 2014-12-10
bool MsgDispatch::OnSocketWithBankSucceed(void *buffer, WORD size)
{
	BYTE cbBuffer[sizeof(CMD_GP_UserInsureSuccess)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_UserInsureSuccess));
	CMD_GP_UserInsureSuccess *userInsureInfo = (CMD_GP_UserInsureSuccess *)cbBuffer;
	memcpy(userInsureInfo, buffer, sizeof(CMD_GP_UserInsureSuccess));
	m_dwlUserScore = userInsureInfo->lUserScore;				//用户豆豆
	m_dwlUserInsure = userInsureInfo->lUserInsure;				//保险柜豆豆
	g_GlobalUnits.getGolbalUserData()->lScore = m_dwlUserScore;
	g_GlobalUnits.getGolbalUserData()->lGoldEggs = 0;
	NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_UPDATESCOREANDGOLDEGG,NULL);  //À¢–¬«Æ∫Õª®∑—µ„
	MyNSString::wc2mbs(userInsureInfo->szDescribeString, sizeof(userInsureInfo->szDescribeString), szDescribeString);
	NotificationCenter::getInstance()->postNotification("UserInsureInfo");
	return true;
}

//查询保险柜失败      add by wangjun 2014-12-10
bool MsgDispatch::OnSocketWithBankFailure(void *buffer, WORD size)
{
	BYTE cbBuffer[sizeof(CMD_GP_UserInsureFailure)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_UserInsureFailure));
	CMD_GP_UserInsureFailure *userInsureInfo = (CMD_GP_UserInsureFailure *)cbBuffer;
	memcpy(userInsureInfo, buffer, sizeof(CMD_GP_UserInsureFailure));
	MyNSString::wc2mbs(userInsureInfo->szDescribeString, sizeof(userInsureInfo->szDescribeString), szDescribeString);
	NotificationCenter::getInstance()->postNotification("UserInsureInfo");
	return true;
}

//登录消息
bool MsgDispatch::OnSocketGameLogin(CMD_Command* pCommand, void * pBuffer, WORD wDataSize)
{
	dispEntityMsg(pCommand,pBuffer,wDataSize,GBEVENT_NETWORK_LOGIN);
	return true;
}

//匿名登陆
void MsgDispatch::setAnonymous()
{
	setUserLogin("sys_user01","aaaAAA111get_notice!",true);
	m_bAnonymous = true;
}

//用户登陆
void MsgDispatch::setUserLogin(string account,string password,bool bAnonymous)
{
	m_pAccounts = account;
	m_pPassword = password;
	string str = "setUserLogin";
	MyNSString::GBKToUTF8(str);
	//CCLOG("%s", str.c_str());

	if (!bAnonymous)
	{
		m_szAccount = account;
		m_szPassword = password;
	}
	BYTE cbBuffer[2048];
	memset(cbBuffer, 0 , sizeof(CMD_MB_LogonAccounts));

	m_bAnonymous = bAnonymous;
	// 登录标识
	m_wBehaviorFlags = (m_bStartType?(BEHAVIOR_LOGON_IMMEDIATELY|VIEW_MODE_PART):BEHAVIOR_LOGON_NORMAL);

	//±‰¡ø∂®“Â
	//CMD_MB_LogonAccounts *pLogonByAccounts = (CMD_MB_LogonAccounts *)cbBuffer;
	//memset(pLogonByAccounts,0,sizeof(CMD_MB_LogonAccounts));

	//pLogonByAccounts->dwPlazaVersion = VERSION_MOBILE;
	//pLogonByAccounts->wModuleID = m_dwKindID;

	//memset(pLogonByAccounts->szAccounts, 0, sizeof(TCHART)*NAME_LEN);
	//memset(pLogonByAccounts->szPassword, 0, sizeof(TCHART)*PASS_LEN);
	//memset(pLogonByAccounts->szMobilePhone, 0, sizeof(TCHART)*LEN_MOBILE_PHONE);
	//memset(pLogonByAccounts->szMachineID, 0, sizeof(TCHART)*LEN_MD5);

	//char szMD5[33];
	//memset(szMD5,0,sizeof(szMD5));
	//CMD5Encrypt::EncryptData((unsigned char*)password.c_str(), szMD5);
	//MyNSString::mbs2wc(account.c_str(), NAME_LEN, pLogonByAccounts->szAccounts);
	//MyNSString::mbs2wc(szMD5, PASS_LEN, pLogonByAccounts->szPassword);
	//string macAddr = this->getMacaddress();
	//MyNSString::mbs2wc(macAddr.c_str(), macAddr.length(), pLogonByAccounts->szMachineID);

	//memset(m_pPassWordMD5, 0, sizeof(TCHART)*LEN_MD5);
	//memset(m_pMachineMD5, 0, sizeof(TCHART)*LEN_MD5);
	//MyNSString::mbs2wc(szMD5, PASS_LEN, m_pPassWordMD5);
	//MyNSString::mbs2wc(macAddr.c_str(), macAddr.length(), m_pMachineMD5);
	//m_eDataType = CMD_Logon;
	//m_pClientLoginSock->sendData(MDM_MB_LOGON,SUB_MB_LOGON_ACCOUNTS,pLogonByAccounts,sizeof(CMD_MB_LogonAccounts));

	CMD_GP_LogonAccounts *pLogonByAccounts = (CMD_GP_LogonAccounts *)cbBuffer;
	memset(pLogonByAccounts,0,sizeof(CMD_GP_LogonAccounts));

	pLogonByAccounts->dwPlazaVersion = VERSION_MOBILE;

	memset(pLogonByAccounts->szAccounts, 0, sizeof(TCHART)*NAME_LEN);
	memset(pLogonByAccounts->szPassword, 0, sizeof(TCHART)*PASS_LEN);
	memset(pLogonByAccounts->szMachineID, 0, sizeof(TCHART)*LEN_MD5);

	char szMD5[33];
	memset(szMD5,0,sizeof(szMD5));
	CMD5Encrypt::EncryptData((unsigned char*)password.c_str(), szMD5);
	// add by hxh 20160804
#ifdef WIN32
	wstring wstr=MyNSString::s2ws(account);
	wcscpy((wchar_t *)pLogonByAccounts->szAccounts,wstr.c_str());
#else
	MyNSString::mbs2wc(account.c_str(), NAME_LEN, pLogonByAccounts->szAccounts);
#endif
	MyNSString::mbs2wc(szMD5, PASS_LEN, pLogonByAccounts->szPassword);

	
	string macAddr = this->getMacaddress();
	MyNSString::mbs2wc(macAddr.c_str(), macAddr.length(), pLogonByAccounts->szMachineID);

	memset(m_pPassWordMD5, 0, sizeof(TCHART)*LEN_MD5);
	memset(m_pMachineMD5, 0, sizeof(TCHART)*LEN_MD5);
	MyNSString::mbs2wc(szMD5, PASS_LEN, m_pPassWordMD5);
	MyNSString::mbs2wc(macAddr.c_str(), macAddr.length(), m_pMachineMD5);
	m_eDataType = CMD_Logon;
	m_pClientLoginSock->sendData(MDM_GP_LOGON,SUB_GP_LOGON_ACCOUNTS,pLogonByAccounts,sizeof(CMD_GP_LogonAccounts));

}

void MsgDispatch::getVersion()
{
	DWORD dwKindID = GAMEKINDDDZ;
	//m_pClientLoginSock->sendData(MDM_GP_LOGON, SUB_GP_LOGON_VERSION, &dwKindID, sizeof(dwKindID));
}

//用户注册
void MsgDispatch::setUserRegister(string account,string password,int nfaceId,int nGender, int visiter)
{
	m_szAccount = account;
	m_szPassword = password;
	m_bAnonymous = false;
	m_bRegLogin =  true;

	//账号注册
	BYTE cbBuffer[sizeof(CMD_MB_RegisterAccounts)];
	memset(cbBuffer, 0 , sizeof(CMD_MB_RegisterAccounts));
	CMD_MB_RegisterAccounts *accounts = (CMD_MB_RegisterAccounts *)cbBuffer;

	accounts->wModuleID		= m_dwKindID;
	accounts->dwPlazaVersion = VERSION_MOBILE;
	//这里可以修改默认的 上级代理ID
	accounts->wFaceID  =nfaceId;
	accounts->cbGender = nGender;
	accounts->dwSpreadID = 43648;
	char szMD5[33];
	memset(szMD5,0,sizeof(szMD5));
	CMD5Encrypt::EncryptData((unsigned char*)password.c_str(), szMD5);
	MyNSString::mbs2wc(account.c_str(), NAME_LEN, accounts->szAccounts);
	MyNSString::mbs2wc(szMD5, PASS_LEN, accounts->szLogonPass);
	MyNSString::mbs2wc(account.c_str(), NAME_LEN, accounts->szNickName);
	MyNSString::mbs2wc(szMD5, PASS_LEN, accounts->szInsurePass);
	string macAddr = this->getMacaddress();
	MyNSString::mbs2wc(macAddr.c_str(), macAddr.length(), accounts->szMachineID);
	MyNSString::mbs2wc("12345678901", 12, accounts->szMobilePhone);

	memset(m_pPassWordMD5, 0, sizeof(TCHART)*LEN_MD5);
	memset(m_pMachineMD5, 0, sizeof(TCHART)*LEN_MD5);
	MyNSString::mbs2wc(szMD5, PASS_LEN, m_pPassWordMD5);
	MyNSString::mbs2wc(macAddr.c_str(), macAddr.length(), m_pMachineMD5);

	m_eDataType = CMD_Logon;

	int size = sizeof(CMD_MB_RegisterAccounts);
	//CCLOG("%d", size);

	m_pClientLoginSock->sendData(MDM_MB_LOGON,SUB_MB_REGISITER_ACCOUNTS,accounts,sizeof(CMD_MB_RegisterAccounts));
}

std::string& MsgDispatch::getMacaddress()
{
	//CCLOG("entitymgr address = %s",EntityMgr::instance()->getMacAddress());
	return EntityMgr::instance()->getMacAddress();
}

//发送登陆包
bool MsgDispatch::sendLoginPacket()
{
	return true;
	tagGlobalUserData* pUserData = g_GlobalUnits.getGolbalUserData();

	//获取信息
	BYTE cbBuffer[SOCKET_PACKET];
	memset(cbBuffer,0,SOCKET_PACKET);

	//tagGlobalUserData & GlobalUserData=g_GlobalUnits.GetGolbalUserData();

	char szMD5[33];
	CMD5Encrypt::EncryptData((unsigned char*)m_szPassword.c_str(), szMD5);

	//登录数据包
	CMD_GR_LogonByUserID * pLogonByUserID=(CMD_GR_LogonByUserID *)cbBuffer;
	pLogonByUserID->dwUserID		 = pUserData->dwUserID;
	pLogonByUserID->dwPlazaVersion	 = 0;
	pLogonByUserID->dwProcessVersion = 0;
	std::string plogonByPassWord = UserDefault::getInstance()->getStringForKey("NEWZJD_PASSWORD");
	memcpy(pLogonByUserID->szPassWord,szMD5 ,PASS_LEN);

	//memcpy(pLogonByUserID->szPassWord,szMD5,PASS_LEN);

	//memcpy(pLogonByUserID->szPassWord,pUserData->szPassWord,PASS_LEN);

	//机器序列号
	//tagClientSerial ClientSerial;
	//g_GlobalUnits.GetClientSerial(ClientSerial);

	//发送数据包
	CSendPacketHelper Packet(cbBuffer+sizeof(CMD_GR_LogonByUserID),sizeof(cbBuffer)-sizeof(CMD_GR_LogonByUserID));

	m_pClientGameSock->sendData(MDM_GR_LOGON,SUB_GR_LOGON_USERID,cbBuffer,sizeof(CMD_GR_LogonByUserID)+Packet.GetDataSize());

	return true;
}

//修改昵称
void MsgDispatch::modifyNickName(const char* nickName)
{
	tagGlobalUserData* pUserData = g_GlobalUnits.getGolbalUserData();

	BYTE cbBuffer[2048];
	CMD_GP_MODIFYNICKNAME* modifyNickName=(CMD_GP_MODIFYNICKNAME *)cbBuffer;

	memset(modifyNickName,0,sizeof(CMD_GP_MODIFYNICKNAME));
	modifyNickName->dwUserID = pUserData->dwUserID;
	const char* pNickName = nickName;
	memcpy(modifyNickName->szNickName,pNickName,sizeof(modifyNickName->szNickName));

	//发送数据包
	CSendPacketHelper Packet(cbBuffer+sizeof(CMD_GP_MODIFYNICKNAME),sizeof(cbBuffer)-sizeof(CMD_GP_MODIFYNICKNAME));
	m_pClientLoginSock->sendData(MDM_GP_USER, SUB_GP_MODIFY_NICKNAME, cbBuffer,sizeof(CMD_GP_MODIFYNICKNAME)+Packet.GetDataSize());
}

//第三方支付
void MsgDispatch::sendOrderInfo(const char* PayName,int chargeNum)
{
	tagGlobalUserData* pUserData = g_GlobalUnits.getGolbalUserData();

	BYTE cbBuffer[2048];
	CMD_GP_CZORDER* pOrderInfo=(CMD_GP_CZORDER *)cbBuffer;
	pOrderInfo->dwUserID = pUserData->dwUserID;
	pOrderInfo->dwMoney = (unsigned int)chargeNum;
	memcpy(pOrderInfo->szPayName,PayName,sizeof(pOrderInfo->szPayName));

	//发送数据包
	CSendPacketHelper Packet(cbBuffer+sizeof(CMD_GP_CZORDER),sizeof(cbBuffer)-sizeof(CMD_GP_CZORDER));

	if (m_loginState == enLogin_Server)
	{
		m_pClientLoginSock->sendData(MDM_GP_USER, SUB_GF_CZ_ORDER, cbBuffer,sizeof(CMD_GP_CZORDER)+Packet.GetDataSize());
		return;
	}
	m_pClientGameSock->sendData(MDM_GF_USER, SUB_GF_CZ_ORDER, cbBuffer,sizeof(CMD_GP_CZORDER)+Packet.GetDataSize());
}

//修改头像
void MsgDispatch::modifyTouxiang(int nHand,int nSex)
{
	tagGlobalUserData* pUserData = g_GlobalUnits.getGolbalUserData();
	BYTE cbBuffer[2048];
	CMD_GP_ModifyHead* pModifyTouxiang=(CMD_GP_ModifyHead*)cbBuffer;
	memset(pModifyTouxiang,0,sizeof(CMD_GP_ModifyHead));
	pModifyTouxiang->dwUserID = pUserData->dwUserID;
	pModifyTouxiang->nHead = nHand;
	pModifyTouxiang->nSex = nSex;
	//发送数据包
	CSendPacketHelper Packet(cbBuffer+sizeof(CMD_GP_ModifyHead),sizeof(cbBuffer)-sizeof(CMD_GP_ModifyHead));
	m_pClientLoginSock->sendData(MDM_GP_USER, SUB_GP_MODIFY_HEAD, cbBuffer,sizeof(CMD_GP_ModifyHead)+Packet.GetDataSize());
}

//紫金岛卡
void MsgDispatch::isLnadCardRecarge(const char* cardNum,const char* cardPass)
{
	tagGlobalUserData* pUserData = g_GlobalUnits.getGolbalUserData();
	BYTE cbBuffer[2048];
	CMD_GP_ISLANDCARD* pLandCard = (CMD_GP_ISLANDCARD*)cbBuffer;
	memset(pLandCard,0,sizeof(CMD_GP_ISLANDCARD));
	pLandCard->dwGameKindID = GAMEKINDDDZ;
	pLandCard->dwUserID = pUserData->dwUserID;
	memcpy(pLandCard->szCardNo,cardNum,sizeof(pLandCard->szCardNo));
	memcpy(pLandCard->szCardPwd,cardPass,sizeof(pLandCard->szCardPwd));
	CSendPacketHelper Packet(cbBuffer+sizeof(CMD_GP_ISLANDCARD),sizeof(cbBuffer)-sizeof(CMD_GP_ISLANDCARD));
	m_pClientLoginSock->sendData(MDM_GP_USER, SUB_GF_ISLAND, cbBuffer,sizeof(CMD_GP_ISLANDCARD)+Packet.GetDataSize());
}

void MsgDispatch::phoneCardRechange(const char* cardType,const char* cardFree,const char* cardNum,const char* cardPass)
{
	tagGlobalUserData* pUserData = g_GlobalUnits.getGolbalUserData();
	BYTE cbBuffer[2048];
	CMD_GP_SZFCARD* pSZcard = (CMD_GP_SZFCARD*)cbBuffer;
	memset(pSZcard,0,sizeof(CMD_GP_SZFCARD));
	pSZcard->dwUserID = pUserData->dwUserID;
	pSZcard->dwCardType = atoi(cardType);
	pSZcard->dwCardFee = atoi(cardFree);
	memcpy(pSZcard->szCardNo,cardNum,sizeof(pSZcard->szCardNo));
	memcpy(pSZcard->szCardPwd,cardPass,sizeof(pSZcard->szCardPwd));
	CSendPacketHelper Packet(cbBuffer+sizeof(CMD_GP_SZFCARD),sizeof(cbBuffer)-sizeof(CMD_GP_SZFCARD));
	m_pClientLoginSock->sendData(MDM_GP_USER, SUB_GF_SZFCARD, cbBuffer,sizeof(CMD_GP_SZFCARD)+Packet.GetDataSize());
}

//获得登陆奖励
void MsgDispatch::getLoginAward()
{
	tagGlobalUserData* pUserData = g_GlobalUnits.getGolbalUserData();
	DWORD cbBuffer[2];
	cbBuffer[0] = pUserData->dwUserID;
	cbBuffer[1] = GAMEKINDDDZ;
	m_pClientLoginSock->sendData(MDM_GP_USER, SUB_GP_GETLOGONPRIZE, cbBuffer,sizeof(cbBuffer));
}

void MsgDispatch::modifyPassWord(string oldPassword,string newPassword, bool isLoginPass)
{
	tagGlobalUserData* pUserData = g_GlobalUnits.getGolbalUserData();
	BYTE cbBuffer[2048];
	CMD_GP_XG_Loginpass* modifyPassWord=(CMD_GP_XG_Loginpass *)cbBuffer;
	memset(modifyPassWord,0,sizeof(CMD_GP_XG_Loginpass));
	modifyPassWord->dwUserID = pUserData->dwUserID;
	char szOldPasswordMD5[33];
	CMD5Encrypt::EncryptData((unsigned char*)oldPassword.c_str(), szOldPasswordMD5);
	char szNewPasswordMD5[33];
	CMD5Encrypt::EncryptData((unsigned char*)newPassword.c_str(), szNewPasswordMD5);

	MyNSString::mbs2wc(szOldPasswordMD5, sizeof(szOldPasswordMD5), modifyPassWord->szOldPass);
	MyNSString::mbs2wc(szNewPasswordMD5, sizeof(szNewPasswordMD5), modifyPassWord->szNewPass);
	//发送数据包
	userServiceSubCmd = SUB_GP_XG_LOGIN_PASS;
	if (isLoginPass) {
		m_pClientLoginSock->sendData(MDM_GP_USER_SERVICE, SUB_GP_XG_LOGIN_PASS, cbBuffer,sizeof(CMD_GP_XG_Loginpass));
	}else{
		m_pClientLoginSock->sendData(MDM_GP_USER_SERVICE, SUB_GP_XG_QUKUAN_PASS, cbBuffer,sizeof(CMD_GP_XG_Loginpass));
	}
}

//领取任务奖励
void MsgDispatch::getDarlyAward(int taskID)
{
	tagGlobalUserData* pUserData = g_GlobalUnits.getGolbalUserData();
	BYTE cbBuffer[2048];
	CMD_GP_GetDailyPrize* getDailyPrize = (CMD_GP_GetDailyPrize*)cbBuffer;
	memset(getDailyPrize,0,sizeof(CMD_GP_GetDailyPrize));
	getDailyPrize->dwUserID = pUserData->dwUserID;
	getDailyPrize->dwGameKindID = GAMEKINDDDZ;
	getDailyPrize->dwPrizeID = (unsigned int)taskID;
	getDailyPrize->nSubCmd = 1;
	CSendPacketHelper Packet(cbBuffer+sizeof(CMD_GP_GetDailyPrize),sizeof(cbBuffer)-sizeof(CMD_GP_GetDailyPrize));
	m_pClientLoginSock->sendData(MDM_GP_USER, SUB_GP_TASKDAILYDATA, cbBuffer,sizeof(CMD_GP_GetDailyPrize)+Packet.GetDataSize());
}

void MsgDispatch::getDownLoadAward(int gameID,int subcmd)
{
	tagGlobalUserData* pUserData = g_GlobalUnits.getGolbalUserData();
	if(pUserData==NULL){
		return;
	}
	BYTE cbBuffer[2048];
	SDownloadGamePrize* sDownLoadDate = (SDownloadGamePrize*)cbBuffer;
	memset(sDownLoadDate,0,sizeof(SDownloadGamePrize));
	sDownLoadDate->dwGameKindID = GAMEKINDDDZ;
	sDownLoadDate->dwUserID = pUserData->dwUserID;
	sDownLoadDate->nIndex = gameID;
	sDownLoadDate->nSubCmd = subcmd;
	CSendPacketHelper Packet(cbBuffer+sizeof(SDownloadGamePrize),sizeof(cbBuffer)-sizeof(SDownloadGamePrize));
	m_pClientLoginSock->sendData(MDM_GP_USER, SUB_GP_DOWNLOADGAMEGOLD, cbBuffer,sizeof(SDownloadGamePrize)+Packet.GetDataSize());
}

//完善注册
void MsgDispatch::overRegist(string newAccount,string newPassword)
{
	tagGlobalUserData* pUserData = g_GlobalUnits.getGolbalUserData();
	BYTE cbBuffer[2048];
	CMD_GP_Visitor2Player* sVisitor2Player = (CMD_GP_Visitor2Player*)cbBuffer;
	memset(sVisitor2Player,0,sizeof(CMD_GP_Visitor2Player));
	sVisitor2Player->dwUserID = pUserData->dwUserID;

	char szMD5[33];
	std::string plogonByPassWord = UserDefault::getInstance()->getStringForKey("NEWZJD_PASSWORD");
	CMD5Encrypt::EncryptData((unsigned char*)plogonByPassWord.c_str(), szMD5);
	memcpy(sVisitor2Player->szPassWord,szMD5 ,PASS_LEN);

	char szMD5New[33];
	CMD5Encrypt::EncryptData((unsigned char*)newPassword.c_str(), szMD5New);
	memcpy(sVisitor2Player->szPassWordNew,szMD5New ,PASS_LEN);

	memcpy(sVisitor2Player->szAccounts, newAccount.c_str(), sizeof(sVisitor2Player->szAccounts));
	CSendPacketHelper Packet(cbBuffer+sizeof(CMD_GP_Visitor2Player),sizeof(cbBuffer)-sizeof(CMD_GP_Visitor2Player));
	m_pClientLoginSock->sendData(MDM_GP_USER, SUB_GP_VISITOR_TO_PALYER, cbBuffer,sizeof(CMD_GP_Visitor2Player)+Packet.GetDataSize());
}

void MsgDispatch::getRelieve(int nUserID)
{
	m_pClientLoginSock->sendData(MDM_GP_USER, SUB_GP_GET_RELIEVE, &nUserID,sizeof(nUserID));
}

//发送赠送包
void MsgDispatch::sendScoreGiftPacket(const char* szAccount,long lValue)
{
	CM_GF_SCOREGIFT ScoreGiftUser;
	memset(&ScoreGiftUser,0,sizeof(ScoreGiftUser));
	ScoreGiftUser.dwUserID = g_GlobalUnits.getGolbalUserData()->dwUserID;
	ScoreGiftUser.lValue   = lValue;
	memcpy(ScoreGiftUser.szToUserName, szAccount, sizeof(ScoreGiftUser.szToUserName));

	char szMD5[33];
	CMD5Encrypt::EncryptData((unsigned char*)m_szPassword.c_str(), szMD5);
	memcpy(ScoreGiftUser.szPassword, szMD5,sizeof(ScoreGiftUser.szPassword));

	BYTE szSendBuffer[2048];
	CDataArchive SendBuffer(szSendBuffer,2048);
	SendBuffer.Write(&ScoreGiftUser,sizeof(CM_GF_SCOREGIFT));

	m_pClientLoginSock->sendData(MDM_GP_USER, SUB_GF_GIFT, (void*)szSendBuffer,SendBuffer.GetBufferOffset());
}

//发送金币话费领取包
void MsgDispatch::sendChargePacket(int ntype,const char* des)
{
	//发送信息
	CMD_GF_Bank ChargeUser;
	memset(&ChargeUser,0,sizeof(ChargeUser));

	ChargeUser.dwUserID = g_GlobalUnits.getGolbalUserData()->dwUserID;
	ChargeUser.dwGameKindID = GAMEKINDDDZ;
	//通过lValue来区分对话话费还是金币 0话费,1金币
	ChargeUser.lValue = ntype;

	memcpy(ChargeUser.szPassword, des,sizeof(ChargeUser.szPassword));

	CCLOG("%s",m_szPassword.c_str());

	char szMD5[33];
	CMD5Encrypt::EncryptData((unsigned char*)m_szPassword.c_str(), szMD5);
	memcpy(ChargeUser.szRealPsd, szMD5,sizeof(ChargeUser.szRealPsd));


	BYTE szSendBuffer[2048];
	CDataArchive SendBuffer(szSendBuffer,2048);
	SendBuffer.Write(&ChargeUser,sizeof(CMD_GF_Bank));

	m_pClientLoginSock->sendData(MDM_GP_USER, SUB_GF_BANK_GET, (void*)szSendBuffer,SendBuffer.GetBufferOffset());
}

//∑¢ÀÕ≈≈∂”«Î«Û
void MsgDispatch::sendQueuePacket(int cbQueueCell)
{
	CMD_GR_JoinMatch JoinMatch;
	memset(&JoinMatch,0,sizeof(JoinMatch));

	// 0 第一次排队
	// 1 不允许服务器帮你排队
	// 2 快速排队

	JoinMatch.wJoin = 0;
	JoinMatch.lCellScore = cbQueueCell;

	BYTE szSendBuffer[2048];
	CDataArchive SendBuffer(szSendBuffer,2048);
	SendBuffer.Write(&JoinMatch,sizeof(CMD_GR_JoinMatch));

	CCLOG("Queued requests! file(%s) line(%d)",__FILE__, __LINE__);

	m_pClientGameSock->sendData(MDM_GR_USER, SUB_GR_USER_START_QUEUE_REQ, (void*)szSendBuffer,SendBuffer.GetBufferOffset());
}

//获取服务器配置信息
void MsgDispatch::sendGetConfigPacket(int nVersion)
{
	CMD_GP_GetConfig config;
	config.dwGameKindID = GAMEKINDDDZ;
	config.dwVersion = nVersion;

	m_pClientLoginSock->sendData(MDM_GP_LOGON, SUB_GP_GET_PZINFO, (void*)&config, sizeof(config));
}
//找回密码
void MsgDispatch::findPassword(const char* szAccount)
{
	//char buf[50];
	//memset(buf, 0, 50);
	//utf::utf_gb_convert("utf-8", "gb2312", buf,50, const_cast<char*>(szAccount), strlen(szAccount));

	CMD_GP_GetPassword GetPassword;
	memcpy(GetPassword.szAccounts, szAccount, sizeof(GetPassword.szAccounts));

	std::string str = getMacaddress();
	memcpy(GetPassword.szMobileCode, str.c_str(), str.length());

	BYTE szSendBuffer[2048];
	CDataArchive SendBuffer(szSendBuffer,2048);
	SendBuffer.Write(&GetPassword,sizeof(GetPassword));

	m_pClientLoginSock->sendData(MDM_GP_LOGON, SUB_GP_GET_PASSWORD, (void*)szSendBuffer,SendBuffer.GetBufferOffset());

}

//发送数据
void MsgDispatch::sendSocketDate(WORD wMainCmdID,WORD wSubCmdID)
{
	m_pClientGameSock->sendData(wMainCmdID,wSubCmdID);
}

//发送数据
void MsgDispatch::sendSocketDate(WORD wMainCmdID,WORD wSubCmdID, void* pData, WORD wDataSize)
{
	m_pClientGameSock->sendData(wMainCmdID,wSubCmdID,pData,wDataSize);
}

//网络命令消息发送
// 网络验证@tag:0=登录服务器；1=游戏服务器
void MsgDispatch::SendPacketWithCompilatio(int tag)
{
	BYTE cbBuffer[sizeof(TCP_Validate)];
	memset(cbBuffer, 0 , sizeof(TCP_Validate));
	TCP_Validate *validate = (TCP_Validate *)cbBuffer;
	Common_Compilation(validate->szValidateKey);
	if(tag == 0){
		m_pClientLoginSock->sendData(MDM_KN_COMMAND, SUB_KN_VALIDATE_SOCKET, (void*)validate, sizeof(TCP_Validate));
	}else{
		m_pClientGameSock->sendData(MDM_KN_COMMAND, SUB_KN_VALIDATE_SOCKET, (void*)validate, sizeof(TCP_Validate));
	}
	CCLOG("		发送验证");
}

bool MsgDispatch::connectLoginServer()
{
	CCLOG("connectLoginServer");
	if (m_pClientLoginSock->isServerConected())
	{
		//m_pClientLoginSock->closeSocket();
		return true;
	}
	CCLOG("connectLoginServer");


	// add by hxh 20160712
	bool ret = false;
	//没有网络则不连接服务器
	if(netWorkType == 0) return false;

#if 0
	ret = m_pClientLoginSock->connectServer(IP_LOGON, PORT_LOGON);
	if(!ret)
		ret = m_pClientLoginSock->connectServer(IP_LOGON2, PORT_LOGON);
	if (!ret)
		ret = m_pClientLoginSock->connectServer(IP_LOGON3, PORT_LOGON);
	if(!ret)
		ret = m_pClientLoginSock->connectServer(IP_LOGON4, PORT_LOGON);
#else
	const char *ip1= ConfigMgr::instance()->text("display_config.xml","t5010");
	const char *ip2= ConfigMgr::instance()->text("display_config.xml","t5011");
	const char *ip3= ConfigMgr::instance()->text("display_config.xml","t5012");
	const char *ip4= ConfigMgr::instance()->text("display_config.xml","t5013");
	const int port=atoi(ConfigMgr::instance()->text("display_config.xml","t5002"));

	ret = m_pClientLoginSock->connectServer(ip1, port);
	if(!ret)
		ret = m_pClientLoginSock->connectServer(ip2, port);
	if (!ret)
		ret = m_pClientLoginSock->connectServer(ip3, port);
	if(!ret)
		ret = m_pClientLoginSock->connectServer(ip4, port);
#endif
	if (!ret)
	{
		CCLOG("connect login server faild! file(%s) line(%d)",__FILE__, __LINE__);
		return  false;
	}
	SendPacketWithCompilatio(0);
	return true;
}

//连接游戏服务器
bool MsgDispatch::connectGameServer(int nGameKindId,int nServerId/*=0*/)
{
	if (m_pClientGameSock->isServerConected())
	{
				m_pClientGameSock->closeSocket();
	}
	//tagGameServer server = m_ServerListMgr.getGameServerByIndex(roomIndex);
	// 修改原因：手机棋牌要支持选房间<by hxh 20161213>
#if 0
	tagGameServer server= m_ServerListMgr.getGameServer(nGameKindId);
#else
	tagGameServer server;
	int idx=-1;
	if(nServerId>0)
	{
		server = m_ServerListMgr.getGameServerByServerId(nServerId,&idx);
	}
	if(idx==-1)
	{
	    server = m_ServerListMgr.getGameServer(nGameKindId);
	}
#endif
	char ip[64];
	MyNSString::wc2mbs(server.szServerAddr, sizeof(server.szServerAddr), ip);
	unsigned short port = server.wServerPort;
	CCLOG("ip=%s,wServerPort=%d",ip,port);
	bool ret = m_pClientGameSock->connectServer(ip, port);
	if (!ret)
	{
		CCLOG("connect game server faild! file(%s) line(%d)",__FILE__, __LINE__);
		return  false;
	}
	//m_dwKindID = nGameKindId;
	m_dwKindID = server.wKindID;//<by hxh>
	SendPacketWithCompilatio(1);
	return true;
}

//快速登陆房间      add by wangjun 2014-12-10
bool MsgDispatch::connectGameServer()
{
	if (m_pClientGameSock->isServerConected())
	{
		m_pClientGameSock->closeSocket();
	}
	tagGameServer server;
	for(int roomIndex = 0 ; roomIndex < g_GlobalUnits.m_nOnlineCount ; roomIndex++)
	{
		server = m_ServerListMgr.getGameServerByIndex(roomIndex);
		if(m_dwGoldScore > server.lMinServerScore)
		{
			break;
		}
	}
	char ip[64];
	int isie = sizeof(server.szServerAddr);
	MyNSString::wc2mbs(server.szServerAddr, sizeof(server.szServerAddr)/sizeof(TCHART), ip);
	unsigned short port = server.wServerPort;
	bool ret = m_pClientGameSock->connectServer(ip, port);
	CCLOG("============================");
	CCLOG("gameserver ip = %s", ip);
	CCLOG("gameserver port = %d", port);
	CCLOG("============================");
	if (!ret)
	{
		CCLOG("connect game server faild! file(%s) line(%d)",__FILE__, __LINE__);
		return  false;
	}
	SendPacketWithCompilatio(1);
	return true;
}

//登陆房间      add by wangjun 2014-12-10
void MsgDispatch::SendPacketWithEnterRoomPageCount(WORD count)
{
	// 房间登录
	BYTE cbBuffer[sizeof(CMD_GR_LogonMobile)];
	memset(cbBuffer, 0 , sizeof(CMD_GR_LogonMobile));
	CMD_GR_LogonMobile *lm = (CMD_GR_LogonMobile *)cbBuffer;

	lm->wGameID			= m_dwKindID;					// 游戏标识
	lm->dwProcessVersion = VERSION_FRAME;                 // 手机版本
	//lm->wBehaviorFlags	= m_wBehaviorFlags;				// 行为标识
	//lm->wPageTableCount	= count;						// 页数
	lm->wVisibleMode	= m_wBehaviorFlags;				// 行为标识
	lm->wVisibleTable	= count;						// 页数
	lm->dwUserID			= m_dwUserID;					// 用户id
	//lm->cbDeviceType		= m_cbDeviceType;				// 设备类型

	//MyNSString::mbs2wc(m_szPassword.c_str(), (int)LEN_PASSWORD, lm.szPassword);
	memcpy(lm->szPassword, m_pPassWordMD5, sizeof(TCHART)*LEN_MD5);
	CCLOG("sizeof(CMD_GR_LogonMobile)=%d,m_dwUserID=%d", sizeof(CMD_GR_LogonMobile),m_dwUserID);
	CCLOG("wBehaviorFlags=wVisibleMode=%d,wPageTableCount=wVisibleTable=%d",lm->wVisibleMode,lm->wVisibleTable);
	m_pClientGameSock->sendData(MDM_GR_LOGON, SUB_GR_LOGON_MOBILE, (void*)lm, sizeof(CMD_GR_LogonMobile));
	// 消息状态
	m_eDataType = CMD_Room;
	setLoginState(enGame_Server);
	//NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_GAMELINK,Integer::create(1));
}

//连接游戏服务器
bool MsgDispatch::connectGameServerAdr(DWORD dwAdrIP, WORD wPort)
{
	in_addr addr;
	addr.s_addr = dwAdrIP;
	char* ip = inet_ntoa(addr);

	bool ret = m_pClientGameSock->connectServer(ip, wPort);

	setLoginState(enGame_Server);
	m_eDataType = CMD_Room;
	if (!ret)
	{
		CCLOG("connect game server failed! file(%s) line(%d)",__FILE__, __LINE__);
	}
	CCLOG("------------------------------connect game server success!---------------------------------------");

	return ret;
}
//发送数据
void MsgDispatch::sendSocketDate(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	m_pClientGameSock->sendData(MDM_GF_GAME,wSubCmdID,pData,wDataSize);
}

//发送表情、文字
void MsgDispatch::sendFrameDate(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	m_pClientGameSock->sendData(MDM_GF_FRAME,wSubCmdID,pData,wDataSize);
}

//发送数据
void MsgDispatch::sendSocketDate(WORD wSubCmdID)
{
	m_pClientGameSock->sendData(MDM_GF_GAME,wSubCmdID);
}


//获取全服总人数
DWORD MsgDispatch::getServerOnLineCount()
{
	return m_ServerListMgr.getServerOnLineCount();
}

GameServerList MsgDispatch::getGameServerList(unsigned int nKind)
{
	return m_ServerListMgr.getGameServerList(nKind);
}

void MsgDispatch::closeLoginSocket()
{
	m_pClientLoginSock->closeSocket();

	m_dwRecvPacketCount = 0;
	m_wRecvSize = 0;
	memset(m_cbRecvBuff, 0, 81920);
}

//关闭游戏服务连接
void MsgDispatch::closeGameSocket()
{
	m_pClientGameSock->closeSocket();

	m_dwRecvPacketCount = 0;
	m_wRecvSize = 0;
	memset(m_cbRecvBuff, 0, 81920);

	setLoginState(enLogin_Server);
}

int MsgDispatch::getLevelbyScore(LONG lScore)
{
	int nLevle = 0;
	for (int i = 0; i < CountArray(m_lUserLevel); i++)
	{
		if (lScore >= m_lUserLevel[i])
		{
			nLevle = i + 1;
		}
	}
	return  nLevle;
}

int MsgDispatch::getBaseCell(int type,int nLevel)   //获取倍率
{
	UserLevel::iterator iter = m_UserLevelList.begin();
	for (; iter != m_UserLevelList.end(); iter++)
	{
		if ((*iter).lInfoType == type)
		{
			return  (*iter).tagUserLeve[nLevel].lUserLevel;
		}
	}
	return 0;
}

//∑¢ÀÕ”Œœ∑πÊ‘Ú      add by wangjun 2014-12-10
void MsgDispatch::SendPacketWithGameOption()
{
	BYTE cbBuffer[sizeof(CMD_GF_GameOption)];
	memset(cbBuffer, 0 , sizeof(CMD_GF_GameOption));
	CMD_GF_GameOption *gameOption = (CMD_GF_GameOption *)cbBuffer;
	gameOption->cbAllowLookon=false;
	gameOption->dwFrameVersion=INVALID_DWORD;
	gameOption->dwClientVersion=INVALID_DWORD;

	m_pClientGameSock->sendData(MDM_GF_FRAME, SUB_GF_GAME_OPTION, gameOption, sizeof(CMD_GF_GameOption));
	CCLOG("		∑¢ÀÕ”Œœ∑πÊ‘Ú");
}

//ªÒ»°”√ªß ªÚ…Í«Î”√ªß–≈œ¢
void MsgDispatch::SendPacketWithRequestUserInfo(DWORD dwUserID, WORD wTableID)
{
	BYTE cbBuffer[sizeof(CMD_GR_UserInfoReq)];
	memset(cbBuffer, 0 , sizeof(CMD_GR_UserInfoReq));
	CMD_GR_UserInfoReq *info = (CMD_GR_UserInfoReq *)cbBuffer;

	info->dwUserIDReq=dwUserID;
	info->wTablePos=wTableID;

	m_pClientGameSock->sendData(MDM_GR_USER, SUB_GR_USER_INFO_REQ, info, sizeof(CMD_GR_UserInfoReq));

	CCLOG("Sending User Info Req");
}

void MsgDispatch::SendPacketWithRequestChairUserInfo(WORD wTableID, WORD wChairID)
{
	BYTE cbBuffer[sizeof(CMD_GR_ChairUserInfoReq)];
	memset(cbBuffer, 0 , sizeof(CMD_GR_ChairUserInfoReq));
	CMD_GR_ChairUserInfoReq *info = (CMD_GR_ChairUserInfoReq *)cbBuffer;

	info->wTableID=wTableID;
	info->wChairID=wChairID;

	m_pClientGameSock->sendData(MDM_GR_USER, SUB_GR_USER_CHAIR_INFO_REQ, info, sizeof(CMD_GR_ChairUserInfoReq));

	CCLOG("		∑¢ÀÕID«Î«Û");
}

//«Î«Û∑¢ÀÕ◊‘º∫◊¿◊”µƒ”√ªß–≈œ¢
void MsgDispatch::SendPacketWithRequestMyTableUserInfo(int playerNum)
{
	for(WORD chairID = 0 ; chairID < playerNum ; chairID++)
	{
		this->SendPacketWithRequestChairUserInfo(m_wTableID, chairID);
	}
}

//…Í«Îªª◊¿
void MsgDispatch::SendPacketWithChangeTable()
{
	m_pClientGameSock->sendData(MDM_GR_USER, SUB_GR_USER_CHAIR_REQ);
}

void MsgDispatch::SendPacketWithSitDown(WORD wTableID, WORD wChairID)
{
	BYTE cbBuffer[sizeof(CMD_GR_UserSitDown)];
	memset(cbBuffer, 0 , sizeof(CMD_GR_UserSitDown));
	CMD_GR_UserSitDown *info = (CMD_GR_UserSitDown *)cbBuffer;

	info->wChairID = m_wChairID;
	info->wTableID = m_wTableID;

	m_pClientGameSock->sendData(MDM_GR_USER, SUB_GR_USER_SITDOWN, info, sizeof(CMD_GR_UserSitDown));
}

void MsgDispatch::SendPacketWithUserStatus(BYTE status)
{
	BYTE cbBuffer[sizeof(CMD_GR_UserStatus)];
	memset(cbBuffer, 0 , sizeof(CMD_GR_UserStatus));
	CMD_GR_UserStatus *ready = (CMD_GR_UserStatus *)cbBuffer;

	ready->dwUserID = m_dwUserID;
	ready->UserStatus.wChairID = m_wChairID;
	ready->UserStatus.wTableID = m_wTableID;
	ready->UserStatus.cbUserStatus = status;

	m_pClientGameSock->sendData(MDM_GF_FRAME, SUB_GF_USER_READY, ready, sizeof(CMD_GR_UserStatus));
// 	EntityMgr::instance()->roomFrame()->setGameIsStart(true);
}

//¥ÛÃ¸±£œ’πÒ≤Ÿ◊˜
void MsgDispatch::SendPacketWithQueryBankInfo()
{
	BYTE cbBuffer[sizeof(CMD_GP_QueryInsureInfo)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_QueryInsureInfo));
	CMD_GP_QueryInsureInfo *info = (CMD_GP_QueryInsureInfo *)cbBuffer;

	info->dwUserID=m_dwUserID;
	memcpy(info->szPassword, m_pPassWordMD5, sizeof(CMD_GP_QueryInsureInfo)-sizeof(info->dwUserID));

	if (!m_pClientLoginSock->isServerConected())
	{
		this->connectLoginServer();
		//m_pClientLoginSock->connectServer(IP_LOGON, PORT_TEMP);
	}
	m_pClientLoginSock->sendData(MDM_GP_USER_SERVICE, SUB_GP_QUERY_INSURE_INFO, info, sizeof(CMD_GP_QueryInsureInfo));
}

void MsgDispatch::SendPacketWithDeposit(SCORE score)
{
	BYTE cbBuffer[sizeof(CMD_GP_UserSaveScore)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_UserSaveScore));
	CMD_GP_UserSaveScore *info = (CMD_GP_UserSaveScore *)cbBuffer;

	info->dwUserID=m_dwUserID;
	info->lSaveScore = score;
	memcpy(info->szMachineID, m_pMachineMD5, sizeof(CMD_GP_UserSaveScore)-sizeof(info->dwUserID)-sizeof(info->lSaveScore));

	if (!m_pClientLoginSock->isServerConected())
	{
		this->connectLoginServer();
		//m_pClientLoginSock->connectServer(IP_LOGON, PORT_TEMP);
	}
	m_pClientLoginSock->sendData(MDM_GP_USER_SERVICE, SUB_GP_USER_SAVE_SCORE, info, sizeof(CMD_GP_UserSaveScore));
}

void MsgDispatch::SendPacketWithWithdraw(SCORE score, string password)
{
	BYTE cbBuffer[sizeof(CMD_GP_UserTakeScore)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_UserTakeScore));
	CMD_GP_UserTakeScore *info = (CMD_GP_UserTakeScore *)cbBuffer;

	info->dwUserID=m_dwUserID;
	info->lTakeScore = score;
	memcpy(info->szMachineID, m_pMachineMD5, sizeof(TCHART)*LEN_MACHINE_ID);

	char szMD5[33];
	memset(szMD5,0,sizeof(szMD5));
	CMD5Encrypt::EncryptData((unsigned char*)password.c_str(), szMD5);
	MyNSString::mbs2wc(szMD5, PASS_LEN, info->szPassword);

	if (!m_pClientLoginSock->isServerConected())
	{
		this->connectLoginServer();
		//m_pClientLoginSock->connectServer(IP_LOGON, PORT_TEMP);
	}
	m_pClientLoginSock->sendData(MDM_GP_USER_SERVICE, SUB_GP_USER_TAKE_SCORE, info, sizeof(CMD_GP_UserTakeScore));
}

void MsgDispatch::SendPacketWithSendScore(string nickname, SCORE score, string password, BYTE byNickName)
{

	BYTE cbBuffer[sizeof(CMD_GP_UserTransferScore)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_UserTransferScore));
	CMD_GP_UserTransferScore *info = (CMD_GP_UserTransferScore *)cbBuffer;

	info->dwUserID=m_dwUserID;
	info->cbByNickName = byNickName;
	info->lTransferScore = score;
	MyNSString::mbs2wc(nickname.c_str(), LEN_NICENAME, info->szNickName);

	char szMD5[33];
	memset(szMD5,0,sizeof(szMD5));
	CMD5Encrypt::EncryptData((unsigned char*)password.c_str(), szMD5);
	MyNSString::mbs2wc(szMD5, PASS_LEN, info->szPassword);

	memcpy(info->szMachineID, m_pMachineMD5, sizeof(TCHART)*LEN_MACHINE_ID);

	if (!m_pClientLoginSock->isServerConected())
	{
		this->connectLoginServer();
		//m_pClientLoginSock->connectServer(IP_LOGON, PORT_TEMP);
	}
	m_pClientLoginSock->sendData(MDM_GP_USER_SERVICE, SUB_GP_USER_TRANSFER_SCORE, info, sizeof(CMD_GP_UserTransferScore));
}

void MsgDispatch::SendPacketWithGiveGoods(WORD wPropertyIndex, DWORD dwTargetUserID)
{
	BYTE cbBuffer[sizeof(CMD_GR_C_PropertyBuy)];
	memset(cbBuffer, 0 , sizeof(CMD_GR_C_PropertyBuy));
	CMD_GR_C_PropertyBuy *info = (CMD_GR_C_PropertyBuy *)cbBuffer;

	info->cbRequestArea = 0;
	info->cbConsumeScore = 0;
	info->wItemCount = 1;
	info->wPropertyIndex = wPropertyIndex;
	info->dwTargetUserID = dwTargetUserID;

	if (!m_pClientGameSock->isServerConected())
	{
		//超时处理
	}

	m_pClientGameSock->sendData(MDM_GR_USER, SUB_GR_PROPERTY_BUY, info, sizeof(CMD_GR_C_PropertyBuy));
}

//购买礼物
void MsgDispatch::SendPacketWithBuyGoods(DWORD dwBuyGoodsID, DWORD dwBuyCount)
{
	BYTE cbBuffer[sizeof(CMD_GP_UserBuyGoodsInfo)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_UserBuyGoodsInfo));
	CMD_GP_UserBuyGoodsInfo *info = (CMD_GP_UserBuyGoodsInfo *)cbBuffer;

	info->dwUserID=m_dwUserID;
	info->dwBuyGoodsGNO = dwBuyGoodsID;
	info->dwBuyCount = dwBuyCount;

	if (!m_pClientLoginSock->isServerConected())
	{
		this->connectLoginServer();
	}

	m_pClientLoginSock->sendData(MDM_GP_USER_SERVICE, SUB_GP_BUY_GOODS_INFO, info, sizeof(CMD_GP_UserBuyGoodsInfo));
}

void MsgDispatch::SendPacketWithGetUserPacket()
{
	BYTE cbBuffer[sizeof(CMD_GP_GetMyPocket)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetMyPocket));
	CMD_GP_GetMyPocket *info = (CMD_GP_GetMyPocket *)cbBuffer;

	info->dwUserID=m_dwUserID;

	if (!m_pClientLoginSock->isServerConected())
	{
		this->connectLoginServer();
	}

	m_pClientLoginSock->sendData(MDM_GP_USER_SERVICE, SUB_GR_GET_USER_PACKET, info, sizeof(CMD_GP_GetMyPocket));
}

//猜硬币
void MsgDispatch::SendPacketWithGuessCoin(bool bCoinSide, SCORE	 lScore)
{
	BYTE cbBuffer[sizeof(CMD_GP_GuessCoin)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GuessCoin));
	CMD_GP_GuessCoin *info = (CMD_GP_GuessCoin *)cbBuffer;

	info->dwUserID=m_dwUserID;
	info->bCoinSide = bCoinSide;
	info->lScore = lScore;

	if (!m_pClientLoginSock->isServerConected())
	{
		this->connectLoginServer();
	}

	m_pClientLoginSock->sendData(MDM_GP_USER_SERVICE, SUB_GP_GUESS_COIN, info, sizeof(CMD_GP_GuessCoin));
}

//大转盘
void MsgDispatch::SendPacketWithTurnTable()
{
	BYTE cbBuffer[sizeof(CMD_GP_TurnTable)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_TurnTable));
	CMD_GP_TurnTable *info = (CMD_GP_TurnTable *)cbBuffer;

	info->dwUserID=m_dwUserID;

	if (!m_pClientLoginSock->isServerConected())
	{
		this->connectLoginServer();
	}

	m_pClientLoginSock->sendData(MDM_GP_USER_SERVICE, SUB_GP_TURN_TABLE, info, sizeof(CMD_GP_TurnTable));
}

/*********”√ªß«Î«Û**********/
//«Î«Û∆¡¢
void MsgDispatch::SendPacketWithPerformStandup(WORD wTableID, WORD wChairID, BYTE cbForceLeave)
{
	m_nSendGameOption = 0;
	if(wTableID == INVALID_TABLE)
	{
		wTableID = m_wTableID;
	}
	if(wChairID == INVALID_CHAIR)
	{
		wChairID = m_wChairID;
	}
	if(wTableID == INVALID_TABLE || wChairID == INVALID_CHAIR)
	{
		return;
	}

	//if(m_cbStatus != US_SIT)
	//{
	//	return;
	//}

	BYTE cbBuffer[sizeof(CMD_GR_UserStandUp)];
	memset(cbBuffer, 0 , sizeof(CMD_GR_UserStandUp));
	CMD_GR_UserStandUp *info = (CMD_GR_UserStandUp *)cbBuffer;

	info->wTableID=wTableID;
	info->wChairID = wChairID;
	info->cbForceLeave = cbForceLeave;

	m_pClientGameSock->sendData(MDM_GR_USER, SUB_GR_USER_STANDUP, info, sizeof(CMD_GR_UserStandUp));
}

void MsgDispatch::SendPacketWithInviteReq(DWORD dwUserID)
{

	BYTE cbBuffer[sizeof(CMD_GR_UserInviteReq)];
	memset(cbBuffer, 0 , sizeof(CMD_GR_UserInviteReq));
	CMD_GR_UserInviteReq *info = (CMD_GR_UserInviteReq *)cbBuffer;

	info->wTableID = m_wTableID;
	info->dwUserID = dwUserID;

	CCLOG("%lu", sizeof(CMD_GR_UserInviteReq));

	m_pClientGameSock->sendData(MDM_GR_USER, SUB_GR_USER_INVITE_REQ, info, sizeof(CMD_GR_UserInviteReq));
}

void MsgDispatch::SendPacketQueryGameResult(int kindID)
{
	BYTE cbBuffer[sizeof(CMD_GP_QueryGameResult)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_QueryGameResult));
	CMD_GP_QueryGameResult *info = (CMD_GP_QueryGameResult *)cbBuffer;

	info->wKindID = kindID;

	if (!m_pClientLoginSock->isServerConected())
	{
		this->connectLoginServer();
	}

	//
#if 1
	if(kindID == 0)
	{			
		m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_QUERY_GAME_RESULT, info, sizeof(CMD_GP_QueryGameResult));
	}else
	{
		m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_QUERY_GAME_MOBILE_RESULT, info, sizeof(CMD_GP_QueryGameResult));
 		
	}
#else

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_QUERY_GAME_RESULT, info, sizeof(CMD_GP_QueryGameResult));

#endif // 

}

//查询单个彩种的开奖记录
void MsgDispatch::SendPacketQueryMoreGameResult(int kindID)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetMoreRecord)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetMoreRecord));
	CMD_GP_GetMoreRecord *info = (CMD_GP_GetMoreRecord *)cbBuffer;

	info->n_t_type = kindID;

	if (!m_pClientLoginSock->isServerConected())
	{
		this->connectLoginServer();
	}
	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_MORE_RECORD, info, sizeof(CMD_GP_GetMoreRecord));
}

//投注
void MsgDispatch::SendPacketWithTouZhuCQSSC(string qiShu, int gameType, int gameKind, string betNumber, int zhuShu,
											int beiTou, int moshi, int sign, int zhuiHao, int nIndex)
{
	CCLOG("touzhu qihao:%s",qiShu.c_str());
	//注数
	if (betNumber.length() <= 0)//15360)
	{
		BYTE cbBuffer[sizeof(CMD_GP_TouZhuCQSSC)];
		memset(cbBuffer, 0 , sizeof(CMD_GP_TouZhuCQSSC));
		CMD_GP_TouZhuCQSSC *info = (CMD_GP_TouZhuCQSSC *)cbBuffer;	
		info->dwUserID = m_dwUserID;
		memcpy(info->strQishu, qiShu.c_str(),qiShu.length());
		info->nGameType = gameType;
		info->nGameKind = gameKind;	
		info->nZhushu = zhuShu;
		info->nBeitou = beiTou;
		info->nMoshi = moshi;
		info->nSign = m_wTouZhuSign++;
		info->bZhuihao = 0;
		info->nIndex = nIndex;
		//最大长度15360
		memcpy(info->strHaoma, betNumber.c_str(),betNumber.length());
		info->nHaoMaLen = betNumber.length();
		m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_TOUZHU_CQSSC, info, sizeof(CMD_GP_TouZhuCQSSC));
	
		//time_t t = time(0);
		//char timeStr[64] = {0};
		//strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S",localtime(&t) );
		//DBHandler *handler = DBHandler::sharedDBHandler();
		//char sql[24000]={0};
		//sprintf(sql, "insert into TouZhuXXInfo(time,dwUserID,gameType,gameKind,qiShu,betNumber,zhuShu,beiTou,moshi,sign,isEnd,zhuiHao,nIndex)values('%s',%d,%d,%d,'%s','%s',%d,%d,%d,%d,%d,%d,%d)",
		//	timeStr,m_dwUserID,gameType,gameKind,qiShu.c_str(),betNumber.c_str(),zhuShu,beiTou,moshi,sign,0,zhuiHao,nIndex);
		//handler->execute(sql);
	} else//单式投注
	{
		BYTE cbBuffer[sizeof(CMD_GP_TouZhuCQSSC_Dan)];
		memset(cbBuffer, 0 , sizeof(CMD_GP_TouZhuCQSSC_Dan));
		CMD_GP_TouZhuCQSSC_Dan *info = (CMD_GP_TouZhuCQSSC_Dan *)cbBuffer;	

		//最大长度
		int max_count = 15360;	
		//压缩后的数组
		BYTE cbBuffer2[50000];
		memset(cbBuffer2, 0, 50000);
		size_t tempSize = 0;	//压缩后的大小		
		
		HDLzma::CompressBuffer(betNumber.c_str(),betNumber.size(),cbBuffer2,50000, &tempSize);

		//计算次数		
		int iSendCount = tempSize;
		iSendCount = iSendCount / max_count + (iSendCount % max_count != 0 ? 1 : 0);

		for(int i = 0;i < iSendCount;i ++)
		{
			info->dwUserID = m_dwUserID;
			memcpy(info->strQishu, qiShu.c_str(),qiShu.length());
			info->nGameType = gameType;
			info->nGameKind = gameKind;	
			info->nZhushu = zhuShu;
			info->nBeitou = beiTou;
			info->nMoshi = moshi;
			info->nSign = m_wTouZhuSign;
			info->bZhuihao = 0;
			info->nIndex = nIndex;
			//最大长度15360	
			int jCount = max_count * (i + 1);
			//大小
			if (jCount > tempSize)
			{
				jCount = tempSize;
			}
			for(int j = i * max_count;j < jCount;j ++) 
			{
				info->strHaoma[j - i * max_count] = cbBuffer2[j];
			}

			if (i == iSendCount - 1)
			{
				info->nHaoMaLen = tempSize % max_count;
				info->nEnd = 0;
			} else
			{
				info->nHaoMaLen = max_count;
				info->nEnd = 1;
			}			

			m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_TOUZHU_CQSSC_DAN, info, sizeof(CMD_GP_TouZhuCQSSC_Dan));
		}
		
		//标记自增
		m_wTouZhuSign ++;
	}	
}

void MsgDispatch::SendPacketWithTouZhuCQSSC_Dan(string qiShu, int gameType, int gameKind, string betNumber, int zhuShu,
												int beiTou, int moshi, int sign, int isEnd, int zhuiHao, int nIndex)												
{
	BYTE cbBuffer[sizeof(CMD_GP_TouZhuCQSSC_Dan)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_TouZhuCQSSC_Dan));
	CMD_GP_TouZhuCQSSC_Dan *info = (CMD_GP_TouZhuCQSSC_Dan *)cbBuffer;	

	//最大长度
	int max_count = 15360;	
	//压缩后的数组
	BYTE cbBuffer2[50000];
	memset(cbBuffer2, 0, 50000);
	size_t tempSize = 0;	//压缩后的大小		

	HDLzma::CompressBuffer(betNumber.c_str(),betNumber.size(),cbBuffer2,50000, &tempSize);

	//计算次数		
	int iSendCount = tempSize;
	iSendCount = iSendCount / max_count + (iSendCount % max_count != 0 ? 1 : 0);

	for(int i = 0;i < iSendCount;i ++)
	{
		info->dwUserID = m_dwUserID;
		memcpy(info->strQishu, qiShu.c_str(),qiShu.length());
		info->nGameType = gameType;
		info->nGameKind = gameKind;	
		info->nZhushu = zhuShu;
		info->nBeitou = beiTou;
		info->nMoshi = moshi;
		info->nSign = m_wTouZhuSign;
		info->bZhuihao = 0;
		info->nIndex = nIndex;
		//最大长度15360	

		for(int j = i * max_count;j < max_count * (i + 1);j ++) 
		{
			info->strHaoma[j - i * max_count] = cbBuffer2[j];
		}

		if (i == iSendCount - 1)
		{
			info->nHaoMaLen = tempSize % max_count;
			info->nEnd = 0;
		} else
		{
			info->nHaoMaLen = max_count;
			info->nEnd = 1;
		}			

		m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_TOUZHU_CQSSC_DAN, info, sizeof(CMD_GP_TouZhuCQSSC_Dan));
	}

	//标记自增
	m_wTouZhuSign ++;

}

void MsgDispatch::SendPacketWithGetTouZhuLogCount(bool bTime, const char *szTimeStart, const char *szTimeEnd,const int caizhong, const int status)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetTouzhuLogCount)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetTouzhuLogCount));
	CMD_GP_GetTouzhuLogCount *info = (CMD_GP_GetTouzhuLogCount *)cbBuffer;
	info->dwUserID = m_dwUserID;
	info->bTime = bTime;
	info->nCaiZhong = caizhong;
	info->nStatus = status;
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_TOUZHU_LOG_COUNT, info, sizeof(CMD_GP_GetTouzhuLogCount));
}

void MsgDispatch::SendPacketWithGetTouZhuLog(int nPage, int nSize, bool bTime, const char *szTimeStart, const char *szTimeEnd, const int caizhong,const int status)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetTouzhuLog)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetTouzhuLog));
	CMD_GP_GetTouzhuLog *info = (CMD_GP_GetTouzhuLog *)cbBuffer;
	info->dwUserID = m_dwUserID;
	info->nPage = nPage;
	info->nSize = nSize;
	info->bByTime = bTime;
	info->nCaiZhong = caizhong;
	info->nStatus = status;
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_TOUZHU_LOG, info, sizeof(CMD_GP_GetTouzhuLog));
}

//查询会员信息日志数
void MsgDispatch::SendPacketWithGetHYXXLogCount(bool bTime, const char *szTimeStart, const char *szTimeEnd, int n_t_type, int n_t_user_id, const char *s_t_account)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetHYXXLogCount)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetHYXXLogCount));
	CMD_GP_GetHYXXLogCount *info = (CMD_GP_GetHYXXLogCount *)cbBuffer;
	info->dwUserID = m_dwUserID;
	//    info->dwUserID = n_t_user_id;
	info->bTime = bTime;
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);
	info->n_t_type = n_t_type;
	info->n_t_user_id = n_t_user_id;
	MyNSString::mbs2wc(s_t_account, strlen(s_t_account), info->s_t_account);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_HYXX_LOG_COUNT, info, sizeof(CMD_GP_GetHYXXLogCount));
}

//查询会员信息
void MsgDispatch::SendPacketWithGetHYXXLog(int nPage, int nSize, bool bTime, const char *szTimeStart, const char *szTimeEnd, int n_t_type, int n_t_user_id, const char *s_t_account)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetHYXXLog)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetHYXXLog));
	CMD_GP_GetHYXXLog *info = (CMD_GP_GetHYXXLog *)cbBuffer;
	info->dwUserID = m_dwUserID;
	//    info->dwUserID = n_t_user_id;
	info->nPage = nPage;
	info->nSize = nSize;
	info->bByTime = bTime;
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);
	info->n_t_type = n_t_type;
	info->n_t_user_id = n_t_user_id;
	MyNSString::mbs2wc(s_t_account, strlen(s_t_account), info->s_t_account);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_HYXX_LOG, info, sizeof(CMD_GP_GetHYXXLog));
}

//获取下级投注统计数目
void MsgDispatch::SendPacketWithGetXJTZHLogCount(bool bTime, const char *szTimeStart, const char *szTimeEnd, int n_t_type, int n_t_user_id, const char *s_t_account)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetXJTZHLogCount)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetXJTZHLogCount));
	CMD_GP_GetXJTZHLogCount *info = (CMD_GP_GetXJTZHLogCount *)cbBuffer;
	info->dwUserID = m_dwUserID;
	//    info->dwUserID = n_t_user_id;
	info->bTime = bTime;
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);
	info->n_t_type = n_t_type;
	info->n_t_user_id = n_t_user_id;
	MyNSString::mbs2wc(s_t_account, strlen(s_t_account), info->s_t_account);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_XJTZH_LOG_COUNT, info, sizeof(CMD_GP_GetXJTZHLogCount));
}

//获取平台公告
void MsgDispatch::SendPacketWithGetNews()
{
	BYTE cbBuffer[sizeof(CMD_GP_GetNotic)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetNotic));
	CMD_GP_GetNotic *info = (CMD_GP_GetNotic *)cbBuffer;
	info->n_t_type = 1;

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_NOTICE, info, sizeof(CMD_GP_GetNotic));
} 

//获取活动
void MsgDispatch::SendPacketWithGetActive()
{
	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_ACTIONINFO);
} 

//平台公告返回
bool MsgDispatch::OnSocketWithGetNews(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GP_GetNoticRet  *info = (CMD_GP_GetNoticRet  *)cbBuffer;
	memcpy(info, buffer, size);	

	int count = size / sizeof(CMD_GP_GetNoticRet);

	Array *dataArray = Array::create();
	for(int i = 0 ; i < count ; i++){
		char s_t_news[5000] = {0};
		char s_t_title[100] = {0};

		Dictionary *data = Dictionary::create();

		MyNSString::wc2mbs((info + i)->s_t_content, sizeof((info+i)->s_t_content), s_t_news);
		data->setObject(String::create(s_t_news), "s_t_news");
		MyNSString::wc2mbs((info + i)->s_t_title, sizeof((info+i)->s_t_title), s_t_title);
		data->setObject(String::create(s_t_title), "s_t_title");

		//时间
		data->setObject(String::createWithFormat("%04d-%02d-%02d %02d:%02d:%02d", (info + i)->n_t_time.wYear,
			(info + i)->n_t_time.wMonth, (info + i)->n_t_time.wDay, (info + i)->n_t_time.wHour, (info + i)->n_t_time.wMinute,
			(info + i)->n_t_time.wSecond), "n_t_time");	

		dataArray->addObject(data);
	}

	NotificationCenter::getInstance()->postNotification("GetNoticeRet", dataArray);
	return true;
}

bool MsgDispatch::OnSocketWithGetActive(void *buffer, WORD size)
{
	//
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetActionInfoRet  *info = (CMD_GR_GetActionInfoRet  *)cbBuffer;
	memcpy(info, buffer, size);	

	int count = size / sizeof(CMD_GR_GetActionInfoRet);

	Array *dataArray = Array::create();

	for(int i = 0;i < count;i ++)
	{
		Dictionary* dic = new Dictionary();		
		//汉字
		char s_t_Remark[3100] = {0};
		MyNSString::wc2mbs((info + i)->s_t_Remark, sizeof((info + i)->s_t_Remark), s_t_Remark);
		dic->setObject(String::create(s_t_Remark),"s_t_Remark");		

		dataArray->addObject(dic);
	}

	NotificationCenter::getInstance()->postNotification("GetActiveRet", dataArray);

	return true;
}

//获取站内信
//SUB_GP_GET_INBOX_MESSAGE
void MsgDispatch::SendPacketWithGetInboxMessage(const int page,const int size,const int type)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetInboxMessage)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetInboxMessage));
	CMD_GP_GetInboxMessage *info = (CMD_GP_GetInboxMessage *)cbBuffer;
	info->n_t_page = page;
	info->n_t_size = size;
	info->n_t_typeid = type;
	info->n_t_userid = m_dwUserID;

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_INBOX_MESSAGE, info, sizeof(CMD_GP_GetInboxMessage));
} 

//站内信返回
bool MsgDispatch::OnSocketWithGetInboxMessage(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GP_GetInboxMessageRet *info = (CMD_GP_GetInboxMessageRet *)cbBuffer;
	memcpy(info, buffer, size);

	int count = size / sizeof(CMD_GP_GetInboxMessageRet);

	Array *dataArray = Array::create();

	for(int i = 0;i < count;i ++)
	{
		Dictionary* dic = new Dictionary();
		dic->setObject(Integer::create((info + i)->c_t_ifread),"c_t_ifread");
		dic->setObject(Integer::create((info + i)->n_rcv_userid),"n_rcv_userid");
		dic->setObject(Integer::create((info + i)->n_send_userid),"n_send_userid");
		dic->setObject(Integer::create((info + i)->n_t_count),"n_t_count");
		dic->setObject(Integer::create((info + i)->n_t_id),"n_t_id");
		dic->setObject(Integer::create((info + i)->n_t_rownum),"n_t_rownum");
		dic->setObject(Integer::create((info + i)->n_t_sitetype),"n_t_sitetype");
		dic->setObject(Integer::create((info + i)->n_t_ifSj), "n_t_ifSj");
		//时间
		dic->setObject(String::createWithFormat("%04d-%02d-%02d %02d:%02d:%02d", (info + i)->n_t_time.wYear,
			(info + i)->n_t_time.wMonth, (info + i)->n_t_time.wDay, (info + i)->n_t_time.wHour, (info + i)->n_t_time.wMinute,
			(info + i)->n_t_time.wSecond), "n_t_time");	
		//汉字
		char s_t_content[3100] = {0};
		MyNSString::wc2mbs((info + i)->s_t_content, sizeof((info + i)->s_t_content), s_t_content);
		dic->setObject(String::create(s_t_content),"s_t_content");
		char s_t_title[370] = {0};
		MyNSString::wc2mbs((info + i)->s_t_title, sizeof((info + i)->s_t_title), s_t_title);
		dic->setObject(String::create(s_t_title),"s_t_title");
		char s_t_username[99] = {0};
		MyNSString::wc2mbs((info + i)->s_t_username, sizeof((info + i)->s_t_username), s_t_username);
		dic->setObject(String::create(s_t_username),"s_t_username");

		dataArray->addObject(dic);
	}

	NotificationCenter::getInstance()->postNotification("GetInboxMessageRet", dataArray);

	return true;
}

//查看站内信
void MsgDispatch::SendPacketWithChkInboxMessage(const int id)
{
	BYTE cbBuffer[sizeof(CMD_GP_ChkInboxMessage)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_ChkInboxMessage));
	CMD_GP_ChkInboxMessage *info = (CMD_GP_ChkInboxMessage *)cbBuffer;
	info->n_t_id = id;
	info->n_t_userid = m_dwUserID;

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_CHK_INBOX_MESSAGE, info, sizeof(CMD_GP_ChkInboxMessage));
}

//查看站内信返回
bool MsgDispatch::OnSocketWithChkInboxMessage(void *buffer, WORD size)
{
	return true;

	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GP_ChkInboxMessage *info = (CMD_GP_ChkInboxMessage *)cbBuffer;
	memcpy(info, buffer, size);

	Dictionary* dic = new Dictionary();

	NotificationCenter::getInstance()->postNotification("ChkInboxMessageRet", dic);

	return true;
}

//获得所有下级用户信息
void MsgDispatch::SendPacketWithGetXJUserInfo(const char* name,const int type,const int size,const int type_id)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetAllUserInfo)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetAllUserInfo));
	CMD_GP_GetAllUserInfo *info = (CMD_GP_GetAllUserInfo *)cbBuffer;
	info->n_t_typeid = type;
	info->n_t_size = size;
	info->n_t_edittype = type_id;
	info->n_t_userid = m_dwUserID;
	info->n_t_count = 30;
//	info->s_t_search

	MyNSString::mbs2wc(name, strlen(name), info->s_t_search);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_ALL_USERINFO, info, sizeof(CMD_GP_GetAllUserInfo));
}

//删除id
void MsgDispatch::SendPacketWithDeleteMessageAt(const int type_id,const int id)
{
	//CMD_GP_DelMessage
	BYTE cbBuffer[sizeof(CMD_GP_DelMessage)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_DelMessage));
	CMD_GP_DelMessage *info = (CMD_GP_DelMessage *)cbBuffer;	

	info->n_t_id = id;
	info->n_t_type = type_id;

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_DEL_MESSAGE, info, sizeof(CMD_GP_DelMessage));
}

//删除信息返回
bool MsgDispatch::OnSocketWithDeleteMessageRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GP_DelMessageRet *info = (CMD_GP_DelMessageRet *)cbBuffer;
	memcpy(info, buffer, size);
	
	Dictionary* dic = new Dictionary();
	dic->setObject(Integer::create(info->n_t_retid),"n_t_retid");		

	NotificationCenter::getInstance()->postNotification("DeleteMessageRet", dic);

	return true;
}

//获得所有下级用户信息返回
bool MsgDispatch::OnSocketWithGetXJUserInfo(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GP_GetUserInfoRet *info = (CMD_GP_GetUserInfoRet *)cbBuffer;
	memcpy(info, buffer, size);

	int count  = size / sizeof(CMD_GP_GetUserInfoRet);

	Array* data = Array::create();
	
	for(int i = 0;i < count;i ++)
	{
		Dictionary* dic = new Dictionary();

		dic->setObject(Integer::create((info + i)->n_t_type),"n_t_type");
		dic->setObject(Integer::create((info + i)->n_t_userid),"n_t_userid");
		dic->setObject(Integer::create((info + i)->n_t_count), "n_t_count");  //总共的条数
		//名字
		char s_t_name[99] = {0};
		MyNSString::wc2mbs((info + i)->s_t_name, sizeof((info + i)->s_t_name), s_t_name);
		dic->setObject(String::create(s_t_name),"s_t_name");

		data->addObject(dic);
	}

	NotificationCenter::getInstance()->postNotification("GetXJUserInfoRet", data);

	return true;
}

//发送站内信
void MsgDispatch::SendPacketWithSendMessage(const int revid,const int sign,const int type,const char* content,const char* title)
{
	BYTE cbBuffer[sizeof(CMD_GP_SendMessage)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_SendMessage));
	CMD_GP_SendMessage *info = (CMD_GP_SendMessage *)cbBuffer;
	info->n_t_revid = revid;
	info->n_t_sign = sign;
	info->n_t_type = type;
	info->n_t_userid = m_dwUserID;
	//需要转
	MyNSString::mbs2wc(content, strlen(content), info->s_t_content);
	MyNSString::mbs2wc(title, strlen(title), info->s_t_title);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_SEND_MESSAGE, info, sizeof(CMD_GP_SendMessage));
}

//发送站内信返回
bool MsgDispatch::OnSocketWithSendMessage(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GP_SendMessageRet *info = (CMD_GP_SendMessageRet *)cbBuffer;
	memcpy(info, buffer, size);

	Dictionary* dic = new Dictionary();

	dic->setObject(Integer::create(info->n_t_retid),"n_t_retid");
	dic->setObject(Integer::create(info->n_t_sign),"n_t_sign");

	NotificationCenter::getInstance()->postNotification("SendMessageRet", dic);

	return true;
}

//获得站内信数量
void MsgDispatch::SendPacketWithGetMessageCount()
{
	BYTE cbBuffer[sizeof(CMD_GP_GetZnxCount)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetZnxCount));
	CMD_GP_GetZnxCount *info = (CMD_GP_GetZnxCount *)cbBuffer;
	info->n_t_userid = m_dwUserID;

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_ZNX_COUNT, info, sizeof(CMD_GP_GetZnxCount));
}

//站内信返回数量
bool MsgDispatch::OnSocketWithGetMessageCount(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GP_GetZnxCountRet *info = (CMD_GP_GetZnxCountRet *)cbBuffer;
	memcpy(info, buffer, size);

	Dictionary* dic = new Dictionary();

	dic->setObject(Integer::create(info->n_t_count), "nResult");

	NotificationCenter::getInstance()->postNotification("GetMessageCountRet", dic);

	return true;
}

//获取下级投注统计
void MsgDispatch::SendPacketWithGetXJTZHLog(int nPage, int nSize, bool bTime, const char *szTimeStart, const char *szTimeEnd, int n_t_type, int n_t_user_id, const char *s_t_account)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetXJTZHLog)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetXJTZHLog));
	CMD_GP_GetXJTZHLog *info = (CMD_GP_GetXJTZHLog *)cbBuffer;
	info->dwUserID = m_dwUserID;
	//    info->dwUserID = n_t_user_id;
	info->nPage = nPage;
	info->nSize = nSize;
	info->bByTime = bTime;
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);
	info->n_t_type = n_t_type;
	info->n_t_user_id = n_t_user_id;
	MyNSString::mbs2wc(s_t_account, strlen(s_t_account), info->s_t_account);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_XJTZH_LOG, info, sizeof(CMD_GP_GetXJTZHLog));
}

//查看下级投注数目
void MsgDispatch::SendPacketWithCHKXJTZHLogCount(int caizhong,int status,bool bTime, const char *szTimeStart, const char *szTimeEnd, int n_t_type, int n_t_user_id, const char *s_t_account)
{
	BYTE cbBuffer[sizeof(CMD_GP_CHKXJTZHLogCount)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_CHKXJTZHLogCount));
	CMD_GP_CHKXJTZHLogCount *info = (CMD_GP_CHKXJTZHLogCount *)cbBuffer;
	info->dwUserID = m_dwUserID;
	info->bTime = bTime;
	info->n_t_caizhong = caizhong;
	info->n_t_status = status;   //状态
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);
	info->n_t_type = n_t_type;
	info->n_t_user_id = n_t_user_id;
	MyNSString::mbs2wc(s_t_account, strlen(s_t_account), info->s_t_account);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_CHK_XJTZH_LOG_COUNT, info, sizeof(CMD_GP_CHKXJTZHLogCount));
}

//查看下级投注
void MsgDispatch::SendPacketWithCHKXJTZHLog(int caizhong,int status,int nPage, int nSize, bool bTime, const char *szTimeStart, const char *szTimeEnd, int n_t_type, int n_t_user_id, const char *s_t_account)
{
	BYTE cbBuffer[sizeof(CMD_GP_CHKXJTZHLog)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_CHKXJTZHLog));
	CMD_GP_CHKXJTZHLog *info = (CMD_GP_CHKXJTZHLog *)cbBuffer;
	info->dwUserID = m_dwUserID;
	info->nPage = nPage;
	info->nSize = nSize;
	info->n_t_caizhong = caizhong;
	info->n_t_status = status;
	info->bByTime = bTime;
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);
	info->n_t_type = n_t_type;
	info->n_t_user_id = n_t_user_id;
	MyNSString::mbs2wc(s_t_account, strlen(s_t_account), info->s_t_account);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_CHK_XJTZH_LOG, info, sizeof(CMD_GP_CHKXJTZHLog));
}

//查询下级游戏盈亏信息日志数
void MsgDispatch::SendPacketWithGetXJYXTjCount(bool bTime, const char *szTimeStart, const char *szTimeEnd, int n_t_type, int n_t_user_id, const char *s_t_account)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetXJYXTjCount)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetXJYXTjCount));
	CMD_GP_GetXJYXTjCount *info = (CMD_GP_GetXJYXTjCount *)cbBuffer;
	info->dwUserID = m_dwUserID;
	info->bTime = bTime;
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);
	info->n_t_type = n_t_type;
	info->n_t_user_id = n_t_user_id;
	MyNSString::mbs2wc(s_t_account, strlen(s_t_account), info->s_t_account);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_XJYX_TJ_COUNT, info, sizeof(CMD_GP_GetXJYXTjCount));
}

//查询下级游戏盈亏
void MsgDispatch::SendPacketWithGetXJYXTj(int nPage, int nSize, bool bTime, const char *szTimeStart, const char *szTimeEnd, int n_t_type, int n_t_user_id, const char *s_t_account)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetXJYXTj)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetXJYXTj));
	CMD_GP_GetXJYXTj *info = (CMD_GP_GetXJYXTj *)cbBuffer;
	info->dwUserID = m_dwUserID;
	info->nPage = nPage;
	info->nSize = nSize;
	info->bByTime = bTime;
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);
	info->n_t_type = n_t_type;
	info->n_t_user_id = n_t_user_id;
	MyNSString::mbs2wc(s_t_account, strlen(s_t_account), info->s_t_account);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_XJYX_TJ, info, sizeof(CMD_GP_GetXJYXTj));
}

//查询下级盈亏明细信息日志数
void MsgDispatch::SendPacketWithGetXJYKLogCount(bool bTime, const char *szTimeStart, const char *szTimeEnd, int n_t_type, int n_t_user_id, const char *s_t_account,const int nType)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetXJYKLogCount)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetXJYKLogCount));
	CMD_GP_GetXJYKLogCount *info = (CMD_GP_GetXJYKLogCount *)cbBuffer;
	info->dwUserID = m_dwUserID;
	info->bTime = bTime;
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);
	info->n_t_type = n_t_type;
	info->nType = nType;
	info->n_t_user_id = n_t_user_id;
	MyNSString::mbs2wc(s_t_account, strlen(s_t_account), info->s_t_account);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_XJYK_LOG_COUNT, info, sizeof(CMD_GP_GetXJYKLogCount));
}

//查询下级盈亏明细
void MsgDispatch::SendPacketWithGetXJYKLog(int nPage, int nSize, bool bTime, const char *szTimeStart, const char *szTimeEnd, int n_t_type, int n_t_user_id, const char *s_t_account,const int nType)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetXJYKLog)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetXJYKLog));
	CMD_GP_GetXJYKLog *info = (CMD_GP_GetXJYKLog *)cbBuffer;
	info->dwUserID = m_dwUserID;
	info->nPage = nPage;
	info->nSize = nSize;
	info->bByTime = bTime;
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);
	info->n_t_type = n_t_type;
	info->n_sort_type = nType;
	info->n_t_user_id = n_t_user_id;
	MyNSString::mbs2wc(s_t_account, strlen(s_t_account), info->s_t_account);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_XJYK_LOG, info, sizeof(CMD_GP_GetXJYKLog));
}

//查询下级盈亏统计信息日志数
void MsgDispatch::SendPacketWithGetXJYKTjCount(bool bTime, const char *szTimeStart, const char *szTimeEnd, int n_t_type, int n_t_user_id, const char *s_t_account)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetXJYKTjCount)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetXJYKTjCount));
	CMD_GP_GetXJYKTjCount *info = (CMD_GP_GetXJYKTjCount *)cbBuffer;
	info->dwUserID = m_dwUserID;
	info->bTime = bTime;
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);
	info->n_t_type = n_t_type;
	info->n_t_user_id = n_t_user_id;
	MyNSString::mbs2wc(s_t_account, strlen(s_t_account), info->s_t_account);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_XJYK_TJ_COUNT, info, sizeof(CMD_GP_GetXJYKTjCount));
}

void MsgDispatch::SendPacketWithGetParentRelative(const int xjUserId)
{
	BYTE cbBuffer[sizeof(CMD_GetParent)];
	memset(cbBuffer, 0 , sizeof(CMD_GetParent));
	CMD_GetParent *info = (CMD_GetParent *)cbBuffer;
	info->n_t_userid = m_dwUserID;
	info->n_t_xj_id = xjUserId;

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_PARENT, info, sizeof(CMD_GetParent));
	
}

void MsgDispatch::SendPacketWithGetGameKind(const int typeId)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetQipaiKind)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetQipaiKind));
	CMD_GP_GetQipaiKind *info = (CMD_GP_GetQipaiKind *)cbBuffer;
	info->n_t_typeid = typeId;

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_QIPAI_KIND, info, sizeof(CMD_GP_GetQipaiKind));

}

//查询下级盈亏统计
void MsgDispatch::SendPacketWithGetXJYKTj(int nPage, int nSize, bool bTime, const char *szTimeStart, const char *szTimeEnd, int n_t_type, int n_t_user_id, const char *s_t_account,const int sortType)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetXJYKTj)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetXJYKTj));
	CMD_GP_GetXJYKTj *info = (CMD_GP_GetXJYKTj *)cbBuffer;
	info->dwUserID = m_dwUserID;
	info->nPage = nPage;
	info->nSize = nSize;
	info->bByTime = bTime;
	info->n_sort_type = sortType;
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);
	info->n_t_type = n_t_type;
	info->n_t_user_id = n_t_user_id;
	MyNSString::mbs2wc(s_t_account, strlen(s_t_account), info->s_t_account);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_XJYK_TJ, info, sizeof(CMD_GP_GetXJYKTj));
}

//查询下级充值日志数
void MsgDispatch::SendPacketWithGetXjChzhLogCount(bool bTime, const char *szTimeStart, const char *szTimeEnd, int n_t_type, int n_t_user_id, const char *s_t_account)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetXjChzhLogCount)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetXjChzhLogCount));
	CMD_GP_GetXjChzhLogCount *info = (CMD_GP_GetXjChzhLogCount *)cbBuffer;
	info->dwUserID = m_dwUserID;
	info->bTime = bTime;
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);
	info->n_t_type = n_t_type;
	info->n_t_user_id = n_t_user_id;
	MyNSString::mbs2wc(s_t_account, strlen(s_t_account), info->s_t_account);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_XJCHZH_LOG_COUNT, info, sizeof(CMD_GP_GetXjChzhLogCount));
}

//查询下级充值
void MsgDispatch::SendPacketWithGetXJCHZHLog(int nPage, int nSize, bool bTime, const char *szTimeStart, const char *szTimeEnd, int n_t_type, int n_t_user_id, const char *s_t_account)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetXJCHZHLog)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetXJCHZHLog));
	CMD_GP_GetXJCHZHLog *info = (CMD_GP_GetXJCHZHLog *)cbBuffer;
	info->dwUserID = m_dwUserID;
	info->nPage = nPage;
	info->nSize = nSize;
	info->bByTime = bTime;
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);
	info->n_t_type = n_t_type;
	info->n_t_user_id = n_t_user_id;
	MyNSString::mbs2wc(s_t_account, strlen(s_t_account), info->s_t_account);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_XJCHZH_LOG, info, sizeof(CMD_GP_GetXJCHZHLog));
}

//查询下级提现日志数
void MsgDispatch::SendPacketWithGetXjTxLogCount(bool bTime, const char *szTimeStart, const char *szTimeEnd, int n_t_type, int n_t_user_id, const char *s_t_account)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetXjTxLogCount)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetXjTxLogCount));
	CMD_GP_GetXjTxLogCount *info = (CMD_GP_GetXjTxLogCount *)cbBuffer;
	info->dwUserID = m_dwUserID;
	info->bTime = bTime;
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);
	info->n_t_type = n_t_type;
	info->n_t_user_id = n_t_user_id;
	MyNSString::mbs2wc(s_t_account, strlen(s_t_account), info->s_t_account);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_XJTX_LOG_COUNT, info, sizeof(CMD_GP_GetXjTxLogCount));
}

//查询下级提现
void MsgDispatch::SendPacketWithGetXJTxLog(int nPage, int nSize, bool bTime, const char *szTimeStart, const char *szTimeEnd, int n_t_type, int n_t_user_id, const char *s_t_account)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetXJTxLog)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetXJTxLog));
	CMD_GP_GetXJTxLog *info = (CMD_GP_GetXJTxLog *)cbBuffer;
	info->dwUserID = m_dwUserID;
	info->nPage = nPage;
	info->nSize = nSize;
	info->bByTime = bTime;
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);
	info->n_t_type = n_t_type;
	info->n_t_user_id = n_t_user_id;
	MyNSString::mbs2wc(s_t_account, strlen(s_t_account), info->s_t_account);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_XJTX_LOG, info, sizeof(CMD_GP_GetXJTxLog));
}

//查询下级游戏记录日志数
void MsgDispatch::SendPacketWithGetQiPaiYingkuiCount(DWORD userID, int nTypeID, int nByTime,const char* username, const char *szTimeStart, const char *szTimeEnd,const int gameid)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetQiPaiYingkuiCount)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetQiPaiYingkuiCount));
	CMD_GP_GetQiPaiYingkuiCount *info = (CMD_GP_GetQiPaiYingkuiCount *)cbBuffer;
	info->n_t_userid = m_dwUserID;
	info->n_t_xjid = userID;
	info->nTypeID = nTypeID;
	info->nByTime = nByTime;
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);
	MyNSString::mbs2wc(username, strlen(username), info->s_t_account);
	info->nKindID = gameid;

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_QP_YINGKUI_COUNT, info, sizeof(CMD_GP_GetQiPaiYingkuiCount));
}

//查询下级游戏记录
void MsgDispatch::SendPacketWithGetQiPaiYingkui(const char *accStr, DWORD userID, int nPage, int nSize, bool bTime, const char *szTimeStart, const char *szTimeEnd,const int gameKind)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetQiPaiYingkui)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetQiPaiYingkui));
	CMD_GP_GetQiPaiYingkui *info = (CMD_GP_GetQiPaiYingkui *)cbBuffer;
	info->n_t_userid = m_dwUserID;
	info->n_t_xjid = userID;
	info->nPage = nPage;
	info->nSize = nSize;
	info->bByTime = bTime;
	info->nKindID = gameKind;
	//info->s_t_account = s_t_account;
	MyNSString::mbs2wc(accStr, strlen(accStr), info->s_t_account);
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_QP_YINGKUI, info, sizeof(CMD_GP_GetQiPaiYingkui));
}

void MsgDispatch::SendPacketWithGetTiXianLogCount(bool bTime, const char *szTimeStart, const char *szTimeEnd)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetTixianLogCount)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetTixianLogCount));
	CMD_GP_GetTixianLogCount *info = (CMD_GP_GetTixianLogCount *)cbBuffer;
	info->dwUserID = m_dwUserID;
	info->bTime = bTime;
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_TIXIAN_LOG_COUNT, info, sizeof(CMD_GP_GetTouzhuLogCount));
}	

void MsgDispatch::SendPacketWithGetTiXianLog(int nPage, int nSize, bool bTime, const char *szTimeStart, const char *szTimeEnd)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetTixianLog)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetTixianLog));
	CMD_GP_GetTixianLog *info = (CMD_GP_GetTixianLog *)cbBuffer;
	info->dwUserID = m_dwUserID;
	info->nPage = nPage;
	info->nSize = nSize;
	info->bByTime = bTime;
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_TIXIAN_LOG, info, sizeof(CMD_GP_GetTixianLog));
}	

void MsgDispatch::SendPacketWithGetChongzhiLogCount(bool bTime, const char *szTimeStart, const char *szTimeEnd)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetChongzhiLogCount)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetChongzhiLogCount));
	CMD_GP_GetChongzhiLogCount *info = (CMD_GP_GetChongzhiLogCount *)cbBuffer;
	info->dwUserID = m_dwUserID;
	info->bTime = bTime;
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_CHONGZHI_LOG_COUNT, info, sizeof(CMD_GP_GetChongzhiLogCount));
}	

void MsgDispatch::SendPacketWithGetChongzhiLog(int nPage, int nSize, bool bTime, const char *szTimeStart, const char *szTimeEnd)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetChongzhiLog)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetChongzhiLog));
	CMD_GP_GetChongzhiLog *info = (CMD_GP_GetChongzhiLog *)cbBuffer;
	info->dwUserID = m_dwUserID;
	info->nPage = nPage;
	info->nSize = nSize;
	info->bByTime = bTime;
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_CHONGZHI_LOG, info, sizeof(CMD_GP_GetChongzhiLog));
}

void MsgDispatch::SendPacketWithGetYingkuiLogCount(bool bTime, const char *szTimeStart, const char *szTimeEnd)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetYingkuiLogCount)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetYingkuiLogCount));
	CMD_GP_GetYingkuiLogCount *info = (CMD_GP_GetYingkuiLogCount *)cbBuffer;
	info->dwUserID = m_dwUserID;
	info->bTime = bTime;
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_YINGKUI_LOG_COUNT, info, sizeof(CMD_GP_GetYingkuiLogCount));
}	

void MsgDispatch::SendPacketWithGetYingkuiLog(int nPage, int nSize, bool bTime, const char *szTimeStart, const char *szTimeEnd)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetYingkuiLog)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetYingkuiLog));
	CMD_GP_GetYingkuiLog *info = (CMD_GP_GetYingkuiLog *)cbBuffer;
	info->dwUserID = m_dwUserID;
	info->nPage = nPage;
	info->nSize = nSize;
	info->bByTime = bTime;
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_YINGKUI_LOG, info, sizeof(CMD_GP_GetYingkuiLog));
}

void MsgDispatch::SendPacketWithGetQiPaiYingkuiLogCount(int nTypeID, int nByTime,const char* username, const char *szTimeStart, const char *szTimeEnd,const int gameid)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetQiPaiYingkuiCount)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetQiPaiYingkuiCount));
	CMD_GP_GetQiPaiYingkuiCount *info = (CMD_GP_GetQiPaiYingkuiCount *)cbBuffer;
	info->n_t_userid = m_dwUserID;
	info->n_t_xjid = m_dwUserID;
	info->nTypeID = nTypeID;
	info->nByTime = nByTime;
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);
	MyNSString::mbs2wc(username, strlen(username), info->s_t_account);
	info->nKindID = gameid;

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_QP_YINGKUI_COUNT, info, sizeof(CMD_GP_GetQiPaiYingkuiCount));
}	

void MsgDispatch::SendPacketWithGetQiPaiYingkuiLog(int nPage, int nSize, bool bTime, const char *szTimeStart, const char *szTimeEnd,const int gameKind)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetQiPaiYingkui)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetQiPaiYingkui));
	CMD_GP_GetQiPaiYingkui *info = (CMD_GP_GetQiPaiYingkui *)cbBuffer;
	info->n_t_userid = m_dwUserID;
	info->n_t_xjid = 0;
	info->nPage = nPage;
	info->nSize = nSize;
	info->bByTime = bTime;
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);
	info->nKindID = gameKind;

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_QP_YINGKUI, info, sizeof(CMD_GP_GetQiPaiYingkui));
}

void  MsgDispatch::SendPacketWithTouZhuCQSSC_ZhioHao(int zhuiHaoQiShu, char qiShu[100][20],
													 int gameType, int gameKind, string betNumber, int zhuShu, int beiTou[100], int moshi, int sign, int isEnd, int zhuiHao, int nIndex)
{
	BYTE cbBuffer[sizeof(CMD_GP_TouZhuCQSSC_Zhuihao)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_TouZhuCQSSC_Zhuihao));
	CMD_GP_TouZhuCQSSC_Zhuihao *info = (CMD_GP_TouZhuCQSSC_Zhuihao *)cbBuffer;

	info->dwUserID = m_dwUserID;
	for(int i = 0 ; i < 100 ; i++){
		memcpy(info->strQishu[i], qiShu[i],sizeof(info->strQishu[i]));
	}
	info->nGameType = gameType;
	info->nGameKind = gameKind;	

	info->nZhushu = zhuShu;
	memcpy(info->nBeitou, beiTou, sizeof(info->nBeitou));
	info->nMoshi = moshi;
	
	info->bZhuihao = zhuiHao;	
	info->nSign = m_wTouZhuSign ++;
	
	//最大长度
	int max_count = 14336;	
	
	if(betNumber.size() > max_count)
	{
		//压缩后的数组
		BYTE cbBuffer2[50000];
		memset(cbBuffer2, 0, 50000);
		size_t tempSize = 5;	//压缩后的大小	
		HDLzma::CompressBuffer(betNumber.c_str(),betNumber.size(),cbBuffer2,50000,&tempSize);	

		//计算次数		
		int iSendCount = tempSize;
		iSendCount = iSendCount / max_count + (iSendCount % max_count != 0 ? 1 : 0);

		for(int i = 0;i < iSendCount;i ++)
		{		
			int jCount = max_count * (i + 1);
			//大小
			if (jCount > tempSize)
			{
				jCount = tempSize;
			}

			for(int j = i * max_count;j < jCount;j ++)//max_count * (i + 1);j ++) 
			{
				info->strHaoma[j - i * max_count] = cbBuffer2[j];
			}

			if (i == iSendCount - 1)
			{
				info->nHaoMaLen = tempSize % max_count;
				info->nEnd = 0;
			} else
			{
				info->nHaoMaLen = max_count;
				info->nEnd = 1;
			}	
			info->nIndex = i;

			m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_TOUZHU_CQSSC_ZHUIHAO, info, sizeof(CMD_GP_TouZhuCQSSC_Zhuihao));
		}
	} else
	{
		memcpy(info->strHaoma, betNumber.c_str(),betNumber.length());
			info->nHaoMaLen = betNumber.length();
			info->nEnd = 0;
			m_pClientLoginSock->sendData(MDM_GR_USER, SUB_MB_TOUZHU_CQSSC_ZHUIHAO, info, sizeof(CMD_GP_TouZhuCQSSC_Zhuihao));		
	}
}

void MsgDispatch::SendPacketWithGetTouZhuXX(int id)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetTouzhu)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetTouzhu));
	CMD_GP_GetTouzhu *info = (CMD_GP_GetTouzhu *)cbBuffer;
	info->n_t_id = id;
	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_MB_GET_TOU_ZHU_XX, info, sizeof(CMD_GP_GetTouzhu));
}

void MsgDispatch::SendPacketWithGetTouZhuXXXX(int id) //add buy huangdi
{
	BYTE cbBuffer[sizeof(CMD_GP_GetTouzhu)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetTouzhu));
	CMD_GP_GetTouzhu *info = (CMD_GP_GetTouzhu *)cbBuffer;
	info->n_t_id = id;
	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_TOU_ZHU_XX, info, sizeof(CMD_GP_GetTouzhu));
}

void MsgDispatch::sendPacketWithGetKeFuURL() //add buy huangdi
{	
	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_KEFU_URL);
}

void MsgDispatch::SendPacketWithValidateCode()
{
	//m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_KEFU_URL);
}

void MsgDispatch::SendPacketWithBindMobileNum(const char* mobileNumber)
{	
	BYTE cbBuffer[sizeof(CMD_GP_BindPhone)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_BindPhone));
	CMD_GP_BindPhone *info = (CMD_GP_BindPhone *)cbBuffer;
	info->nUserID = m_dwUserID;
	info->cType = 1;
	MyNSString::mbs2wc(mobileNumber, strlen(mobileNumber), info->sPhoneNum);
	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_BIND_PHONE, info, sizeof(CMD_GP_BindPhone));
}

void MsgDispatch::SendPacketWithUnBindMobileNum()
{	
	BYTE cbBuffer[sizeof(CMD_GP_UnBindPhone)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_UnBindPhone));
	CMD_GP_UnBindPhone *info = (CMD_GP_UnBindPhone *)cbBuffer;
	info->nUserID = m_dwUserID;

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_UNBIND_PHONE, info, sizeof(CMD_GP_UnBindPhone));
}

void MsgDispatch::SendPacketWithSendYanZhengMa(const char* validateCode, const int iType)
{
	//CMD_SendYanZhengma   SUB_GP_SEND_YANZHENGMA
	BYTE cbBuffer[sizeof(CMD_SendYanZhengma)];
	memset(cbBuffer, 0 , sizeof(CMD_SendYanZhengma));
	CMD_SendYanZhengma *info = (CMD_SendYanZhengma *)cbBuffer;

	info->c_t_stype = 1;	
	info->c_t_type = 2;  //发送	
	
	info->n_t_userid = m_dwUserID;

	MyNSString::mbs2wc(validateCode, strlen(validateCode), info->s_t_code);

	if(iType == 0) //登陆
	{
		m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_SEND_YANZHENGMA, info, sizeof(CMD_SendYanZhengma));
	}else
	if(iType == 3)
	{
		m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_SEND_YANZHENGMA_TRANS, info, sizeof(CMD_SendYanZhengma));
	}
	
}

//验证
void MsgDispatch::SendPacketWithValidateYanZhengMa(const char* validateCode, const int iType)
{
	//CMD_SendYanZhengma   SUB_GP_SEND_YANZHENGMA
	BYTE cbBuffer[sizeof(CMD_SendYanZhengma)];
	memset(cbBuffer, 0 , sizeof(CMD_SendYanZhengma));
	CMD_SendYanZhengma *info = (CMD_SendYanZhengma *)cbBuffer;

	info->c_t_stype = 1;
	info->c_t_type = 3;//验证

	info->n_t_userid = m_dwUserID;

	MyNSString::mbs2wc(validateCode, strlen(validateCode), info->s_t_code);

	if(iType == 0) //登陆
	{
		m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_SEND_CHECKYANZHENGMA, info, sizeof(CMD_SendYanZhengma));
	}else
	if(iType == 3)
	{
		m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_SEND_CHECKYANZHENGMA_TRANS, info, sizeof(CMD_SendYanZhengma));
	}
	
}

//获取手机号码和转帐需要
void MsgDispatch::SendPacketWithGetTransPhone(const int iType)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetPhoneTransferFenhong)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetPhoneTransferFenhong));
	CMD_GP_GetPhoneTransferFenhong *info = (CMD_GP_GetPhoneTransferFenhong *)cbBuffer;

	info->n_t_type = iType;
	info->n_t_userid = m_dwUserID;

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_PHONE_TRASFER_FENHONG, info, sizeof(CMD_GP_GetPhoneTransferFenhong));
}

void MsgDispatch::SendPacketWithSetValidateType(const int fenhong,const int login,const int transfer)
{
	BYTE cbBuffer[sizeof(CMD_GP_SetBindPhoneInfo)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_SetBindPhoneInfo));
	CMD_GP_SetBindPhoneInfo *info = (CMD_GP_SetBindPhoneInfo *)cbBuffer;
	info->nUserID = m_dwUserID;
	info->cType = 2;
	info->cfenhong = fenhong;	//分红
	info->cLoginverify = login;	//登陆
	info->cTranferAccount = transfer;//转帐
	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_BIND_PHONE_INFO, info, sizeof(CMD_GP_SetBindPhoneInfo));
}

void MsgDispatch::SendPacketWithGetValidateType()
{
	BYTE cbBuffer[sizeof(CMD_GP_SetBindPhoneInfo)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_SetBindPhoneInfo));
	CMD_GP_SetBindPhoneInfo *info = (CMD_GP_SetBindPhoneInfo *)cbBuffer;
	info->nUserID = m_dwUserID;
	info->cType = 1;
	info->cfenhong = 0;	//分红
	info->cLoginverify = 0;	//登陆
	info->cTranferAccount = 0;//转帐
	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_BIND_PHONE_INFO, info, sizeof(CMD_GP_SetBindPhoneInfo));
}

void MsgDispatch::sendPacketWithGetQuKuanLimit()
{
	//SUB_GP_GET_QUKUAN_LIMIT
	BYTE cbBuffer[sizeof(CMD_GP_GetQukuanLimit)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetQukuanLimit));
	CMD_GP_GetQukuanLimit *info = (CMD_GP_GetQukuanLimit *)cbBuffer;
	info->n_t_userid = m_dwUserID;
	m_pClientLoginSock->sendData(MDM_GR_USER,SUB_GP_GET_QUKUAN_LIMIT, info, sizeof(CMD_GP_GetQukuanLimit));
}

void MsgDispatch::SendPacketWithGetSystemTime()
{
	//if(m_eDataType != CMD_Logon)
	//	return;
	//检测网路有效性,2018.8:重要且有效！
	if(m_loginState == enLogin_Server)
	{
		if (!m_pClientLoginSock->isServerConected())
		{
			this->connectLoginServer();
		}
		m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_QUERY_TIME);

	}
}

void MsgDispatch::SendPacketWithCancelTouZhu(int t_id, int t_kindid, string strQiHao)
{
	BYTE cbBuffer[sizeof(CMD_GP_CancelTouzhu)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_CancelTouzhu));
	CMD_GP_CancelTouzhu *info = (CMD_GP_CancelTouzhu *)cbBuffer;
	info->n_t_userid = m_dwUserID;
	info->n_t_id = t_id;
	info->n_t_kindid = t_kindid;
	MyNSString::mbs2wc(strQiHao.c_str(), strQiHao.size(), info->s_t_qihao);
	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_CANCEL_TOUZHU, info, sizeof(CMD_GP_CancelTouzhu));
}

void MsgDispatch::SendPacketWithGetChongzhiType(BYTE	cbType)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetChongzhiType)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetChongzhiType));
	CMD_GP_GetChongzhiType *info = (CMD_GP_GetChongzhiType *)cbBuffer;
	info->nUserID = m_dwUserID;
	info->cbType = cbType;
	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_CHONG_ZHI_TYPE, info, sizeof(CMD_GP_GetChongzhiType));
}

void MsgDispatch::SendPacketWithGetQuKuanInfo(BYTE	cbType)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetQukuanInfo)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetQukuanInfo));
	CMD_GP_GetQukuanInfo *info = (CMD_GP_GetQukuanInfo *)cbBuffer;
	info->dwUserID = m_dwUserID;
	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_QU_KUAN_INFO, info, sizeof(CMD_GP_GetQukuanInfo));
}

void MsgDispatch::SendPacketWithDoQukuan(DOUBLE fJine, string password)
{
	BYTE cbBuffer[sizeof(CMD_GP_DoQukuan)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_DoQukuan));
	CMD_GP_DoQukuan *info = (CMD_GP_DoQukuan *)cbBuffer;
	info->dwUserID = m_dwUserID;
	info->fJine = fJine;

	char szMD5[33];
	memset(szMD5,0,sizeof(szMD5));
	CMD5Encrypt::EncryptData((unsigned char*)password.c_str(), szMD5);
	MyNSString::mbs2wc(szMD5, strlen(szMD5), info->szQukuanPass);
	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_DO_QU_KUAN, info, sizeof(CMD_GP_DoQukuan));
}

void MsgDispatch::SendPacketWithSetQukuanZhanghao(string szKaihuYinghang,string szKaihuRen,
												  string szYinhangZhanghu,string szQukuanPass)
{
	BYTE cbBuffer[sizeof(CMD_GP_SetQukuanZhanghao)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_SetQukuanZhanghao));
	CMD_GP_SetQukuanZhanghao *info = (CMD_GP_SetQukuanZhanghao *)cbBuffer;
	info->dwUserID = m_dwUserID;
	MyNSString::mbs2wc(szKaihuYinghang.c_str(), strlen(szKaihuYinghang.c_str()), info->szKaihuYinghang);
	MyNSString::mbs2wc(szKaihuRen.c_str(), strlen(szKaihuRen.c_str()), info->szKaihuRen);
	MyNSString::mbs2wc(szYinhangZhanghu.c_str(), strlen(szYinhangZhanghu.c_str()), info->szYinhangZhanghu);

	char szMD5[33];
	memset(szMD5,0,sizeof(szMD5));
	CMD5Encrypt::EncryptData((unsigned char*)szQukuanPass.c_str(), szMD5);
	MyNSString::mbs2wc(szMD5, strlen(szMD5), info->szQukuanPass);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_SET_QUKUAN_ZHANGHAO, info, sizeof(CMD_GP_SetQukuanZhanghao));
}

void MsgDispatch::QueryMyYinHang()
{
	BYTE cbBuffer[sizeof(CMD_GP_QueryMyYinHang)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_QueryMyYinHang));
	CMD_GP_QueryMyYinHang *info = (CMD_GP_QueryMyYinHang *)cbBuffer;
	info->dwUserID = m_dwUserID;

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_QUERY_MY_YINHANG, info, sizeof(CMD_GP_QueryMyYinHang));
}

void MsgDispatch::QueryYinHang()
{
	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_QUERY_YINHANG);
}

void MsgDispatch::SendPacketWithAddHuiYuan(int n_type, DOUBLE f_t_fandian,
										   string s_t_account,string s_t_login_pass,string s_t_qukuan_pass,string s_t_qq)
{
	BYTE cbBuffer[sizeof(CMD_GP_AddHuiYuan)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_AddHuiYuan));
	CMD_GP_AddHuiYuan *info = (CMD_GP_AddHuiYuan *)cbBuffer;
	info->n_t_daili_id = m_dwUserID;
	info->n_type = n_type;
	info->f_t_fandian = f_t_fandian;
	MyNSString::mbs2wc(s_t_account.c_str(), strlen(s_t_account.c_str()), info->s_t_account);
	MyNSString::mbs2wc(s_t_qq.c_str(), strlen(s_t_qq.c_str()), info->s_t_qq);

	char szMD5[33]={0};
	char szMD51[33]= {0};
	memset(szMD5,0,sizeof(szMD5));
	CMD5Encrypt::EncryptData((unsigned char*)s_t_qukuan_pass.c_str(), szMD5);
	MyNSString::mbs2wc(szMD5, strlen(szMD5), info->s_t_qukuan_pass);
	memset(szMD51,0,sizeof(szMD51));
	CMD5Encrypt::EncryptData((unsigned char*)s_t_login_pass.c_str(), szMD51);
	MyNSString::mbs2wc(szMD51, strlen(szMD51), info->s_t_login_pass);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_ADD_HY, info, sizeof(CMD_GP_AddHuiYuan));
}

void MsgDispatch::SendPacketWithPeiE()
{
	BYTE cbBuffer[sizeof(CMD_GetPeie)];
	memset(cbBuffer, 0 , sizeof(CMD_GetPeie));
	CMD_GetPeie *info = (CMD_GetPeie *)cbBuffer;
	info->n_t_userid = m_dwUserID;		

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_Peie, info, sizeof(CMD_GetPeie));
}

void MsgDispatch::SendPacketWithWebFanDian(DOUBLE f_fandian)
{
	BYTE cbBuffer[sizeof(CMD_GP_SetWebFandian)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_SetWebFandian));
	CMD_GP_SetWebFandian *info = (CMD_GP_SetWebFandian *)cbBuffer;
	info->n_t_userid = m_dwUserID;
	info->f_t_fandian = f_fandian;	
	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_SET_WEB_FANDIAN, info, sizeof(CMD_GP_SetWebFandian));
}

void MsgDispatch::SendPacketWithQueryWebFanDian()
{
	BYTE cbBuffer[sizeof(CMD_GetUrlFandian)];
	memset(cbBuffer, 0 , sizeof(CMD_GetUrlFandian));
	CMD_GetUrlFandian *info = (CMD_GetUrlFandian *)cbBuffer;
	info->n_t_userid = m_dwUserID;
	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_REG_URL, info, sizeof(CMD_GetUrlFandian));
}


void MsgDispatch::SendPacketWithTouZhuCQSSC_FanDian( int gameType, int gameKind)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetUserFandian)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetUserFandian));
	CMD_GP_GetUserFandian *info = (CMD_GP_GetUserFandian *)cbBuffer;
	info->n_t_userid = m_dwUserID;
	info->n_t_type_id = gameType;
	info->n_t_kind_id = gameKind;
	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_USER_FANDIAN, info, sizeof(CMD_GP_GetUserFandian));
}

void MsgDispatch::SendPacketWithGetHyShj(string start, string end, bool isOnlyToday)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetHyShj)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetHyShj));
	CMD_GP_GetHyShj *info = (CMD_GP_GetHyShj *)cbBuffer;
	info->dwUserID = m_dwUserID;
	info->cbOnlyToday = isOnlyToday;

	MyNSString::mbs2wc(start.c_str(), strlen(start.c_str()), info->szTimeStart);
	MyNSString::mbs2wc(end.c_str(), strlen(end.c_str()), info->szTimeEnd);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_HYSHJ, info, sizeof(CMD_GP_GetHyShj));
}

//设置彩票返点
void MsgDispatch::SendPacketWithGetTouZhuSetUserBonus(int Type_id,int Kind_id,DOUBLE bonus,DOUBLE bonuspercent)
{
	BYTE cbBuffer[sizeof(CMD_GP_SetUserBonus)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_SetUserBonus));
	CMD_GP_SetUserBonus *info = (CMD_GP_SetUserBonus *)cbBuffer;
	info->n_t_userid = m_dwUserID;
	info->n_t_type_id = Type_id;
	info->n_t_kind_id = Kind_id;
	info->f_t_bonus = bonus;
	info->f_bonuspercent = bonuspercent;
	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_SET_USER_BONUS	, info, sizeof(CMD_GP_SetUserBonus));

}

//获取个人基本信息
void MsgDispatch::SendPacketWithGetUserInfo()
{
	BYTE cbBuffer[sizeof(CMD_GP_GetUserInfo)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetUserInfo));
	CMD_GP_GetUserInfo *info = (CMD_GP_GetUserInfo *)cbBuffer;
	info->dwUserID = m_dwUserID;
	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_USER_INFO	, info, sizeof(CMD_GP_GetUserInfo));

}

//发送退出服务器
void MsgDispatch::SendExitGame()
{
	BYTE cbBuffer[sizeof(CMD_GP_QuitGame)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_QuitGame));
	CMD_GP_QuitGame *info = (CMD_GP_QuitGame *)cbBuffer;
	info->dwUserID = m_dwUserID;
	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_QUIT_GAME	, info, sizeof(CMD_GP_QuitGame));

}

//为0都不显示只显示断线 为1显示重连和成功  为其它只显示成功
void MsgDispatch::SendReConnect(int typeId)
{
	//断线重连文字
	if(typeId > 1) NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_NETWORK_RECON_ING);

	//重新连接
	std::string userAccount = UserDefault::getInstance()->getStringForKey("NEWZJD_ACCOUNT");
	std::string userPassword = UserDefault::getInstance()->getStringForKey("NEWZJD_PASSWORD");

	bool bRet = EntityMgr::instance()->getDispatch()->connectLoginServer();
	if (!bRet)
	{		
		//失败	
		if(typeId == 0) NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_LS_NETWORKSHUT);
	}
	else
	{
		//成功
		NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_NETWORK_RECON,Integer::create(typeId));

		EntityMgr::instance()->login()->setUserLogin(userAccount, userPassword);
	}
}

//获取最新余额信息
void MsgDispatch::SendPacketWithGetLastYue()
{
	BYTE cbBuffer[sizeof(CMD_GP_GetLastYue)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetLastYue));
	CMD_GP_GetLastYue *info = (CMD_GP_GetLastYue *)cbBuffer;
	info->dwUserID = m_dwUserID;
	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_LAST_YUE, info, sizeof(CMD_GP_GetLastYue));

}

void MsgDispatch::SendPacketWithGetLiuHeFenDan()
{
	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_LHC_QIHAO);
}

void MsgDispatch::SendPacketWithGetRankList()
{
	BYTE cbBuffer[sizeof(CMD_GP_GetNews)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetNews));
	CMD_GP_GetNews *info = (CMD_GP_GetNews *)cbBuffer;

	
	info->n_t_id = m_dwUserID;
	info->n_t_ccount = 10;

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_NEWS, info, sizeof(CMD_GP_GetNews));
	
}

//获取韩国1.5分彩的期号差
void MsgDispatch::SendPacketWithGetHgydwfcDelta()
{
	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_QIHAO_CHA);
}

//获取加拿大3.5分彩的期号
void MsgDispatch::SendPacketWithGetCanadaSDWFQiHao()
{
	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_CANADA_START_QIHAO);
}

//获取跨年期号
void MsgDispatch::SendPacketWithGetKuaNianQiHao(int gameKind)
{
	//CMD_GP_QueryStatusLottery
	BYTE cbBuffer[sizeof(CMD_GP_QueryStatusLottery)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_QueryStatusLottery));
	CMD_GP_QueryStatusLottery *info = (CMD_GP_QueryStatusLottery *)cbBuffer;
	info->n_t_kindid = gameKind;

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_QUERY_STATUS_LOTTERY,info, sizeof(CMD_GP_QueryStatusLottery));
}

//签到
void MsgDispatch::SendPacketWithSignUp()
{
	BYTE cbBuffer[sizeof(CMD_GP_UserQiandao)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_UserQiandao));
	CMD_GP_UserQiandao *info = (CMD_GP_UserQiandao *)cbBuffer;
	info->dwUserID = m_dwUserID;
	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_USER_QIANDAO, info, sizeof(CMD_GP_UserQiandao));
}

//代理签到
void MsgDispatch::SendPacketWithDailiSignUp()
{
	BYTE cbBuffer[sizeof(CMD_GP_DailiLingjiang)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_DailiLingjiang));
	CMD_GP_DailiLingjiang *info = (CMD_GP_DailiLingjiang *)cbBuffer;
	info->n_t_userid = m_dwUserID;
	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_DAILI_LJ, info, sizeof(CMD_GP_DailiLingjiang));
}

//充值送
void MsgDispatch::SendPacketWithChongZhiSongGet()
{
	BYTE cbBuffer[sizeof(CMD_GP_GetChongZhiSong)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetChongZhiSong));
	CMD_GP_GetChongZhiSong *info = (CMD_GP_GetChongZhiSong *)cbBuffer;
	info->n_t_userid = m_dwUserID;
	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_ChongZhiSong, info, sizeof(CMD_GP_GetChongZhiSong));
}

//充值送确定
void MsgDispatch::SendPacketWithChongZhiSongSet()
{
	BYTE cbBuffer[sizeof(CMD_GP_SetChongZhiSong)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_SetChongZhiSong));
	CMD_GP_SetChongZhiSong *info = (CMD_GP_SetChongZhiSong *)cbBuffer;
	info->n_t_userid = m_dwUserID;
	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_SET_ChongZhiSong, info, sizeof(CMD_GP_SetChongZhiSong));
}

//代理签到查询信息
void MsgDispatch::SendPacketWithDailiSignUpInfo()
{
	BYTE cbBuffer[sizeof(CMD_GP_GetDailiHuikui)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetDailiHuikui));
	CMD_GP_GetDailiHuikui *info = (CMD_GP_GetDailiHuikui *)cbBuffer;
	info->n_t_userid = m_dwUserID;
	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_DAILI_HUIKUI, info, sizeof(CMD_GP_GetUserHuanlesong));
}

void MsgDispatch::SendPacketWithZhuanZhang(DOUBLE money,int targetID,int i_type,string password)
{
	BYTE cbBuffer[sizeof(CMD_GP_HYXX_ZhuanZhang)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_HYXX_ZhuanZhang));
	CMD_GP_HYXX_ZhuanZhang *info = (CMD_GP_HYXX_ZhuanZhang *)cbBuffer;	
	info->f_t_jine = money;  //金额
	info->n_t_target_id = targetID;//目标id
	info->n_t_type = i_type;//转帐类型
	info->n_t_user_id = m_dwUserID;//用户id

	char szMD5[33];
	memset(szMD5,0,sizeof(szMD5));
	CMD5Encrypt::EncryptData((unsigned char*)password.c_str(), szMD5);
	MyNSString::mbs2wc(szMD5, PASS_LEN, info->s_t_password);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_HYXX_ZHUANZHANG, info, sizeof(CMD_GP_HYXX_ZhuanZhang));
}

void MsgDispatch::SendPacketWithXiaJiPeiE()
{
	//SUB_GP_GET_XJ_Peie
	BYTE cbBuffer[sizeof(CMD_GetXjPeie)];
	memset(cbBuffer, 0 , sizeof(CMD_GetXjPeie));
	CMD_GetXjPeie *info = (CMD_GetXjPeie *)cbBuffer;

	info->n_t_userid = m_dwUserID;

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_XJ_Peie, info, sizeof(CMD_GetXjPeie));
}

//设置下级返点配额
void MsgDispatch::SendPacketSetXiaJiPeiE(const int user_id,const int peiE1,const int peiE2,const int peiE3,const int peiE4,const int peiE5)
{
	//SUB_GP_SET_XJ_Peie
	BYTE cbBuffer[sizeof(CMD_SetXjPeie)];
	memset(cbBuffer, 0 , sizeof(CMD_SetXjPeie));
	CMD_SetXjPeie *info = (CMD_SetXjPeie *)cbBuffer;

	info->n_t_userid = m_dwUserID;
	info->n_t_xj_id = user_id;

#ifdef VER_QIANBO
	info->n_t_peie1 = peiE1;
	info->n_t_peie2 = peiE2;
	info->n_t_peie3 = peiE3;
	info->n_t_peie4 = peiE4;
	info->n_t_peie5 = peiE5;
#endif

#ifdef VER_369
	info->n_t_peie1 = peiE1;
	info->n_t_peie2 = peiE2;
	info->n_t_peie3 = peiE3;
	info->n_t_peie4 = peiE4;
	info->n_t_peie5 = peiE5;
#endif

#ifdef VER_9YI
	info->n_t_peie1 = peiE1;
	info->n_t_peie2 = peiE3;
	info->n_t_peie3 = peiE4;
#endif

	m_pClientLoginSock->sendData(MDM_GR_USER,SUB_GP_SET_XJ_Peie,info,sizeof(CMD_SetXjPeie));
}

//设置下级返点
void MsgDispatch::SendPacketSetXiaJiFanDian(const int user_id,const double fandian)
{
	//SUB_GP_SET_XJ_Peie
	BYTE cbBuffer[sizeof(CMD_GP_XGHYFandian)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_XGHYFandian));
	CMD_GP_XGHYFandian *info = (CMD_GP_XGHYFandian *)cbBuffer;

	info->dwUserID = m_dwUserID;
	info->f_Fandian = fandian;
	info->n_xiaji_id = user_id;

	m_pClientLoginSock->sendData(MDM_GR_USER,SUB_GP_XG_HY_FANDIAN,info,sizeof(CMD_GP_XGHYFandian));
}

//查询签到欢乐颂信息
void MsgDispatch::SendPacketWithGetSignUpInfo()
{
	BYTE cbBuffer[sizeof(CMD_GP_GetUserHuanlesong)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetUserHuanlesong));
	CMD_GP_GetUserHuanlesong *info = (CMD_GP_GetUserHuanlesong *)cbBuffer;
	info->dwUserID = m_dwUserID;
	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_USER_HUANLESONG, info, sizeof(CMD_GP_GetUserHuanlesong));
}

//棋牌彩票互转
void MsgDispatch::SendPacketWithZhuanHuanMoney(int n_t_type, DOUBLE f_t_jine)
{
	BYTE cbBuffer[sizeof(CMD_GP_ZhuanHuan)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_ZhuanHuan));
	CMD_GP_ZhuanHuan *info = (CMD_GP_ZhuanHuan *)cbBuffer;
	info->n_t_userid = m_dwUserID;
	info->n_t_type = n_t_type;
	info->f_t_jine = f_t_jine;
	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_ZHUANHUAN, info, sizeof(CMD_GP_ZhuanHuan));

}

void MsgDispatch::onExitProgram(Node* obj)
{
	Director::getInstance()->end();
	//退出黑屏
	exit(0);
}