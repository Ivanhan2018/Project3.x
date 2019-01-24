#include "MsgDispatch.h"
#include "GBEventIDs.h"
#include "GlobalUnits.h"
#include "CMD_LogonServer.h"
#include "IDataArchive.h"
#include "GlobalUnits.h"
#include "GlobalField.h"
#include <string>
#include "Encrypt.h"
#include "GlobalFrame.h"
#include "utf-8.h"
#include "ConfigMgr.h"
#include "packet.h"
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
#include "MyConfig.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//#include "../JniHelper.h"
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
	t = time(NULL);
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
	m_eDataType = CMD_Logon;
	m_dwKindID = 122;
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
	m_dwGameID		= 0;
	m_cbStatus		= US_NULL;
	m_dwGoldScore   = 0;

	m_pClientLoginSock = new ClientSock();
	m_pClientGameSock  = new ClientSock();
	memset(szDescribeString, 0, sizeof(szDescribeString));
	memset(m_wArticleNum, 0, sizeof(m_wArticleNum));
	current_time_t = 0;
	m_nDeltaTime = 0;
}

MsgDispatch::~MsgDispatch()
{	
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(MsgDispatch::update),this);
	CC_SAFE_DELETE(m_pClientLoginSock);
	CC_SAFE_DELETE(m_pClientGameSock);
}

void MsgDispatch::startSchedule()
{
	Director::getInstance()->getScheduler()->schedule(schedule_selector(MsgDispatch::update),this,0,false);	
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
	if (m_loginState == enLogin_Server)
	{
		m_pClientLoginSock->update();
	}else
	{
		m_pClientGameSock->update();
	}
}

void MsgDispatch::setGameStatus(bool bStatus)
{
	m_bGameStatus = bStatus;
}

void MsgDispatch::onEventSocketRecv(GBEventConstArg& arg)
{
	tagBuffer& tag = (tagBuffer&)arg;
	memmove(m_cbRecvBuff + m_wRecvSize, tag.dateBuffer, 81920 - m_wRecvSize);

	m_wRecvSize += tag.nDataSize;

	WORD wPacketSize=0;
	BYTE cbDataBuffer[SOCKET_PACKET+sizeof(CMD_Head)];
	memset(cbDataBuffer, 0, SOCKET_PACKET+sizeof(CMD_Head));
	CMD_Head * pHead=(CMD_Head*)m_cbRecvBuff;
	while (m_wRecvSize>=sizeof(CMD_Head))
	{
		wPacketSize = pHead->CmdInfo.wPacketSize;
		if (wPacketSize < sizeof(CMD_Head))
		{
			CCLOG("data error,small than head data");
			return;
		}

		if (pHead->CmdInfo.cbVersion != SOCKET_VER)
		{
			CCLOG("version error");
			return;
		}

		if (m_wRecvSize < wPacketSize)
		{
			return;
		}

		m_dwRecvPacketCount++;

		memcpy(cbDataBuffer,m_cbRecvBuff,wPacketSize);
		m_wRecvSize-=wPacketSize;
		memmove(m_cbRecvBuff,m_cbRecvBuff + wPacketSize,m_wRecvSize);

		WORD wRealySize = wPacketSize;
		if (m_loginState == enLogin_Server)
		{
#ifdef VER_369
			wRealySize=m_pClientLoginSock->crevasseBuffer(cbDataBuffer,wPacketSize);
#else
			if(!Common_unMappedBuffer(cbDataBuffer,wPacketSize)){
				CCLOG("Common_unMappedBuffer FAILED");
			}
#endif
		}
		else
		{
#ifdef VER_369
			wRealySize=m_pClientGameSock->crevasseBuffer(cbDataBuffer,wPacketSize);
#else
			if(!Common_unMappedBuffer(cbDataBuffer,wPacketSize)){
				CCLOG("Common_unMappedBuffer FAILED");
			}
#endif
		}

		WORD wDataSize=wRealySize-sizeof(CMD_Head);
		void * pDataBuffer=cbDataBuffer + sizeof(CMD_Head);
		CMD_Command Command=((CMD_Head*)cbDataBuffer)->CommandInfo;

		if(Command.wMainCmdID == 0 &&Command.wSubCmdID == 1)
		{
			return;
		}

		if (Command.wMainCmdID == MDM_KN_COMMAND)
		{
			switch (Command.wSubCmdID)
			{
			case SUB_KN_DETECT_SOCKET:
				{
					if (m_loginState == enLogin_Server)
					{
						SendPacketWithCompilatio(0);
					}
					else
					{
						SendPacketWithCompilatio(1);
					}
					break;
				}
			}
			continue;
		}

		CCLOG("wMainCmdID = %d,wSubCmdID = %d",Command.wMainCmdID,Command.wSubCmdID);
		CCLOG("m_eDataType = %d", m_eDataType);
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

void MsgDispatch::setLoginState(enLoginState state)
{
	m_loginState = state;
}


bool MsgDispatch::onEventTcpLoginRead(WORD wSocketId,CMD_Command* pCommand,void* pData,WORD wDataSize)
{
	switch (pCommand->wMainCmdID)
	{
	case MDM_GP_LOGON:
		{
			switch (pCommand->wSubCmdID)
			{
			case SUB_MB_LOGON_SUCCESS:
				{
					return this->OnSocketSubGPLogonSuccess(pData, wDataSize);
				}
			case SUB_MB_LOGON_FAILURE:
				{
					return this->OnSocketSubGPLogonFailure(pData, wDataSize);
				}
			default:
				return true;
			}
		}
	case MDM_MB_SERVER_LIST:
		return OnTCPNetworkMainMBServerList(pCommand->wSubCmdID,pData,wDataSize);
	case MDM_GP_SERVER_LIST:
		return OnTCPNetworkMainMBServerList(pCommand->wSubCmdID,pData,wDataSize);
	case MDM_MB_USER_SERVICE:
		return OnTCPNetworkMainMBIphoneUserService(pCommand->wSubCmdID, pData,wDataSize);
	case MDM_KN_COMMAND:
		return true;
	case MDM_GP_USER_SERVICE:
		return onSocketMainUser(pCommand, pData,wDataSize);
	default:
		return false;
	}
	return true;
}

bool MsgDispatch::OnTCPNetworkMainMBServerList(int wSubCmdID, void *buffer, WORD size)
{
	switch (wSubCmdID)
	{
	case SUB_MB_LIST_KIND:
		{
			return this->OnSocketSubListKind(buffer, size);
		}
	case SUB_MB_LIST_SERVER:
		{
			return this->OnSocketSubListServer(buffer, size);
		}
	case SUB_MB_LIST_FINISH:
		{
			return this->OnSocketSubListFinish(buffer, size);
		}
	}
	return true;
}

bool MsgDispatch::OnTCPNetworkMainMBIphoneUserService(int wSubCmdID, void *buffer, WORD size)
{
	return true;
}

bool MsgDispatch::OnSocketSubGPLogonSuccess(void *buffer, WORD size)
{
	CCLOG("OnSocketSubLogonSuccess");

	BYTE cbBuffer[sizeof(CMD_GP_LogonSuccess)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_LogonSuccess));
	CMD_GP_LogonSuccess *logonSuccess = (CMD_GP_LogonSuccess *)cbBuffer;
	memcpy(logonSuccess, buffer, sizeof(CMD_GP_LogonSuccess));

	//char nicknameArray[128]={0};
	//MyNSString::wc2mbs(logonSuccess->szNickName, sizeof(logonSuccess->szNickName), nicknameArray);
	//m_pNickname = string(nicknameArray);

	m_cbGender = logonSuccess->cbGender;
	m_wFaceID = logonSuccess->wFaceID;
	m_dwUserID = logonSuccess->dwUserID;
	m_dwGoldScore = logonSuccess->f_yue;
	m_dwExperience = logonSuccess->dwExperience;
	m_cbType = logonSuccess->n_type;
	f_fandian = logonSuccess->f_fandian;

	f_qipai_yue = logonSuccess->lScore;
	f_yue = logonSuccess->f_yue;

	tagGlobalUserData* pUserData = new tagGlobalUserData;
	memset(pUserData, 0, sizeof(tagGlobalUserData));
	//memcpy(pUserData->szNickName, nicknameArray, sizeof(pUserData->szNickName));

	pUserData->cbGender=logonSuccess->cbGender;
	pUserData->wFaceID=logonSuccess->wFaceID;
	pUserData->dwUserID=logonSuccess->dwUserID;
	pUserData->lScore=logonSuccess->f_yue;
	pUserData->lScore=logonSuccess->lScore;// 棋牌余额
	pUserData->lExperience=logonSuccess->dwExperience;
	g_GlobalUnits.setGolbalUserData(pUserData);
	CCLOG("wFaceID:%d",pUserData->wFaceID);
	CCLOG("login success2! file(%s) line(%d)",__FILE__, __LINE__);

	UserDefault::getInstance()->setStringForKey("NEWZJD_ACCOUNT", m_szAccount);
	UserDefault::getInstance()->setStringForKey("NEWZJD_PASSWORD", m_szPassword);

	if ( pMsgDispatch->isRegLogin())
	{
		CCLOG("register success! file(%s) line(%d)",__FILE__, __LINE__);
		int count = UserDefault::getInstance()->getIntegerForKey("KEY_ACCOUNT_COUNT") + 1;
		UserDefault::getInstance()->setIntegerForKey("KEY_ACCOUNT_COUNT", count);
		pMsgDispatch->setRegLogin(false);
		NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_REGSUCC);
		return true;
	}
	CCLOG("login success! file(%s) line(%d)",__FILE__, __LINE__);
	return true;
}

bool MsgDispatch::OnSocketSubGPLogonFailure(void *buffer, WORD size)
{
	CCLOG("OnSocketSubLogonFailure");
	pMsgDispatch->closeLoginSocket();
	CMD_MB_LogonFailure* pLogonError = (CMD_MB_LogonFailure *)buffer;

	NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_LOGINFAIL);
	return true;
}

bool MsgDispatch::OnSocketSubListKind(void *buffer, WORD size)
{
	CCLOG("列表获取");
	return true;
}

bool MsgDispatch::OnSocketSubListServer(void *buffer, WORD size)
{
	int structSize = sizeof(tagGameServer);
	int count = size/structSize;
	int index = 0;

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

	return true;
}

//列表完成
bool MsgDispatch::OnSocketSubListFinish(void *buffer, WORD size)
{
	CCLOG("OnSocketSubListFinish");

	// 拷贝房间信息，并刷新显示
	NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_REFRESHTOTALNUM, NULL);
	GBEvent::instance()->fire(GBEVENT_UI_LOGINFINISH);
	NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_LOGINFINISH);

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
			pConfigMgr->m_Config.m_nGolbalBankruptcy = pListConfig->nBankruptcy;
			memcpy(pConfigMgr->m_Config.szTitle, pListConfig->szTitle, sizeof(pConfigMgr->m_Config.szTitle));
			memcpy(pConfigMgr->m_Config.szContent, pListConfig->szContent, sizeof(pConfigMgr->m_Config.szContent));
			// 拷贝房间信息，并刷新显示
			memcpy(pConfigMgr->m_Config.nServerEnter, pListConfig->nRoomEnter, sizeof(pListConfig->nRoomEnter));
			NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_REFRESHTOTALNUM, NULL);
			return true;
		}

		// 获取游戏服地址
	case SUB_GP_QUICK_GAMESERVER_ADR:
		{
			pMsgDispatch->closeLoginSocket();

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
				bool ret = pMsgDispatch->connectGameServerAdr(lpValue[0], lpValue[1]);			//0为连接失败 1为连接成功  2为没有服务器			
				NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_GAMELINK,Integer::create(ret?1:0));
			}

			break;
		}
	}

	return true;
}

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
	case SUB_GR_GET_USER_PACKET_RET:				
		return this->OnSocketWithGetUserPacket(pBuffer, wDataSize);
	case SUB_GP_GUESS_COIN_RET:				
		return this->OnSocketWithGuessCoinRet(pBuffer, wDataSize);
	case SUB_GP_TURN_TALBE_RET:				
		return this->OnSocketWithTurnTableRet(pBuffer, wDataSize);
	case SUB_GP_QUERY_GAME_RET:				
		return this->OnSocketWithQueryGameRet(pBuffer, wDataSize);
	case SUB_GP_GET_MORE_RECORD_RET:				
		return this->OnSocketWithQueryMoreGameRet(pBuffer, wDataSize);
	case SUB_GP_GET_USER_FANDIAN_RET:
		return this->OnSocketWithQueryMyFanDianRet(pBuffer, wDataSize);
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
	case SUB_GR_SET_QUKUAN_ZHANGHAO_RET:
		return this->OnSocketWithSetQukuanZhanghaoRet(pBuffer, wDataSize);
	case SUB_GR_QUERY_MY_YINHANG_RET:
		return this->OnSocketWithQueryMyYinHangRet(pBuffer, wDataSize);
	case SUB_GP_QUERY_YINHANG_RET: //910
		return this->OnSocketWithQueryYinHangRet(pBuffer, wDataSize);
	case SUB_GP_SET_USER_BONUS_RET:
		return this->OnSocketWithSetusedBonusRet(pBuffer, wDataSize);
	case SUB_GR_XG_LOGIN_PASS_RET:
	case SUB_GR_XG_QUKUAN_PASS_RET:
		{
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
	case SUB_GP_QUIT_GAME_RET:
		return this->OnSocketWithExit(pBuffer, wDataSize);
	default:
		break;
	}

	return true;
}

bool MsgDispatch::OnSocketWithSearchGoods(void *buffer, WORD size)
{
	BYTE cbBuffer[sizeof(CMD_GR_GoodsInfoRet)];
	memset(cbBuffer, 0 , sizeof(CMD_GR_GoodsInfoRet));
	CMD_GR_GoodsInfoRet *goodsInfo = (CMD_GR_GoodsInfoRet *)cbBuffer;
	memcpy(goodsInfo, buffer, sizeof(CMD_GR_GoodsInfoRet));

	return true;
}

bool MsgDispatch::OnSocketWithBuyGoods(void *buffer, WORD size)
{
	BYTE cbBuffer[sizeof(CMD_GP_UserInsureFailure)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_UserInsureFailure));
	CMD_GP_UserInsureFailure *goodsInfo = (CMD_GP_UserInsureFailure *)cbBuffer;
	memcpy(goodsInfo, buffer, sizeof(CMD_GP_UserInsureFailure));

	MyNSString::wc2mbs(goodsInfo->szDescribeString, sizeof(goodsInfo->szDescribeString), szDescribeString);
	NotificationCenter::getInstance()->postNotification("BuyGoodsInfoRet");
	return true;
}

bool MsgDispatch::OnSocketWithGiveGoods(void *buffer, WORD size)
{
	return true;
}

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

//猜硬币返回
bool MsgDispatch::OnSocketWithGuessCoinRet(void *buffer, WORD size)
{
	BYTE cbBuffer[2048];
	memset(cbBuffer, 0 , 2048);
	CMD_GP_GuessCoinRet *info = (CMD_GP_GuessCoinRet *)cbBuffer;
	memcpy(info, buffer, size);
	Array *data = Array::create();
	data->addObject(CCBool::create(info->bCoinSide));
	data->addObject(Integer::create(info->lScore));
	NotificationCenter::getInstance()->postNotification("GuessCoinRet", data);
	return true;
}

//大转盘返回
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

//游戏开奖查询返回
bool MsgDispatch::OnSocketWithQueryGameRet(void *buffer, WORD size)
{
	int count = size/sizeof(DBO_GP_QueryLotResult);
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	DBO_GP_QueryLotResult *info = (DBO_GP_QueryLotResult *)cbBuffer;
	memcpy(info, buffer, size);

	char	period[300] = {0};						//期
	char	lotNum[300] = {0};						//开奖数据
	char	shijian[300] = {0};
	Array *data = Array::create();
	for(int i = 0 ; i < count ; i++)
	{
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

	NotificationCenter::getInstance()->postNotification("QueryGameRet", data);

	return true;
}

//获取更多开奖结果返回
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

//CQSSC 下注返回
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

//获取投注记录数返回
bool MsgDispatch::OnSocketWithGetTouZhuLogCountRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetTouzhuLogCountRet *info = (CMD_GR_GetTouzhuLogCountRet *)cbBuffer;
	memcpy(info, buffer, size);

	NotificationCenter::getInstance()->postNotification("TouZhuLogCount", Integer::create(info->dwCount));

	return true;
}

//获取投注信息返回
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
		dictionary->setObject(String::createWithFormat("%d-%d-%d %d:%d:%d", (info+i)->n_t_time.wYear,
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

//查询会员信息日志数返回
bool MsgDispatch::OnSocketWithGetHYXXLogCountRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetHYXXLogCountRet *info = (CMD_GR_GetHYXXLogCountRet *)cbBuffer;
	memcpy(info, buffer, size);
	NotificationCenter::getInstance()->postNotification("GetLogCount", Integer::create(info->dwCount));

	return true;
}

//查询会员信息返回
bool MsgDispatch::OnSocketWithGetHYXXLogRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetHYXXLogRet *info = (CMD_GR_GetHYXXLogRet *)cbBuffer;
	memcpy(info, buffer, size);
	char s_t_account[120];
	char s_t_qq[120];
	int count = size/sizeof(CMD_GR_GetXJTZHLogRet);
	Array *data = Array::create();
	for(int i = 0 ; i < count ; i++){
		memset(s_t_account, 0, sizeof(s_t_account));
		memset(s_t_qq, 0, sizeof(s_t_qq));
		Dictionary *dictionary = Dictionary::create();
		dictionary->setObject(Integer::create((info+i)->n_t_id), "n_t_id");
		MyNSString::wc2mbs((info+i)->s_t_account, 30, s_t_account);
		dictionary->setObject(String::createWithFormat("%s", s_t_account), "s_t_account");
		dictionary->setObject(String::createWithFormat("%d-%d-%d %d:%d:%d", (info+i)->n_t_time.wYear,
			(info+i)->n_t_time.wMonth, (info+i)->n_t_time.wDay, (info+i)->n_t_time.wHour, (info+i)->n_t_time.wMinute,
			(info+i)->n_t_time.wSecond), "time");
		dictionary->setObject(Integer::create((info+i)->n_t_type), "n_t_type");
		dictionary->setObject(Double::create((info+i)->f_t_fandian), "f_t_fandian");
		dictionary->setObject(Double::create((info+i)->f_t_yue), "f_t_yue");
		dictionary->setObject(Integer::create((info+i)->n_t_online), "n_t_online");
		MyNSString::wc2mbs((info+i)->s_t_qq, 30, s_t_qq);
		dictionary->setObject(String::createWithFormat("%s", s_t_qq), "s_t_qq");
		data->addObject(dictionary);
	}

	NotificationCenter::getInstance()->postNotification("GetLog", data);
	return true;
}

//获取下级投注统计记录数返回
bool MsgDispatch::OnSocketWithGetXJTZHLogCountRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetXJTZHLogCountRet *info = (CMD_GR_GetXJTZHLogCountRet *)cbBuffer;
	memcpy(info, buffer, size);

	NotificationCenter::getInstance()->postNotification("XJTZHLogCountRet", Integer::create(info->dwCount));

	return true;
}

//获取下级投注统计信息返回
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

//查看下级投注记录数返回
bool MsgDispatch::OnSocketWithCHKXJTZHLogCountRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_CHKXJTZHLogCountRet *info = (CMD_GR_CHKXJTZHLogCountRet *)cbBuffer;
	memcpy(info, buffer, size);

	NotificationCenter::getInstance()->postNotification("GetLogCount", Integer::create(info->dwCount));

	return true;
}

//查看下级投注返回
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
		dictionary->setObject(String::createWithFormat("%d-%d-%d %d:%d:%d", (info+i)->n_t_time.wYear,
			(info+i)->n_t_time.wMonth, (info+i)->n_t_time.wDay, (info+i)->n_t_time.wHour, (info+i)->n_t_time.wMinute,
			(info+i)->n_t_time.wSecond), "n_t_time");
		dictionary->setObject(Integer::create((info+i)->n_t_state), "n_t_state");
		dictionary->setObject(Integer::create((info+i)->n_t_winzhushu), "n_t_winzhushu");
		dictionary->setObject(Double::create((info+i)->f_t_yingkui), "f_t_yingkui");
		dictionary->setObject(Integer::create((info+i)->n_t_zhuihao), "n_t_zhuihao");
		data->addObject(dictionary);
	}
	
	NotificationCenter::getInstance()->postNotification("GetLog", data);
	return true;
}

//查看下级游戏盈亏记录数返回
bool MsgDispatch::OnSocketWithGetXJYXTjCountRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetXJYXTjCountRet *info = (CMD_GR_GetXJYXTjCountRet *)cbBuffer;
	memcpy(info, buffer, size);

	NotificationCenter::getInstance()->postNotification("GetXJYXTjCountRet", Integer::create(info->dwCount));

	return true;
}

//查看下级游戏盈亏返回
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

//查询下级盈亏信息日志数返回
bool MsgDispatch::OnSocketWithGetXJYKLogCountRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetXJYKLogCountRet *info = (CMD_GR_GetXJYKLogCountRet *)cbBuffer;
	memcpy(info, buffer, size);

	NotificationCenter::getInstance()->postNotification("GetXJYKLogCountRet", Integer::create(info->dwCount));

	return true;
}

//查询下级盈亏返回
bool MsgDispatch::OnSocketWithGetXJYKLogRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetXJYKLogRet *info = (CMD_GR_GetXJYKLogRet *)cbBuffer;
	memcpy(info, buffer, size);
	char s_t_account[120];
	char s_t_comment[120];
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
		dictionary->setObject(String::createWithFormat("%d-%d-%d %d:%d:%d", (info+i)->n_t_time.wYear,
			(info+i)->n_t_time.wMonth, (info+i)->n_t_time.wDay, (info+i)->n_t_time.wHour, (info+i)->n_t_time.wMinute,
			(info+i)->n_t_time.wSecond), "n_t_time");
		int type = (info+i)->n_t_type;
		dictionary->setObject(Integer::create((info+i)->n_t_type), "n_t_type");
		MyNSString::wc2mbs((info+i)->s_t_comment, 33, s_t_comment);
		dictionary->setObject(String::createWithFormat("%s", s_t_comment), "s_t_comment");
		data->addObject(dictionary);
	}

	NotificationCenter::getInstance()->postNotification("GetXJYKLogRet", data);
	return true;
}

//查询下级盈亏统计日志数返回
bool MsgDispatch::OnSocketWithGetXJYKTjCountRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetXJYKTjCountRet *info = (CMD_GR_GetXJYKTjCountRet *)cbBuffer;
	memcpy(info, buffer, size);

	NotificationCenter::getInstance()->postNotification("GetXJYKTjCountRet", Integer::create(info->dwCount));

	return true;
}

//查询下级盈亏统计返回
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

		data->addObject(dictionary);
	}

	NotificationCenter::getInstance()->postNotification("GetXJYKTjRet", data);
	return true;
}

//查询下级充值日志数返回
bool MsgDispatch::OnSocketWithGetXjChzhLogCountRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetXjChzhLogCountRet *info = (CMD_GR_GetXjChzhLogCountRet *)cbBuffer;
	memcpy(info, buffer, size);

	NotificationCenter::getInstance()->postNotification("GetLogCount", Integer::create(info->dwCount));

	return true;
}

//查询下级充值返回
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
		dictionary->setObject(String::createWithFormat("%d-%d-%d %d:%d:%d", (info+i)->n_t_time.wYear,
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

//查询下级提现日志数返回
bool MsgDispatch::OnSocketWithGetXjTxLogCountRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetXjTxLogCountRet *info = (CMD_GR_GetXjTxLogCountRet *)cbBuffer;
	memcpy(info, buffer, size);

	NotificationCenter::getInstance()->postNotification("GetXjTxLogCountRet", Integer::create(info->dwCount));

	return true;
}

//查询下级提现返回
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
		dictionary->setObject(String::createWithFormat("%d-%d-%d %d:%d:%d", (info+i)->n_t_time.wYear,
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

//获取提现记录数返回
bool MsgDispatch::OnSocketWithGetTiXianLogCountRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetTixianLogCountRet *info = (CMD_GR_GetTixianLogCountRet *)cbBuffer;
	memcpy(info, buffer, size);

	NotificationCenter::getInstance()->postNotification("GetLogCount", Integer::create(info->dwCount));

	return true;
}

//获取提现信息返回
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
		dictionary->setObject(String::createWithFormat("%d-%d-%d %d:%d:%d", (info+i)->n_t_time.wYear,
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

//获取充值记录数返回
bool MsgDispatch::OnSocketWithGetChongzhiLogCountRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetChongzhiLogCountRet *info = (CMD_GR_GetChongzhiLogCountRet *)cbBuffer;
	memcpy(info, buffer, size);

	NotificationCenter::getInstance()->postNotification("GetLogCount", Integer::create(info->dwCount));

	return true;
}

//获取充值信息返回
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
		dictionary->setObject(String::createWithFormat("%d-%d-%d %d:%d:%d", (info+i)->n_t_time.wYear,
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

//获取盈亏记录数返回
bool MsgDispatch::OnSocketWithGetYingkuiLogCountRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetYingkuiLogCountRet *info = (CMD_GR_GetYingkuiLogCountRet *)cbBuffer;
	memcpy(info, buffer, size);

	NotificationCenter::getInstance()->postNotification("GetLogCount", Integer::create(info->dwCount));

	return true;
}

//获取盈亏信息返回
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

//获取棋牌盈亏记录数返回
bool MsgDispatch::OnSocketWithGetQiPaiYingkuiLogCountRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetQiPaiYingkuiCountRet *info = (CMD_GR_GetQiPaiYingkuiCountRet *)cbBuffer;
	memcpy(info, buffer, size);

	NotificationCenter::getInstance()->postNotification("GetLogCount", Integer::create(info->dwCount));

	return true;
}

//获取棋牌盈亏信息返回
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
		dictionary->setObject(String::createWithFormat("%d-%d-%d %d:%d:%d", (info+i)->n_t_playtime.wYear,
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


// 下注返回
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

//获取投注返回
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
	if (info->n_t_moshi == 1){
		danZhuJine = 0.2;
	}
	else if (info->n_t_moshi == 2){
		danZhuJine = 0.02;
	}
	else if (info->n_t_moshi == 3)
	{
		danZhuJine = 0.002;
	}
	else{
		danZhuJine = 2;
	}

	data->addObject(String::createWithFormat("%.3f", info->n_t_zhushu*info->n_t_beishu*danZhuJine));
	data->addObject(String::createWithFormat("%d", info->n_t_winzhushu));
	data->addObject(String::createWithFormat("%.3lf", info->f_t_danzhujine * danZhuJine/2));
	data->addObject(String::createWithFormat("%.2lf", info->f_t_yingkui));
	data->addObject(String::createWithFormat("%.2lf", info->f_t_fandian));			//index=15 返点
	//index=16: 返点金额

	data->addObject(String::createWithFormat("%.2f", info->n_t_zhushu*info->n_t_beishu* danZhuJine *info->f_t_fandian*0.01f));
	
	data->addObject(String::createWithFormat("%.2lf", info->f_t_yingkui));


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


//获取系统时间返回
bool MsgDispatch::OnSocketWithGetSystemTimeRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetSystemTime *info = (CMD_GR_GetSystemTime *)cbBuffer;
	memcpy(info, buffer, size);

	Dictionary *data = Dictionary::create();
	data->setObject(Integer::create(info->ServerTime.wYear), "wYear");
	data->setObject(Integer::create(info->ServerTime.wMonth), "wMonth");
	data->setObject(Integer::create(info->ServerTime.wDay), "wDay");
	data->setObject(Integer::create(info->ServerTime.wHour), "wHour");
	data->setObject(Integer::create(info->ServerTime.wMinute), "wMinute");
	data->setObject(Integer::create(info->ServerTime.wSecond), "wSecond");
	data->setObject(Integer::create(info->ServerTime.wMilliseconds), "wMilliseconds");

	NotificationCenter::getInstance()->postNotification("GetSystemTimeRet", data);

	current_time_t = SystemTimeToTime_t(info->ServerTime);

	time_t t_local = time(NULL);
	m_nDeltaTime = current_time_t - t_local ;				//获取差值。
	return true;
}


//撤单返回
bool MsgDispatch::OnSocketWithCancelTouZhuRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_CancelTouzhuRet *info = (CMD_GR_CancelTouzhuRet *)cbBuffer;
	memcpy(info, buffer, size);


	NotificationCenter::getInstance()->postNotification("CancelTouZhuRet", Integer::create(info->nResult));

	return true;
}

//充值信息查询返回
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
	Array *data = Array::create();
	for(int i = 0 ; i < count ; i++){
		memset(s_t_yinhang, 0, sizeof(s_t_yinhang));
		memset(s_t_kaihuren, 0, sizeof(s_t_kaihuren));
		memset(s_t_zhanghao, 0, sizeof(s_t_zhanghao));
		memset(s_t_web, 0, sizeof(s_t_web));
		int size = sizeof((info+i)->s_t_yinhang);
		MyNSString::wc2mbs((info+i)->s_t_yinhang, sizeof((info+i)->s_t_yinhang), s_t_yinhang);
		MyNSString::wc2mbs((info+i)->s_t_kaihuren, sizeof((info+i)->s_t_kaihuren), s_t_kaihuren);
		MyNSString::wc2mbs((info+i)->s_t_zhanghao, sizeof((info+i)->s_t_zhanghao), s_t_zhanghao);
		MyNSString::wc2mbs((info+i)->s_t_web, sizeof((info+i)->s_t_web), s_t_web);
		if(strcmp(s_t_yinhang, "") == 0){
			continue;
		}
		Dictionary *dic = Dictionary::create();
		dic->setObject(String::create(s_t_yinhang), "s_t_yinhang");
		dic->setObject(String::create(s_t_kaihuren), "s_t_kaihuren");
		dic->setObject(String::create(s_t_zhanghao), "s_t_zhanghao");
		dic->setObject(String::create(s_t_web), "s_t_web");
		data->addObject(dic);
	}

	NotificationCenter::getInstance()->postNotification("GetChongzhiTypeRet", data);

	return true;
}

//提款信息查询返回
bool MsgDispatch::OnSocketWithGetQukuanInfoRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GP_GetQukuanInfoRet *info = (CMD_GP_GetQukuanInfoRet *)cbBuffer;
	memcpy(info, buffer, size);

	return true;
}

//提款返回
bool MsgDispatch::OnSocketWithDoQukuanRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GP_DoQukuanRet *info = (CMD_GP_DoQukuanRet *)cbBuffer;
	memcpy(info, buffer, size);

	NotificationCenter::getInstance()->postNotification("DoQukuanRet", Integer::create(info->nResult));

	return true;
}

//绑卡返回
bool MsgDispatch::OnSocketWithSetQukuanZhanghaoRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_SetQukuanZhanghao_RET *info = (CMD_GR_SetQukuanZhanghao_RET *)cbBuffer;
	memcpy(info, buffer, size);

	NotificationCenter::getInstance()->postNotification("SetQukuanZhanghaoRet", Integer::create(info->lResult));

	return true;
}

//绑卡查询返回
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

	data->setObject(String::createWithFormat("%d-%d-%d %d:%d:%d", info->t_ksyj_time.wYear,
		info->t_ksyj_time.wMonth, info->t_ksyj_time.wDay, info->t_ksyj_time.wHour,
		info->t_ksyj_time.wMinute,info->t_ksyj_time.wSecond), "t_ksyj_time");

	data->setObject(String::createWithFormat("%d-%d-%d %d:%d:%d", info->t_fh_time.wYear,
		info->t_fh_time.wMonth, info->t_fh_time.wDay, info->t_fh_time.wHour,
		info->t_fh_time.wMinute,info->t_fh_time.wSecond), "t_fh_time");

	data->setObject(String::createWithFormat("%d-%d-%d %d:%d:%d", info->t_yj_time.wYear,
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

bool MsgDispatch::OnSocketWithExit(void *buffer, WORD size)
{
	NotificationCenter::getInstance()->postNotification("ExitReturn");
	return true;
}

//添加下级返回
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
	data->setObject(Integer::create(info->n_t_peie_1), "total30");
	data->setObject(Integer::create(info->n_t_peie_2), "total28");
	data->setObject(Integer::create(info->n_t_peie_3), "total27");
	data->setObject(Integer::create(info->n_t_peie_s_1), "pere30");
	data->setObject(Integer::create(info->n_t_peie_s_2), "pere28");
	data->setObject(Integer::create(info->n_t_peie_s_3), "pere27");
	
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

//查询会员数据返回
bool MsgDispatch::OnSocketWithGetHyShjRet(void *buffer, WORD size)
{
	BYTE cbBuffer[16384];
	memset(cbBuffer, 0 , 16384);
	CMD_GR_GetHyShjRet *info = (CMD_GR_GetHyShjRet *)cbBuffer;
	memcpy(info, buffer, size);

	Dictionary *data = Dictionary::create();
	data->setObject(Integer::create(info->n_t_xjrs), "n_t_xjrs");
	data->setObject(Integer::create(info->n_t_xjzx), "n_t_xjzx");
	data->setObject(Double::create(info->f_t_hyye), "f_t_hyye");
	data->setObject(Double::create(info->f_t_jrfd), "f_t_jrfd");
	data->setObject(Double::create(info->f_t_jrchz), "f_t_jrchz");
	data->setObject(Double::create(info->f_t_jrqk), "f_t_jrqk");//今日取款
	data->setObject(Double::create(info->f_t_jrtzh), "f_t_jrtzh");//今日投注
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

	NotificationCenter::getInstance()->postNotification("GetHyShjRet", data);

	return true;
}

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

//保险柜信息
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

//查询保险柜成功
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
	NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_UPDATESCOREANDGOLDEGG,NULL);
	MyNSString::wc2mbs(userInsureInfo->szDescribeString, sizeof(userInsureInfo->szDescribeString), szDescribeString);
	NotificationCenter::getInstance()->postNotification("UserInsureInfo");
	return true;
}

//查询保险柜失败
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
	CCLOG("%s", str.c_str());

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
	CMD_GP_LogonAccounts *pLogonByAccounts = (CMD_GP_LogonAccounts *)cbBuffer;
	memset(pLogonByAccounts,0,sizeof(CMD_GP_LogonAccounts));

	pLogonByAccounts->dwPlazaVersion = PROCESS_VERSION(29,0,16);//VERSION_MOBILE;

	memset(pLogonByAccounts->szAccounts, 0, sizeof(TCHART)*NAME_LEN);
	memset(pLogonByAccounts->szPassword, 0, sizeof(TCHART)*PASS_LEN);
	memset(pLogonByAccounts->szMachineID, 0, sizeof(TCHART)*LEN_MD5);

	char szMD5[33];
	memset(szMD5,0,sizeof(szMD5));
	CMD5Encrypt::EncryptData((unsigned char*)password.c_str(), szMD5);
	MyNSString::mbs2wc(account.c_str(), NAME_LEN, pLogonByAccounts->szAccounts);
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
	CCLOG("%d", size);

	m_pClientLoginSock->sendData(MDM_MB_LOGON,SUB_MB_REGISITER_ACCOUNTS,accounts,sizeof(CMD_MB_RegisterAccounts));
}

std::string& MsgDispatch::getMacaddress()
{
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

	char szMD5[33];
	CMD5Encrypt::EncryptData((unsigned char*)m_szPassword.c_str(), szMD5);

	//登录数据包
	CMD_GR_LogonByUserID * pLogonByUserID=(CMD_GR_LogonByUserID *)cbBuffer;
	pLogonByUserID->dwUserID		 = pUserData->dwUserID;
	pLogonByUserID->dwPlazaVersion	 = 0;
	pLogonByUserID->dwProcessVersion = 0;
	std::string plogonByPassWord = UserDefault::getInstance()->getStringForKey("NEWZJD_PASSWORD");
	memcpy(pLogonByUserID->szPassWord,szMD5 ,PASS_LEN);

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

// 网络验证
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
	if (m_pClientLoginSock->isConectServer())
	{
		return true;
	}
	CCLOG("connectLoginServer");


	// add  20160712
	bool ret = false;
#if 0
	ret = m_pClientLoginSock->connectGameServer(IP_LOGON, PORT_LOGON);
	if(!ret)
		ret = m_pClientLoginSock->connectGameServer(IP_LOGON2, PORT_LOGON);
	if (!ret)
		ret = m_pClientLoginSock->connectGameServer(IP_LOGON3, PORT_LOGON);
	if(!ret)
		ret = m_pClientLoginSock->connectGameServer(IP_LOGON4, PORT_LOGON);
#else
    float m_padding=MyConfig::Instance().m_padding;
	MyConfig& myConfig=MyConfig::Instance();
	const char *ip1=IP_LOGON;
	if(myConfig.m_ip1!=""){
		ip1=myConfig.m_ip1.c_str();
	}
	const char *ip2=IP_LOGON;
	if(myConfig.m_ip2!=""){
		ip2=myConfig.m_ip2.c_str();
	}
	const char *ip3=IP_LOGON;
	if(myConfig.m_ip3!=""){
		ip3=myConfig.m_ip3.c_str();
	}
	int port=PORT_LOGON;
	if(myConfig.m_port>0){
		port=myConfig.m_port;
	}
	ret = m_pClientLoginSock->connectGameServer(ip1, port);
	if(!ret)
		ret = m_pClientLoginSock->connectGameServer(ip2, port);
	if (!ret)
		ret = m_pClientLoginSock->connectGameServer(ip3, port);
#endif
	if (!ret)
	{
		CCLOG("connect login server faild! file(%s) line(%d)",__FILE__, __LINE__);
		return  false;
	}
	SendPacketWithCompilatio(0);
	return true;
}

//进入相应桌子
bool MsgDispatch::connectGameServer(int nGameKindId)
{
	if (m_pClientGameSock->isConectServer())
	{
				m_pClientGameSock->closeSocket();
	}
	tagGameServer server = m_ServerListMgr.getGameServer(nGameKindId);
	char ip[64];
	MyNSString::wc2mbs(server.szServerAddr, sizeof(server.szServerAddr), ip);
	unsigned short port = server.wServerPort;
	CCLOG("ip=%s,wServerPort=%d",ip,port);
	bool ret = m_pClientGameSock->connectGameServer(ip, port);
	if (!ret)
	{
		CCLOG("connect game server faild! file(%s) line(%d)",__FILE__, __LINE__);
		return  false;
	}
	m_dwKindID = nGameKindId;
	SendPacketWithCompilatio(1);
	return true;
}

//快速登陆房间
bool MsgDispatch::connectGameServer()
{
	if (m_pClientGameSock->isConectServer())
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
	bool ret = m_pClientGameSock->connectGameServer(ip, port);
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

//登陆房间
void MsgDispatch::SendPacketWithEnterRoomPageCount(WORD count)
{
	// 房间登录
	BYTE cbBuffer[sizeof(CMD_GR_LogonMobile)];
	memset(cbBuffer, 0 , sizeof(CMD_GR_LogonMobile));
	CMD_GR_LogonMobile *lm = (CMD_GR_LogonMobile *)cbBuffer;

	lm->wGameID			= m_dwKindID;					// 游戏标识
	lm->dwProcessVersion = VERSION_FRAME;                 // 手机版本
	lm->wVisibleMode	= m_wBehaviorFlags;				// 行为标识
	lm->wVisibleTable	= count;						// 页数
	lm->dwUserID			= m_dwUserID;					// 用户id

	memcpy(lm->szPassword, m_pPassWordMD5, sizeof(TCHART)*LEN_MD5);
	CCLOG("sizeof(CMD_GR_LogonMobile)=%d,m_dwUserID=%d", sizeof(CMD_GR_LogonMobile),m_dwUserID);
	CCLOG("wBehaviorFlags=wVisibleMode=%d,wPageTableCount=wVisibleTable=%d",lm->wVisibleMode,lm->wVisibleTable);
	m_pClientGameSock->sendData(MDM_GR_LOGON, SUB_GR_LOGON_MOBILE, (void*)lm, sizeof(CMD_GR_LogonMobile));
	// 消息状态
	m_eDataType = CMD_Room;
	setLoginState(enGame_Server);
	NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_GAMELINK,Integer::create(1));
}

//连接游戏服务器
bool MsgDispatch::connectGameServerAdr(DWORD dwAdrIP, WORD wPort)
{
	in_addr addr;
	addr.s_addr = dwAdrIP;
	char* ip = inet_ntoa(addr);

	bool ret = m_pClientGameSock->connectGameServer(ip, wPort);

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
	//UserLevel::iterator iter = m_UserLevelList.begin();
	//for (; iter != m_UserLevelList.end(); iter++)
	//{
	//	if ((*iter).lInfoType == type)
	//	{
	//		return  (*iter).tagUserLeve[nLevel].lUserLevel;
	//	}
	//}
	return 0;
}

void MsgDispatch::SendPacketWithGameOption()
{
	BYTE cbBuffer[sizeof(CMD_GF_GameOption)];
	memset(cbBuffer, 0 , sizeof(CMD_GF_GameOption));
	CMD_GF_GameOption *gameOption = (CMD_GF_GameOption *)cbBuffer;
	gameOption->cbAllowLookon=false;
	gameOption->dwFrameVersion=INVALID_DWORD;
	gameOption->dwClientVersion=INVALID_DWORD;

	m_pClientGameSock->sendData(MDM_GF_FRAME, SUB_GF_GAME_OPTION, gameOption, sizeof(CMD_GF_GameOption));
}

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
}

void MsgDispatch::SendPacketWithRequestMyTableUserInfo(int playerNum)
{
	for(WORD chairID = 0 ; chairID < playerNum ; chairID++)
	{
		this->SendPacketWithRequestChairUserInfo(m_wTableID, chairID);
	}
}

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
	EntityMgr::instance()->roomFrame()->setGameIsStart(true);
}

void MsgDispatch::SendPacketWithQueryBankInfo()
{
	BYTE cbBuffer[sizeof(CMD_GP_QueryInsureInfo)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_QueryInsureInfo));
	CMD_GP_QueryInsureInfo *info = (CMD_GP_QueryInsureInfo *)cbBuffer;

	info->dwUserID=m_dwUserID;
	memcpy(info->szPassword, m_pPassWordMD5, sizeof(CMD_GP_QueryInsureInfo)-sizeof(info->dwUserID));

	if (!m_pClientLoginSock->isConectServer())
	{
		this->connectLoginServer();
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

	if (!m_pClientLoginSock->isConectServer())
	{
		this->connectLoginServer();
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

	if (!m_pClientLoginSock->isConectServer())
	{
		this->connectLoginServer();
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

	if (!m_pClientLoginSock->isConectServer())
	{
		this->connectLoginServer();
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

	if (!m_pClientGameSock->isConectServer())
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

	if (!m_pClientLoginSock->isConectServer())
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

	if (!m_pClientLoginSock->isConectServer())
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

	if (!m_pClientLoginSock->isConectServer())
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

	if (!m_pClientLoginSock->isConectServer())
	{
		this->connectLoginServer();
	}

	m_pClientLoginSock->sendData(MDM_GP_USER_SERVICE, SUB_GP_TURN_TABLE, info, sizeof(CMD_GP_TurnTable));
}

void MsgDispatch::SendPacketWithPerformStandup(WORD wTableID, WORD wChairID, BYTE cbForceLeave)
{
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

	if (!m_pClientLoginSock->isConectServer())
	{
		this->connectLoginServer();
	}
	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_QUERY_GAME_RESULT, info, sizeof(CMD_GP_QueryGameResult));
}

//查询单个彩种的开奖记录
void MsgDispatch::SendPacketQueryMoreGameResult(int kindID)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetMoreRecord)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetMoreRecord));
	CMD_GP_GetMoreRecord *info = (CMD_GP_GetMoreRecord *)cbBuffer;

	info->n_t_type = kindID;

	if (!m_pClientLoginSock->isConectServer())
	{
		this->connectLoginServer();
	}
	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_MORE_RECORD, info, sizeof(CMD_GP_GetMoreRecord));
}

//投注
void MsgDispatch::SendPacketWithTouZhuCQSSC(string qiShu, int gameType, int gameKind, string betNumber, int zhuShu,
											int beiTou, int moshi, int sign, int zhuiHao, int nIndex)
{
	//注数
	if (betNumber.length() <= 15360)
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
	
		time_t t = time(0);
		char timeStr[64] = {0};
		strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S",localtime(&t) );
		DBHandler *handler = DBHandler::sharedDBHandler();
		char sql[24000]={0};
		std::sprintf(sql, "insert into TouZhuXXInfo(time,dwUserID,gameType,gameKind,qiShu,betNumber,zhuShu,beiTou,moshi,sign,isEnd,zhuiHao,nIndex)values('%s',%d,%d,%d,'%s','%s',%d,%d,%d,%d,%d,%d,%d)",
			timeStr,m_dwUserID,gameType,gameKind,qiShu.c_str(),betNumber.c_str(),zhuShu,beiTou,moshi,sign,0,zhuiHao,nIndex);
		handler->execute(sql);
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

void MsgDispatch::SendPacketWithGetTouZhuLogCount(bool bTime, const char *szTimeStart, const char *szTimeEnd)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetTouzhuLogCount)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetTouzhuLogCount));
	CMD_GP_GetTouzhuLogCount *info = (CMD_GP_GetTouzhuLogCount *)cbBuffer;
	info->dwUserID = m_dwUserID;
	info->bTime = bTime;
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_TOUZHU_LOG_COUNT, info, sizeof(CMD_GP_GetTouzhuLogCount));
}

void MsgDispatch::SendPacketWithGetTouZhuLog(int nPage, int nSize, bool bTime, const char *szTimeStart, const char *szTimeEnd)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetTouzhuLog)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetTouzhuLog));
	CMD_GP_GetTouzhuLog *info = (CMD_GP_GetTouzhuLog *)cbBuffer;
	info->dwUserID = m_dwUserID;
	info->nPage = nPage;
	info->nSize = nSize;
	info->bByTime = bTime;
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
	info->bTime = bTime;
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);
	info->n_t_type = n_t_type;
	info->n_t_user_id = n_t_user_id;
	MyNSString::mbs2wc(s_t_account, strlen(s_t_account), info->s_t_account);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_XJTZH_LOG_COUNT, info, sizeof(CMD_GP_GetXJTZHLogCount));
}

//获取下级投注统计
void MsgDispatch::SendPacketWithGetXJTZHLog(int nPage, int nSize, bool bTime, const char *szTimeStart, const char *szTimeEnd, int n_t_type, int n_t_user_id, const char *s_t_account)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetXJTZHLog)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetXJTZHLog));
	CMD_GP_GetXJTZHLog *info = (CMD_GP_GetXJTZHLog *)cbBuffer;
	info->dwUserID = m_dwUserID;
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
void MsgDispatch::SendPacketWithCHKXJTZHLogCount(bool bTime, const char *szTimeStart, const char *szTimeEnd, int n_t_type, int n_t_user_id, const char *s_t_account)
{
	BYTE cbBuffer[sizeof(CMD_GP_CHKXJTZHLogCount)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_CHKXJTZHLogCount));
	CMD_GP_CHKXJTZHLogCount *info = (CMD_GP_CHKXJTZHLogCount *)cbBuffer;
	info->dwUserID = m_dwUserID;
	info->bTime = bTime;
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);
	info->n_t_type = n_t_type;
	info->n_t_user_id = n_t_user_id;
	MyNSString::mbs2wc(s_t_account, strlen(s_t_account), info->s_t_account);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_CHK_XJTZH_LOG_COUNT, info, sizeof(CMD_GP_CHKXJTZHLogCount));
}

//查看下级投注
void MsgDispatch::SendPacketWithCHKXJTZHLog(int nPage, int nSize, bool bTime, const char *szTimeStart, const char *szTimeEnd, int n_t_type, int n_t_user_id, const char *s_t_account)
{
	BYTE cbBuffer[sizeof(CMD_GP_CHKXJTZHLog)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_CHKXJTZHLog));
	CMD_GP_CHKXJTZHLog *info = (CMD_GP_CHKXJTZHLog *)cbBuffer;
	info->dwUserID = m_dwUserID;
	info->nPage = nPage;
	info->nSize = nSize;
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
void MsgDispatch::SendPacketWithGetXJYKLogCount(bool bTime, const char *szTimeStart, const char *szTimeEnd, int n_t_type, int n_t_user_id, const char *s_t_account)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetXJYKLogCount)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetXJYKLogCount));
	CMD_GP_GetXJYKLogCount *info = (CMD_GP_GetXJYKLogCount *)cbBuffer;
	info->dwUserID = m_dwUserID;
	info->bTime = bTime;
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);
	info->n_t_type = n_t_type;
	info->n_t_user_id = n_t_user_id;
	MyNSString::mbs2wc(s_t_account, strlen(s_t_account), info->s_t_account);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_XJYK_LOG_COUNT, info, sizeof(CMD_GP_GetXJYKLogCount));
}

//查询下级盈亏明细
void MsgDispatch::SendPacketWithGetXJYKLog(int nPage, int nSize, bool bTime, const char *szTimeStart, const char *szTimeEnd, int n_t_type, int n_t_user_id, const char *s_t_account)
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

//查询下级盈亏统计
void MsgDispatch::SendPacketWithGetXJYKTj(int nPage, int nSize, bool bTime, const char *szTimeStart, const char *szTimeEnd, int n_t_type, int n_t_user_id, const char *s_t_account)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetXJYKTj)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetXJYKTj));
	CMD_GP_GetXJYKTj *info = (CMD_GP_GetXJYKTj *)cbBuffer;
	info->dwUserID = m_dwUserID;
	info->nPage = nPage;
	info->nSize = nSize;
	info->bByTime = bTime;
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
void MsgDispatch::SendPacketWithGetQiPaiYingkuiCount(DWORD userID, int nTypeID, int nByTime, const char *szTimeStart, const char *szTimeEnd)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetQiPaiYingkuiCount)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetQiPaiYingkuiCount));
	CMD_GP_GetQiPaiYingkuiCount *info = (CMD_GP_GetQiPaiYingkuiCount *)cbBuffer;
	//info->dwUserID = userID;
	info->nTypeID = nTypeID;
	info->nByTime = nByTime;
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_QP_YINGKUI_COUNT, info, sizeof(CMD_GP_GetQiPaiYingkuiCount));
}

//查询下级游戏记录
void MsgDispatch::SendPacketWithGetQiPaiYingkui(DWORD userID, int nPage, int nSize, bool bTime, const char *szTimeStart, const char *szTimeEnd)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetQiPaiYingkui)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetQiPaiYingkui));
	CMD_GP_GetQiPaiYingkui *info = (CMD_GP_GetQiPaiYingkui *)cbBuffer;
	//info->dwUserID = userID;
	info->nPage = nPage;
	info->nSize = nSize;
	info->bByTime = bTime;
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

void MsgDispatch::SendPacketWithGetQiPaiYingkuiLogCount(int nTypeID, int nByTime, const char *szTimeStart, const char *szTimeEnd)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetQiPaiYingkuiCount)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetQiPaiYingkuiCount));
	CMD_GP_GetQiPaiYingkuiCount *info = (CMD_GP_GetQiPaiYingkuiCount *)cbBuffer;
	//info->dwUserID = m_dwUserID;
	info->nTypeID = nTypeID;
	info->nByTime = nByTime;
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_QP_YINGKUI_COUNT, info, sizeof(CMD_GP_GetQiPaiYingkuiCount));
}	

void MsgDispatch::SendPacketWithGetQiPaiYingkuiLog(int nPage, int nSize, bool bTime, const char *szTimeStart, const char *szTimeEnd)
{
	BYTE cbBuffer[sizeof(CMD_GP_GetQiPaiYingkui)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_GetQiPaiYingkui));
	CMD_GP_GetQiPaiYingkui *info = (CMD_GP_GetQiPaiYingkui *)cbBuffer;
	//info->dwUserID = m_dwUserID;
	info->nPage = nPage;
	info->nSize = nSize;
	info->bByTime = bTime;
	MyNSString::mbs2wc(szTimeStart, strlen(szTimeStart), info->szTimeStart);
	MyNSString::mbs2wc(szTimeEnd, strlen(szTimeEnd), info->szTimeEnd);

	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_GET_QP_YINGKUI, info, sizeof(CMD_GP_GetQiPaiYingkui));
}

void  MsgDispatch::SendPacketWithTouZhuCQSSC_ZhioHao(int zhuiHaoQiShu, char qiShu[50][20],
													 int gameType, int gameKind, string betNumber, int zhuShu, int beiTou[50], int moshi, int sign, int isEnd, int zhuiHao, int nIndex)
{
	BYTE cbBuffer[sizeof(CMD_GP_TouZhuCQSSC_Zhuihao)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_TouZhuCQSSC_Zhuihao));
	CMD_GP_TouZhuCQSSC_Zhuihao *info = (CMD_GP_TouZhuCQSSC_Zhuihao *)cbBuffer;

	info->dwUserID = m_dwUserID;
	for(int i = 0 ; i < 50 ; i++){
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

void MsgDispatch::SendPacketWithGetSystemTime()
{
	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_QUERY_TIME);
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

void MsgDispatch::SendReConnect()
{
	//重新连接
	std::string userAccount = UserDefault::getInstance()->getStringForKey("NEWZJD_ACCOUNT");
	std::string userPassword = UserDefault::getInstance()->getStringForKey("NEWZJD_PASSWORD");

	bool bRet = pMsgDispatch->connectLoginServer();
	if (!bRet)
	{		
		//懒得提示了
		return;
	}
	else
	{
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

//棋牌彩票互转
void MsgDispatch::SendPacketWithZhuanHuanMoney(int n_t_type, DOUBLE f_t_jine)
{
	BYTE cbBuffer[sizeof(CMD_GP_ZhuanHuan)];
	memset(cbBuffer, 0 , sizeof(CMD_GP_ZhuanHuan));
	CMD_GP_ZhuanHuan *info = (CMD_GP_ZhuanHuan *)cbBuffer;
	info->n_t_userid = m_dwUserID;
	info->n_t_type = n_t_type;
	info->f_t_jine = f_t_jine;
	m_pClientLoginSock->sendData(MDM_GR_USER, SUB_GP_ZHUANHUAN	, info, sizeof(CMD_GP_ZhuanHuan));

}

void MsgDispatch::onExitProgram(Node* obj)
{
	Director::getInstance()->end();
}
