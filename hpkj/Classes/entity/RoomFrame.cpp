#include "RoomFrame.h"
#include "EntityMgr.h"
#include "GlobalUnits.h"
#include "GlobalFrame.h"
#include "GlobalDef.h"
#include "GlobalField.h"
#include "utf-8.h"
#include "CMD_GameServer.h"
#include "CMD_LogonServer.h"
#include "MyNSString.h"
#include "DBHandler.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../JniHelper.h"
#endif

RoomFrame::RoomFrame()
	:m_pMeUserItem(NULL)
{
	GBEVENT_ARG_REGISTER(GBEVENT_NETWORK_ROOM, RoomFrame::onRoomMsg);
	GBEVENT_ARG_REGISTER(GBEVENT_NETWORK_GAME, RoomFrame::onGameMsg);

	m_nTable = INVALID_CHAIR;
	m_nChair = INVALID_TABLE;
	m_cbStatus = US_NULL;
	gameIsStart = false;
}

RoomFrame::~RoomFrame()
{
	GBEVENT_ARG_UNREGISTER(GBEVENT_NETWORK_ROOM, RoomFrame::onRoomMsg);
	GBEVENT_ARG_UNREGISTER(GBEVENT_NETWORK_GAME, RoomFrame::onGameMsg);
}

void RoomFrame::reset()
{
	UsrDateMap::iterator iter =  m_UserDateList.begin();
	for (; iter != m_UserDateList.end();  iter++)
	{
		delete iter->second;
	}

	m_UserDateList.clear();
	m_pMeUserItem = NULL;
}

//发送游戏服务器登陆包
void RoomFrame::sendLoginPacket()
{
	pMsgDispatch->sendLoginPacket();
}

void RoomFrame::sendQueuePacket(int cbQueueCell)
{
	pMsgDispatch->sendQueuePacket(cbQueueCell);
}

void RoomFrame::onRoomMsg(GBEventConstArg& arg)
{
	tagCmdBuffer& tag = (tagCmdBuffer&)arg;
	CMD_Command* pCommand = &tag.command;
	OnEventTcpRoomRead(pCommand,tag.dateBuffer,tag.nDataSize);
}

//游戏消息处理
void RoomFrame::onGameMsg(GBEventConstArg& arg)
{
	tagCmdBuffer& tag = (tagCmdBuffer&)arg;
	CMD_Command* pCommand = &tag.command;
	OnEventTcpGameRead(pCommand,tag.dateBuffer,tag.nDataSize);
}

//把原来的游戏消息分成房间登陆消息和游戏消息			
bool  RoomFrame::OnEventTcpGameRead(CMD_Command* pCommand,void* pData,WORD wDataSize)
{

	switch (pCommand->wMainCmdID)
	{
	case MDM_GR_STATUS:			//状态信息
		{
			//CCLOG("状态消息! file(%s) line(%d)",__FILE__, __LINE__);
			return OnSocketGameStatus(pCommand,pData,wDataSize);
		}
	case MDM_GR_SYSTEM:			//系统消息
		{
			//CCLOG("系统消息! file(%s) line(%d)",__FILE__, __LINE__);
			return OnSocketGameSystem(pCommand,pData,wDataSize);
		}
	case MDM_GR_SERVER_INFO:	//房间信息
		{
			//CCLOG("房间消息! file(%s) line(%d)",__FILE__, __LINE__);
			return OnSocketGameServerInfo(pCommand,pData,wDataSize);
		}
	case MDM_GF_GAME:			//游戏消息
		{
			//CCLOG("游戏消息! file(%s) line(%d)",__FILE__, __LINE__);

			return OnGameMessage(pCommand->wSubCmdID,pData,wDataSize);
		}
	case MDM_GF_FRAME:			//框架消息
		{
			//CCLOG("框架消息! file(%s) line(%d)",__FILE__, __LINE__);
			return OnFrameMessage(pCommand->wSubCmdID,pData,wDataSize);
		}
	}
	return  true;
}

bool  RoomFrame::OnEventTcpRoomRead(CMD_Command* pCommand,void* pData,WORD wDataSize)
{
	switch (pCommand->wMainCmdID)
	{
	case MDM_GR_LOGON:			//登录消息				1
		{
			//CCLOG("登陆消息! file(%s) line(%d)",__FILE__, __LINE__);
			return OnSocketGameLogin(pCommand,pData,wDataSize);
		}
	case MDM_GR_CONFIG:			//配置信息				2
		{
			//CCLOG("配置消息! file(%s) line(%d)",__FILE__, __LINE__);
			return OnSocketGameInfo(pCommand,pData,wDataSize);
		}
	case MDM_GR_USER:			//用户消息				3
		{
			//CCLOG("用户消息! file(%s) line(%d)",__FILE__, __LINE__);
			return OnSocketGameUser(pCommand,pData,wDataSize);
		}
	case MDM_GR_STATUS:			//配置信息				4
		{
			//CCLOG("状态消息! file(%s) line(%d)",__FILE__, __LINE__);
			return OnSocketGameStatus(pCommand,pData,wDataSize);
		}
	case MDM_GF_GAME:			//游戏命令				200
		{
			//CCLOG("游戏消息! file(%s) line(%d)",__FILE__, __LINE__);

			return OnGameMessage(pCommand->wSubCmdID,pData,wDataSize);
		}
	case MDM_GF_FRAME:			//框架命令				100
		{
			//CCLOG("框架消息! file(%s) line(%d)",__FILE__, __LINE__);
			return OnFrameMessage(pCommand->wSubCmdID,pData,wDataSize);
		}
	case MDM_GR_INSURE:			//保险柜命令			5
		{
			return true;
		}
	case MDM_GR_MANAGE:			//管理命令				6
		{
			//CCLOG("房间消息! file(%s) line(%d)",__FILE__, __LINE__);
			return OnSocketGameServerInfo(pCommand,pData,wDataSize);
			return true;
		}
	case MDM_CM_SYSTEM:			//系统命令				1000
		{
			//CCLOG("系统消息! file(%s) line(%d)",__FILE__, __LINE__);
			return OnSocketGameSystem(pCommand,pData,wDataSize);
		}
	case MDM_KN_COMMAND:			//内核命令			0
		{
			return true;
		}
	}
	return  true;
}


//登录消息
bool RoomFrame::OnSocketGameLogin(CMD_Command* pCommand, void * pBuffer, WORD wDataSize)
{
	switch (pCommand->wSubCmdID)
	{
	case SUB_GR_LOGON_SUCCESS: //登陆成功
		{
			this->OnSocketSubGRLogonSuccess(pBuffer, wDataSize);
			return true;
		}
	case SUB_GR_LOGON_FAILURE:   //登陆失败
		{
			this->OnSocketSubGRLogonFailure(pBuffer, wDataSize);
			return true;
		}
	case SUB_GR_LOGON_FINISH:  //登陆完成
		{
			CCLOG("----------------------gameserver login success!-------------------------- file(%s) line(%d)",__FILE__, __LINE__);
			this->OnSocketSubGRLogonFinish(pBuffer, wDataSize);
			return true;
		}
	}
	return  true;
}

// 房间登录子命令码处理       
//登录成功       
bool RoomFrame::OnSocketSubGRLogonSuccess(void *buffer, WORD size)
{
	CCLOG("OnSocketSubGRLogonSuccess！！！！！");
	//NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_GAMELINK,Integer::create(1));
	NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_LOGINFAIL,Integer::create(2));
	return true;
}

//登录失败       
bool RoomFrame::OnSocketSubGRLogonFailure(void *buffer, WORD size)
{
	pMsgDispatch->closeGameSocket();
	CCLOG("game server login failed! file(%s) line(%d)",__FILE__, __LINE__);
	// 变量定义
	CMD_GR_LogonFailure lf;
	memset(&lf, 0, sizeof(CMD_GR_LogonFailure));
	memcpy(&lf, buffer, sizeof(CMD_GR_LogonFailure));

	//NSLog(@"OnSocketSubGRLogonFailure lErrorCode=%ld", lf.lErrorCode);

	if (lf.lErrorCode == ERROR_CODE_LOGIN)
	{
		//if ([CFrameDate shareFrameDate].m_bChinaPayIsValid)
		if (1)
		{
			//if (alertBuy == nil)
			{
				// 错误提示
				char str[500];
				MyNSString::wc2mbs(lf.szDescribeString, sizeof(lf.szDescribeString), str);
				tagBankruptObj tag;
				tag.bBankrupt = false;
				NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_BANKRUPT,&tag);
				CCLOG(str);
			}
		}
		else
		{
			//if (alertBuy == nil)
			{
				char str[500];
				MyNSString::wc2mbs(lf.szDescribeString, sizeof(lf.szDescribeString), str);
				CCLOG(str);
			}
		}
	}
	else
	{
		// 错误提示
		char str[500];
		MyNSString::wc2mbs(lf.szDescribeString, sizeof(lf.szDescribeString), str);
		CCLOG(str);
	}

	CCLOG("登录失败");
	return true;
}

//登录完成       
bool RoomFrame::OnSocketSubGRLogonFinish(void *buffer, WORD size)
{
	CCLOG("登录完成");
	NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_GBEVENT_UI_ROOMLOGINFINISH,NULL);
	bool m_bStartType = pMsgDispatch->getStartType();
	BYTE m_cbStatus=pMsgDispatch->m_cbStatus;
	// 空闲状态启动房间
	if ( (m_cbStatus == US_FREE || 
		m_cbStatus == US_NULL || 
		m_cbStatus == US_SIT) && !m_bStartType)
	{
		pMsgDispatch->SendPacketWithChangeTable();
		//if ([m_pPlaza respondsToSelector:@selector(FrameDateDelegateWithOnRoomEnter)])
		//[m_pPlaza FrameDateDelegateWithOnRoomEnter];
		return true;
	}

	// 游戏状态启动游戏
	if (m_cbStatus == US_PLAYING || m_cbStatus == US_READY)
	{
		pMsgDispatch->SendPacketWithGameOption();

		m_bStartType = false;

		CCLOG("断线重连");
		return true;
	}
	return true;
}

//提示更新       
bool RoomFrame::OnSocketSubGRUpdateNotify(void *buffer, WORD size)
{
	return true;
}

//大厅登陆成功提示       
bool RoomFrame::OnSocketSubGRLogonNotify(void *buffer, WORD size)
{
	return true;
}

//用户消息
bool RoomFrame::OnSocketGameUser(CMD_Command* pCommand, void* pBuffer, WORD wDataSize)
{
	switch (pCommand->wSubCmdID)
	{
	case SUB_GR_USER_RULE:				// 用户规则	1
		{
			CCLOG("未处理的case");
			return true;
		}
	case SUB_GR_USER_LOOKON:			// 用户旁观	2
		{
			CCLOG("未处理的case");
			return true; 
		}
	case SUB_GR_USER_SITDOWN:			// 用户坐下	3	
		{
			CCLOG("未处理的case");
			return true;
		}
	case SUB_GR_USER_STANDUP:			// 用户起立	4
		{
			CCLOG("未处理的case");
			return true;
		}
	case SUB_GR_USER_INVITE:			// 用户邀请	5
		{
			CCLOG("用户邀请未处理");
			return true;
		}
	case SUB_GR_USER_INVITE_REQ:		// 邀请请求	6
		{
			CCLOG("邀请请求未处理的");
			return true;
		}
	case SUB_GR_USER_REPULSE_SIT:		// 拒绝坐下	7
		{
			CCLOG("拒绝坐下未处理7");
			return true;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////
	case SUB_GR_USER_ENTER:				// 用户进入	100
		{
			return OnSocketSubUserCome(pCommand,pBuffer,wDataSize);
		}
	case SUB_GR_USER_SCORE:				// 用户分数	101
		{
			return OnSocketSubScore(pCommand,pBuffer,wDataSize);
		}
	case SUB_GR_USER_STATUS:			// 用户状态	102
		{
			return OnSocketSubStatus(pCommand,pBuffer,wDataSize);
		}
	case SUB_GR_REQUEST_FAILURE:		// 请求失败	103
		{
			return OnSocketSubSitFailed(pCommand,pBuffer,wDataSize);
		}
	case SUB_GR_USER_CHAT:				// 用户聊天	201
		{
			return OnSocketSubChat(pCommand,pBuffer,wDataSize);
		}
	case SUB_GR_USER_EXPRESSION:		// 用户表情	202
		{
			CCLOG("未处理的case");
			return true;
		}
	case SUB_GR_WISPER_CHAT:			// 用户私聊	203
		{
			CCLOG("未处理的case");
			return true;
		}
	case SUB_GR_WISPER_EXPRESSION:		// 私聊表情	204
		{
			CCLOG("未处理的case");
			return true;
		}
	case SUB_GR_COLLOQUY_CHAT:			//会话消息	205
		{
			CCLOG("未处理的case");
			return true;
		}
	case SUB_GR_COLLOQUY_ERPRESSION:	// 会话表情	206
		{
			CCLOG("未处理的case");
			return true;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////
	case SUB_GR_PROPERTY_BUY:			// 购买道具	300
		{
			CCLOG("道具购买未处理");
			return true;
		}
	case SUB_GR_PROPERTY_SUCCESS:		// 道具成功	301
		{
			//return [self OnSocketPropBuySuccess:wSubCmdID buffer:buffer bufferSize:size];
		}
	case SUB_GR_PROPERTY_FAILURE:		// 道具失败	302		
		{
			//return [self OnSocketPropBuyFailure:wSubCmdID buffer:buffer bufferSize:size];
		}
	case SUB_GR_PROPERTY_MESSAGE:		// 道具消息	303 道具消息好象暂时没有什么用途
		{
			return true;
		}

		//case SUB_GP_USER_INSURE_INFO:           // 保险柜资料 403
		//case SUB_GP_USER_INSURE_SUCCESS:        // 保险柜成功 405
		//case SUB_GP_USER_INSURE_FAILURE:        // 保险柜失败 406
		{
			//return [self OnSocketRecvBankInfo:wSubCmdID buffer:buffer bufferSize:size];
		}

		//////////////////////////////////////////////////////////////////////////////////////////////
	case SUB_GR_GET_BONUS:              // 服务器端返回结果，以及豆豆金额 405      // add by mojunxin
		{
			//return [self OnSocketRecvSilverBonus:buffer bufferSize:size];
		}
	case SUB_GR_GET_ERROR:                  // 点击宝箱返回错误消息描述 1405
		{
			//return [self OnSocketRecvSilverBonusError:buffer bufferSize:size];
		}


		//////////////////////////////////////////////////////////////////////////////////////////////
//	case SUB_GP_PAY_REVE_IP:             // apple应用内购买完成，向服务器申请充值，服务器返回的结果  601      // add by mojunxin
	//	{
//			//return [self OnSocketGetIapResult:buffer bufferSize:size];
//		}

		//////////////////////////////////////////////////////////////////////////////////////////////
	default:
		{
			CCLOG("未处理的case OnTCPNetworkMainGRUser wSubCmdID=%d", pCommand->wSubCmdID);
			return true;
		}
	}
	return true;
}
//配置信息
bool RoomFrame::OnSocketGameInfo(CMD_Command* pCommand, void * pBuffer, WORD wDataSize)
{
	switch (pCommand->wSubCmdID)
	{
	case SUB_GR_CONFIG_COLUMN:			// 列表配置
		{
			return this->OnSocketSubGRConfigColumn(pBuffer, wDataSize);
		}
	case SUB_GR_CONFIG_SERVER:			// 房间配置
		{
			return this->OnSocketSubGRConfigServer(pBuffer, wDataSize);
		}
	case SUB_GR_CONFIG_PROPERTY:		// 道具配置
		{
			return this->OnSocketSubGRConfigPropepty(pBuffer, wDataSize);
		}
	case SUB_GR_CONFIG_FINISH:			// 配置完成
		{
			return this->OnSocketSubGRConfigFinish(pBuffer, wDataSize);
		}
	default:
		//CCAssert(false, "未处理的case");
		return true;
	}
	return true;
}

// 配置信息子命令码处理       
//列表配置       
bool RoomFrame::OnSocketSubGRConfigColumn(void *buffer, WORD size)
{
	CCLOG("列表配置");
	return true;
}

//房间配置       
bool RoomFrame::OnSocketSubGRConfigServer(void *buffer, WORD size)
{
	CCLOG("房间配置");
	// 效验大小
	if (size != sizeof(CMD_GR_ConfigServer)) return false;

	return true;
}

//道具配置       
bool RoomFrame::OnSocketSubGRConfigPropepty(void *buffer, WORD size)
{
	CCLOG("道具配置");
	return true;
}

//配置完成       
bool RoomFrame::OnSocketSubGRConfigFinish(void *buffer, WORD size)
{
	CCLOG("配置完成");
	return true;
}


//参赛用户进入
bool RoomFrame::OnSocketSubMatchUserCome(CMD_Command* pCommand, void * pBuffer, WORD wDataSize)
{
	return true;
}
//状态信息
bool RoomFrame::OnSocketGameStatus(CMD_Command* pCommand, void * pBuffer, WORD wDataSize)
{
	CCLOG("获取用户状态");
	return true;
}
//系统消息
bool RoomFrame::OnSocketGameSystem(CMD_Command* pCommand, void * pBuffer, WORD wDataSize)
{
	//效验参数
	CMD_GF_Message* pMessage=(CMD_GF_Message *)pBuffer;
	if (wDataSize<=(sizeof(CMD_GF_Message)-sizeof(pMessage->szContent)))
	{
		return false;
	}
	//消息处理
	WORD wHeadSize= sizeof(CMD_GF_Message) - sizeof(pMessage->szContent);
	if (wDataSize != (wHeadSize + pMessage->wMessageLength * sizeof(char)))
	{
		return false;
	}
	pMessage->szContent[pMessage->wMessageLength-1]=0;

	if (pMessage->wMessageType & SMT_GLOBAL)
	{
		NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_MATCH);
		NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_MATCHDZPK);
	}

	return true;
}
//房间信息
bool RoomFrame::OnSocketGameServerInfo(CMD_Command* pCommand, void * pBuffer, WORD wDataSize)
{
	return true;
}

//用户信息 用户进入

bool RoomFrame::OnSocketSubUserCome(CMD_Command* pCommand, void* pBuffer, WORD wDataSize)
{
	string str = "用户进入";
	MyNSString::GBKToUTF8(str);
	CCLOG(str.c_str());
	// 变量定义
	BYTE cbBuffer[sizeof(tagGameInUserInfo)];
	memset(cbBuffer, 0 , sizeof(tagGameInUserInfo));
	tagGameInUserInfo *pUserInfoHead = (tagGameInUserInfo *)cbBuffer;
	memcpy(pUserInfoHead, pBuffer, sizeof(tagGameInUserInfo));
	//CCLOG("用户进入! file(%s) line(%d)",__FILE__, __LINE__);
	BYTE bBuffer[sizeof(tagDataDescribe)];
	memset(bBuffer, 0 , sizeof(tagDataDescribe));
	tagDataDescribe *describe= (tagDataDescribe *)bBuffer;
	memcpy(describe, (BYTE *)pBuffer+sizeof(tagGameInUserInfo), sizeof(tagDataDescribe));

	//读取基本信息
	tagUserData* pUserData = new tagUserData;
	memset(pUserData, 0, sizeof(tagUserData));
	memset(pUserData, 0, sizeof(tagUserData));
	// 用户属性
	if (describe->wDataDecribe == DTP_GR_NICK_NAME){
		//memcpy(pUserData->szNickName, ((BYTE *)pBuffer+sizeof(tagGameInUserInfo)+sizeof(tagDataDescribe)), describe->wDataSize);
		MyNSString::wc2mbs((TCHART *)(((BYTE *)pBuffer+sizeof(tagGameInUserInfo)+sizeof(tagDataDescribe))), describe->wDataSize, pUserData->szNickName);
	}
	//pUserData->dwGameID=pUserInfoHead->dwGameID;
	pUserData->dwUserID=pUserInfoHead->dwUserID;
	// 头像信息
	pUserData->wFaceID=pUserInfoHead->wFaceID;
	// 用户属性
	pUserData->cbGender=pUserInfoHead->cbGender;
	pUserData->cbMemberOrder=pUserInfoHead->cbMemberOrder;
	// 用户状态
	pUserData->wTableID=pUserInfoHead->wTableID;
	pUserData->wChairID=pUserInfoHead->wChairID;
	pUserData->cbUserStatus=pUserInfoHead->cbUserStatus;
	// 积分信息
	pUserData->lScore=pUserInfoHead->lScore;
	// 游戏信息
	pUserData->lWinCount=pUserInfoHead->dwWinCount;
	pUserData->lLostCount=pUserInfoHead->dwLostCount;
	pUserData->lDrawCount=pUserInfoHead->dwDrawCount;
	pUserData->lFleeCount=pUserInfoHead->dwFleeCount;
	pUserData->lExperience=pUserInfoHead->dwExperience;
	pUserData->lGoldEggs = 0;
	string dwellPlace = "广东省深圳市";
	MyNSString::GBKToUTF8(dwellPlace);
	memcpy(pUserData->szDwellingPlace, dwellPlace.c_str(), sizeof(pUserData->szDwellingPlace));

	if (pUserInfoHead->cbUserStatus == US_NULL)
	{
		removeItem(pUserInfoHead->dwUserID);
	}else{
		CUserItem* pUserItem = (CUserItem*)insertItem(pUserData);
	}

	if (pUserInfoHead->dwUserID == pMsgDispatch->m_dwUserID)
	{
		pMsgDispatch->m_cbStatus = pUserInfoHead->cbUserStatus;
		//m_cbStatus = pUserInfoHead->cbUserStatus;
		pMsgDispatch->m_wChairID = pUserInfoHead->wChairID;
		pMsgDispatch->m_wTableID = pUserInfoHead->wTableID;
		m_pMeUserItem = new CUserItem(pUserData);
	}

	// 更新桌子视图
	if (pUserInfoHead->wTableID != INVALID_TABLE && pUserInfoHead->wChairID != INVALID_CHAIR && 
		pMsgDispatch->m_wTableID == pUserInfoHead->wTableID) 
	{
		if(pUserInfoHead->cbUserStatus>=US_SIT)
		{
			updateMeTable(pUserInfoHead->dwUserID);
		}
	}
	return true;
}

//1房间列表保存用户原来状态

//用户状态
bool RoomFrame::OnSocketSubStatus(CMD_Command* pCommand, void * pBuffer, WORD wDataSize)
{
	CCLOG("－－－－－－－－－－－－－－－－－－－－用户状态!－－－－－－－－－－－－－－－－－－－－－－－－－");

	if (wDataSize!=sizeof(CMD_GR_UserStatus))
	{
		return false;
	}

	//处理数据
	BYTE cbBuffer[sizeof(CMD_GR_UserStatus)];
	memset(cbBuffer, 0 , sizeof(CMD_GR_UserStatus));
	CMD_GR_UserStatus *pUserStatus = (CMD_GR_UserStatus *)cbBuffer;
	memcpy(pUserStatus, pBuffer, sizeof(CMD_GR_UserStatus));

	CUserItem* pIUserItem = (CUserItem*)getUserItem(pUserStatus->dwUserID);
	if(pIUserItem == NULL && pUserStatus->UserStatus.cbUserStatus != US_NULL)
	{
		//读取基本信息
		tagUserData* pUserData = new tagUserData;
		memset(pUserData, 0, sizeof(tagUserData));
		memset(pUserData, 0, sizeof(tagUserData));
		string nicknameString = "获取用户名失败";
		MyNSString::GBKToUTF8(nicknameString);
		memcpy(pUserData->szNickName, nicknameString.c_str(), sizeof(pUserData->szNickName));
		string dwellPlace = "广东省深圳市";
		MyNSString::GBKToUTF8(dwellPlace);
		memcpy(pUserData->szDwellingPlace, dwellPlace.c_str(), sizeof(pUserData->szDwellingPlace));
		pUserData->dwUserID=pUserStatus->dwUserID;
		pUserData->cbUserStatus = pUserStatus->UserStatus.cbUserStatus;
		pUserData->wTableID = pUserStatus->UserStatus.wTableID;
		pUserData->wChairID = pUserStatus->UserStatus.wChairID;
		// 积分信息
		pUserData->lScore=0;
		pUserData->lExperience=0;
		pUserData->lGoldEggs = 0;
		pIUserItem = (CUserItem*)insertItem(pUserData);
		pMsgDispatch->SendPacketWithRequestChairUserInfo(pUserStatus->UserStatus.wTableID, pUserStatus->UserStatus.wChairID);
	}

	if (pIUserItem != NULL) 
	{
		//现在状态
		WORD wNowTableID=pUserStatus->UserStatus.wTableID;
		WORD wNowChairID=pUserStatus->UserStatus.wChairID;
		BYTE cbNowStatus=pUserStatus->UserStatus.cbUserStatus;

		//原来状态
		tagUserData* pUserData=pIUserItem->getUserData();
		WORD wLastTableID=pUserData->wTableID;
		WORD wLastChairID=pUserData->wChairID;
		BYTE cbLastStatus=pUserData->cbUserStatus;


		tagUserStatus status;
		status.wTableID = pUserStatus->UserStatus.wTableID;
		status.wChairID = pUserStatus->UserStatus.wChairID;
		status.cbUserStatus = pUserStatus->UserStatus.cbUserStatus;

		if (m_pMeUserItem==NULL)
		{
			CCLOG("RoomFrame::OnSocketSubStatus m_pMeUserItem==NULL");
			return true;
		}
		tagUserData* pMeUserData =  m_pMeUserItem->getUserData();

		switch (pUserStatus->UserStatus.cbUserStatus) 
		{
		case US_NULL : 
			{
				// 删除用户
				removeItem(pUserStatus->dwUserID);
				// 如果是自己 退出游戏  -------退出到大厅-------
				if (pMeUserData->dwUserID == pUserStatus->dwUserID)
				{
					pMsgDispatch->m_cbStatus = pUserStatus->UserStatus.cbUserStatus;
					m_cbStatus = pUserStatus->UserStatus.cbUserStatus;
					pMsgDispatch->m_wChairID = pUserStatus->UserStatus.wChairID;
					pMsgDispatch->m_wTableID = pUserStatus->UserStatus.wTableID;
					pUserData->wTableID = wNowTableID;
					pUserData->wChairID = wNowChairID;
					pUserData->cbUserStatus = cbNowStatus;
					m_pMeUserItem->setUserDate(pUserData);
				} 
				break;
			}
		case US_FREE : 
			{
				// 更新用户
				updateUserStatus(pUserStatus->dwUserID,&status);
				// 如果是自己判断是否需显示房间
				//if (pMeUserData->dwUserID == pUserStatus->dwUserID) {
					pMsgDispatch->m_cbStatus = pUserStatus->UserStatus.cbUserStatus;
					m_cbStatus = pUserStatus->UserStatus.cbUserStatus;
					pMsgDispatch->m_wChairID = pUserStatus->UserStatus.wChairID;
					pMsgDispatch->m_wTableID = pUserStatus->UserStatus.wTableID;
					pUserData->wTableID = wNowTableID;
					pUserData->wChairID = wNowChairID;
					pUserData->cbUserStatus = cbNowStatus;
					m_pMeUserItem->setUserDate(pUserData);
				//}
				break;
			}
		case US_LOOKON :
		case US_OFFLINE :
		case US_PLAYING :
		case US_READY :
		case US_SIT : {
			// 更新用户
			updateUserStatus(pUserStatus->dwUserID,&status);

			if (pMeUserData->dwUserID == pUserStatus->dwUserID) 
			{
				pMsgDispatch->m_cbStatus = pUserStatus->UserStatus.cbUserStatus;
				m_cbStatus = pUserStatus->UserStatus.cbUserStatus;
				pMsgDispatch->m_wChairID = pUserStatus->UserStatus.wChairID;
				pMsgDispatch->m_wTableID = pUserStatus->UserStatus.wTableID;
				pUserData->wTableID = wNowTableID;
				pUserData->wChairID = wNowChairID;
				pUserData->cbUserStatus = cbNowStatus;
				m_pMeUserItem->setUserDate(pUserData);
				//发送椅子用户信息
				pMsgDispatch->SendPacketWithRequestChairUserInfo(pUserStatus->UserStatus.wTableID, pUserStatus->UserStatus.wChairID);

				// 坐下判断
				if (cbLastStatus < US_SIT)
				{
					pMsgDispatch->SendPacketWithGameOption();
					// 命令码 MDM_GF_FRAME 与 MDM_MB_LOGON 冲突，这里先取负解决
					NotificationCenter::getInstance()->postNotification("GameStart", NULL);
				}
			} else
			{ 
				//发送椅子用户信息
				pMsgDispatch->SendPacketWithRequestChairUserInfo(pUserStatus->UserStatus.wTableID, pUserStatus->UserStatus.wChairID);
				if (pUserStatus->UserStatus.cbUserStatus == US_SIT) 
				{
					if (pMeUserData->wTableID != INVALID_TABLE && wNowTableID == pMeUserData->wTableID && wLastTableID != pMeUserData->wTableID) 
					{

					}
				}
			}

			// 更新桌子视图
			if (wNowTableID != INVALID_TABLE && wNowChairID != INVALID_CHAIR && pMeUserData->wTableID == wNowTableID) 
			{
				updateMeTable(pUserStatus->dwUserID);
			}
			break;
					  }
		default :
			return true;
		}
	}
	return true;
}
UsrIdList RoomFrame::searchUser(WORD wTableID)
{
	UsrIdList userList;
	UsrDateMap::iterator iter =  m_UserDateList.begin();
	for (; iter != m_UserDateList.end(); iter++)
	{
		CUserItem* pUserItem = (CUserItem*)iter->second;
		if (!pUserItem)
		{
			continue;
		}
		tagUserData* pUserDate = pUserItem->getUserData();
		if (!pUserDate)
		{
			continue;
		}
		if (pUserDate->wTableID == wTableID)
		{
			userList.push_back(pUserDate->dwUserID);
		}
	}
	return userList;
}


UsrDateList RoomFrame::searchUserDate(WORD wTableID)
{
	UsrDateList userList;
	UsrDateMap::iterator iter =  m_UserDateList.begin();
	for (; iter != m_UserDateList.end(); iter++)
	{
		CUserItem* pUserItem = (CUserItem*)iter->second;
		if (!pUserItem)
		{
			continue;
		}
		tagUserData* pUserDate = pUserItem->getUserData();
		if (!pUserDate)
		{
			continue;
		}
		if (pUserDate->wTableID == wTableID)
		{
			userList.push_back(pUserDate);
		}
	}
	return userList;
}


//用户分数
bool RoomFrame::OnSocketSubScore(CMD_Command* pCommand, void * pBuffer, WORD wDataSize)
{
	//处理数据
	CMD_GR_MobileUserScore* pUserScore=(CMD_GR_MobileUserScore *)pBuffer;

	CUserItem* pIUserItem = (CUserItem*)getUserItem(pUserScore->dwUserID);
	if (pIUserItem==NULL)
	{
		return true;
	}

	tagUserData* pUserData = pIUserItem->getUserData();
	if (pUserData == NULL)
	{
		return  true;
	}

	tagMobileUserScore userScore = pUserScore->UserScore;
	if (pUserData->dwUserID ==  m_pMeUserItem->getUserData()->dwUserID)
	{
		m_pMeUserItem->setUserScore(&userScore);
		g_GlobalUnits.getGolbalUserData()->lScore    = userScore.lScore;
		//g_GlobalUnits.getGolbalUserData()->lGoldEggs = userScore.lGoldEggs;
		CCLOG("score=%d",userScore.lScore);

		//小于500 提示破产
		if (userScore.lScore < 500)
		{

		}
	}

	pIUserItem->setUserScore(&userScore);

	//CCLOG("======================%d",pUserScore->UserScore.lScore);			分数改变--刷新桌子
	tagUserBuffer tag;
	tag.dwUserID = pUserScore->dwUserID;
	NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_PLAYERUPDATE,&tag);
	return true;
}
//用户名次
bool RoomFrame::OnSocketSubSort(CMD_Command* pCommand, void * pBuffer, WORD wDataSize)
{
	return true;
	CCLOG("user sort! file(%s) line(%d)",__FILE__, __LINE__);
}
//用户权限
bool RoomFrame::OnSocketSubRight(CMD_Command* pCommand, void * pBuffer, WORD wDataSize)
{
	CCLOG("user right! file(%s) line(%d)",__FILE__, __LINE__);
	return true;
}
//会员等级
bool RoomFrame::OnSocketSubMemberOrder(CMD_Command* pCommand, void * pBuffer, WORD wDataSize)
{
	CCLOG("user member order! file(%s) line(%d)",__FILE__, __LINE__);
	return true;
}
//坐下失败
bool RoomFrame::OnSocketSubSitFailed(CMD_Command* pCommand, void * pBuffer, WORD wDataSize)
{
	CCLOG("user sit failed! file(%s) line(%d)",__FILE__, __LINE__);
	tagBankruptObj tag;
	tag.bBankrupt = true;
	NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_BANKRUPT,&tag);
	return true;
}
//用户聊天
bool RoomFrame::OnSocketSubChat(CMD_Command* pCommand, void * pBuffer, WORD wDataSize)
{
	CCLOG("user chat! file(%s) line(%d)",__FILE__, __LINE__);
	return true;
}
//用户私语
bool RoomFrame::OnSocketSubWisper(CMD_Command* pCommand, void * pBuffer, WORD wDataSize)
{
	CCLOG("user wisper! file(%s) line(%d)",__FILE__, __LINE__);
	return true;
}
//邀请玩家
bool RoomFrame::OnSocketSubUserInvite(CMD_Command* pCommand, void * pBuffer, WORD wDataSize)
{
	CCLOG("user invite! file(%s) line(%d)",__FILE__, __LINE__);
	return true;
}
//参加比赛
bool RoomFrame::OnSocketJoinMatch(CMD_Command* pCommand, void * pBuffer, WORD wDataSize)
{
	CCLOG("user join match! file(%s) line(%d)",__FILE__, __LINE__);
	return true;
}
//修改用户金币
bool RoomFrame::OnSocketSubGold(CMD_Command* pCommand, void * pBuffer, WORD wDataSize)
{
	CCLOG("change user gold! file(%s) line(%d)",__FILE__, __LINE__);
	return true;
}
//游戏比赛信息
bool RoomFrame::OnSocketGameMatchInfo(CMD_Command* pCommand, void * pBuffer, WORD wDataSize)
{
	CCLOG("get match info! file(%s) line(%d)",__FILE__, __LINE__);
	return true;
}
//VIP房间密码信息
bool RoomFrame::OnSocketVIPRoomPassWord(CMD_Command* pCommand, void * pBuffer, WORD wDataSize)
{
	CCLOG("VIP room password! file(%s) line(%d)",__FILE__, __LINE__);
	return true;
}
//vip房间密码错误
bool RoomFrame::OnSocketVIPErrorPassWord(CMD_Command* pCommand, void * pBuffer, WORD wDataSize)
{
	CCLOG("vip error password! file(%s) line(%d)",__FILE__, __LINE__);
	return true;
}

//用户比赛成绩
bool RoomFrame::OnSocketSubMatchScore(CMD_Command* pCommand, void * pBuffer, WORD wDataSize)
{
	CCLOG("user match score result! file(%s) line(%d)",__FILE__, __LINE__);
	return true;
}

bool RoomFrame::OnSocketSubModifyNickName(CMD_Command* pCommand, void * pBuffer, WORD wDataSize)
{
	CCLOG("Modyfu nickname result! file(%s) line(%d)",__FILE__, __LINE__);
	return true;
}

bool RoomFrame::OnSocketGameOrder(void * pBuffer, WORD wDataSize)
{
	assert(wDataSize==sizeof(CMD_GP_ScoreGiftResult));
	if(wDataSize!=sizeof(CMD_GP_ScoreGiftResult)) return false;
	CMD_GP_ScoreGiftResult *pGiftResult = (CMD_GP_ScoreGiftResult *)pBuffer;
	CCLOG("ordwe ID = %d, errCode = %d",pGiftResult->lScore,pGiftResult->lErrCode);
	if(pGiftResult->lErrCode == 0)
	{
		int orderID = pGiftResult->lScore;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		toSendAndroidOrder(orderID);
#endif
	}
	else
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		toSendAndroidOrder(0);
#endif
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//网络消息
bool RoomFrame::OnFrameMessage(WORD wSubCmdID, const void* pBuffer, WORD wDataSize)
{
	CCLOG("OnFrameMessage");
	switch (wSubCmdID)
	{
	case SUB_GF_GAME_OPTION:			// 游戏配置     1
		{
			CCLOG("游戏配置");
			break;
		}
	case SUB_GF_USER_READY:				// 用户准备     2
		{
			CCLOG("用户准备");
			return true;
		}
	case SUB_GF_LOOKON_CONFIG:			//旁观配置      3
		{
			CCLOG("旁观配置");
			return true;
		}
	case SUB_GF_USER_CHAT:				//用户聊天      10
		{
			BYTE cbBuffer1[sizeof(tagGameBuffer)];
			memset(cbBuffer1, 0 , sizeof(tagGameBuffer));
			tagGameBuffer *tag = (tagGameBuffer *)cbBuffer1;
			
			tag->nDataSize = wDataSize;
			tag->wSubCmdID = wSubCmdID;
			memcpy(tag->dateBuffer, pBuffer,wDataSize);
			NotificationCenter::getInstance()->postNotification(GBEVENT_UI_GAMEMESSAGE, tag);
			break;
		}
	case SUB_GF_USER_EXPRESSION:		//用户表情      11
		{
			BYTE cbBuffer[sizeof(tagGameBuffer)];
			memset(cbBuffer, 0 , sizeof(tagGameBuffer));
			tagGameBuffer *tag = (tagGameBuffer *)cbBuffer;
			tag->nDataSize = wDataSize;
			tag->wSubCmdID = wSubCmdID;
			memcpy(tag->dateBuffer, pBuffer,wDataSize);
			NotificationCenter::getInstance()->postNotification(GBEVENT_UI_GAMEMESSAGE, tag);
			break;
		}
	case SUB_GF_GAME_STATUS:			//游戏状态      100
		{
			//效验参数
			if (wDataSize!=sizeof(CMD_GF_Option))
			{
				return false;
			}

			//消息处理
			CMD_GF_Option * pOption=(CMD_GF_Option *)pBuffer;
			m_cbGameStatus = pOption->bGameStatus;
			m_cbAllowLookon = pOption->bAllowLookon;

			CCLOG("gamestatus!");
			return true;
		}
	case SUB_GF_GAME_SCENE:				//游戏场景      101
		{
			BYTE cbBuffer[sizeof(tagSceneBuffer)];
			memset(cbBuffer, 0 , sizeof(tagSceneBuffer));
			tagSceneBuffer *tag = (tagSceneBuffer *)cbBuffer;

			tag->nDataSize = wDataSize;
			tag->cbState = m_cbGameStatus;
			memcpy(tag->dateBuffer, pBuffer,wDataSize);
			NotificationCenter::getInstance()->postNotification(GBEVENT_UI_SCENEMESSAGE, tag);
			NotificationCenter::getInstance()->postNotification(GBEVENT_UI_SCENEMESSAGEDZPK, tag);  //dzpk-jhy
			if(gameIsStart == false){
				
			}
			break;
		}
	case SUB_GF_SYSTEM_MESSAGE:			//系统消息      200
		{
			//效验参数
			CMD_CM_SystemMessage* pMessage=(CMD_CM_SystemMessage *)pBuffer;
			if (wDataSize<=(sizeof(CMD_CM_SystemMessage)-sizeof(pMessage->szString)))
			{
				return false;
			}

			pMessage->szString[pMessage->wLength-1]=0;
			char message[1224];
			time_t tt = time(NULL);//这句返回的只是一个时间cuo
			tm* t= localtime(&tt);
			char msg[1024] = {0};
			MyNSString::wc2mbs(pMessage->szString, pMessage->wLength, msg);
			std::sprintf(message, "insert into SystemMsg(msg) values(\'%s     %d-%02d-%02d %02d:%02d:%02d\')", msg,
				t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
			DBHandler::sharedDBHandler()->execute(message);

			tagGameBuffer tag;
			memset(tag.dateBuffer, 0, 4096);

			tag.nDataSize = wDataSize;
			tag.wSubCmdID = wSubCmdID;
			memcpy(tag.dateBuffer, pBuffer,wDataSize);
			NotificationCenter::getInstance()->postNotification(GBEVENT_UI_GAMEMESSAGE,&tag);
		}
	}
	return true;
}


bool RoomFrame::OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	BYTE cbBuffer[sizeof(tagGameBuffer)];
	memset(cbBuffer, 0 , sizeof(tagGameBuffer));
	tagGameBuffer *tag = (tagGameBuffer *)cbBuffer;

	tag->nDataSize = wDataSize;
	tag->wSubCmdID = wSubCmdID;
	memcpy(tag->dateBuffer, pBuffer,wDataSize);
	NotificationCenter::getInstance()->postNotification(GBEVENT_UI_GAMEMESSAGE, tag);
	NotificationCenter::getInstance()->postNotification(GBEVENT_UI_GAMEMESSAGEDZPK, tag); //dzpk-jhy

	return true;
}

//客户端开始游戏
void RoomFrame::OnClientStartGame()
{
	pMsgDispatch->sendSocketDate(MDM_GF_GAME,SUB_C_ALL_READY);
}

//开始客户端游戏
void RoomFrame::startClientGame()
{
	CCLOG("start game!");

	//获取服务器场景消息
	BYTE cbBuffer[sizeof(CMD_GF_Info)];
	memset(cbBuffer, 0 , sizeof(CMD_GF_Info));
	CMD_GF_Info *Info = (CMD_GF_Info *)cbBuffer;
	Info->bAllowLookon= false;

	//发送消息
	pMsgDispatch->sendSocketDate(MDM_GF_FRAME, SUB_GF_INFO,Info,sizeof(Info));

	CCLOG("-----------------------------------------------------get game scene message!---------------------------------------------------");
}
IUserItem* RoomFrame::insertItem(tagUserData* tag)
{
	if (!tag)
	{
		assert(false);
		return NULL;
	}
	CUserItem* pUserItem = new CUserItem(tag);

	DBHandler *handler = new DBHandler();
	char sql[400];
	std::sprintf(sql, "insert into UserInfo(dwUserID,wFaceID,cbGender,cbMemberOrder,lScore,dwWinCount,dwLostCount,dwDrawCount,dwFleeCount,dwExperience,nickname) select %lu,0,0,0,0,0,0,0,0,0,'' where not exists(select * from UserInfo where dwUserID=%lu)", tag->dwUserID, tag->dwUserID);
	handler->execute(sql);
	memset(sql, 0, sizeof(sql));
	std::sprintf(sql, "update UserInfo set dwUserID = %lu,wFaceID = %d,cbGender=%d,cbMemberOrder=%d,lScore=%ld,dwWinCount=%ld,dwLostCount=%ld,dwDrawCount=%ld,dwFleeCount=%ld,dwExperience=%ld,nickname=\'%s\' where dwUserID=%lu",
		tag->dwUserID, tag->wFaceID, tag->cbGender, tag->cbMemberOrder, tag->lScore, tag->lWinCount, tag->lLostCount, tag->lDrawCount, tag->lFleeCount, tag->lExperience, tag->szNickName, tag->dwUserID);
	handler->execute(sql);
	//handler->execute("insert into UserInfo(dwUserID,wFaceID,cbGender,cbMemberOrder,lScore,dwWinCount,dwLostCount,dwDrawCount,dwFleeCount,dwExperience,nickname) values(10010,0,0,0,0,0,0,0,0,0, 'test009') where not exists(select * from UserInfo where dwUserID=10010)");
	//handler->execute("update UserInfo set nickname='test008' where dwUserID=10010 where exists(select * from UserInfo where dwUserID=10010)");
	UsrDateMap::iterator iter =  m_UserDateList.find(pUserItem->getUserID());
	if (iter != m_UserDateList.end())
	{
		CCLOG("user data insert failed! file(%s) line(%d)",__FILE__, __LINE__);
		m_UserDateList.erase(iter);
		//m_UserDateList[tag->dwUserID] = pUserItem;
		//return NULL;
	}
	m_UserDateList[tag->dwUserID] = pUserItem;
	return pUserItem;
}

IUserItem* RoomFrame::insertItem(IUserItem* item)
{
	return item;
}

void RoomFrame::removeItem(DWORD dwUserID)
{
	UsrDateMap::iterator iter =  m_UserDateList.find(dwUserID);
	if (iter != m_UserDateList.end())
	{
		m_UserDateList.erase(iter);
	}
}

IUserItem* RoomFrame::getUserItem(DWORD dwUserID)
{
	UsrDateMap::iterator iter =  m_UserDateList.find(dwUserID);
	if (iter != m_UserDateList.end())
	{
		return iter->second;
	}
	return NULL;
}

void RoomFrame::removeItemByTable(unsigned int nTableID,unsigned int nChairID)
{

}

IUserItem* RoomFrame::getUserItemByChairID(WORD wChairID)
{
	UsrDateMap::iterator iter =  m_UserDateList.begin();
	for(;iter != m_UserDateList.end();iter++)
	{
		IUserItem *pIUserItem=iter->second;
		if(!pIUserItem)
			continue;
		tagUserData* pUserData=pIUserItem->getUserData();
		if(pUserData->wChairID==wChairID)
			return pIUserItem;
	}
	return NULL;
}

void RoomFrame::updateUserStatus(DWORD dwUserID, const tagUserStatus* pUserStatus)
{
	CUserItem* pUserItem = NULL;
	UsrDateMap::iterator iter =  m_UserDateList.find(dwUserID);
	if (iter != m_UserDateList.end())
	{
		((CUserItem*)iter->second)->setUserStatus(pUserStatus);
	}

}

void RoomFrame::updateTableUserStatus(unsigned int nTableID, unsigned int nChairID,IUserItem* pUserItem)
{

}

//更新自己桌子
void RoomFrame::updateMeTable(DWORD dwUserID)
{
	if (!pMsgDispatch->isGameStatuc())
	{
		return;
	}

	//发送状态玩家id
	tagUserBuffer tag;
	tag.dwUserID = dwUserID;
	NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_PLAYERUPDATE,&tag);
}
