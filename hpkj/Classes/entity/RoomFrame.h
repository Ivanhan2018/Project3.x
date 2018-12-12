//
//  RoomFrame.h
//  房间类
//
//  Created by zhouwei on 13-6-21.
//
//

#ifndef __Game__RoomFrame__
#define __Game__RoomFrame__

#include <iostream>
#include "GBEvent.h"
#include "GBEventIDs.h"
#include "BaseObject.h"
#include "Define.h"
#include "GlobalDef.h"
#include "GlobalUnits.h"
#include "UserItem.h"
#include "cocos2d.h"
#include <map>

typedef std::vector<DWORD> UsrIdList;
typedef std::vector<tagUserData*> UsrDateList;

#define ERROR_CODE_LOGIN        (101)
#define	US_NULL				0x00					// 没有状态
#define US_FREE				0x01					// 站立状态
#define US_SIT				0x02					// 坐下状态
#define US_READY			0x03					// 同意状态
#define US_LOOKON			0x04					// 旁观状态
#define US_PLAYING			0x05					// 游戏状态
#define US_OFFLINE			0x06					// 断线状态

class RoomFrame : public BaseObject,Object
{
public:
	RoomFrame();
	~RoomFrame();
	void reset();
public:
	//把原来的游戏消息分成房间登陆消息和游戏消息
	bool  OnEventTcpRoomRead(CMD_Command* pCommand,void* pData,WORD wDataSize);
	bool  OnEventTcpGameRead(CMD_Command* pCommand,void* pData,WORD wDataSize);	
public:
	void onRoomMsg(GBEventConstArg& arg);
	//游戏消息处理
	void onGameMsg(GBEventConstArg& arg);

public:
	//发送游戏服务器登陆包
	void sendLoginPacket();

	//发送排队请求
	void sendQueuePacket(int cbQueueCell = 0);
public:
	/******************************游戏服务器消息*******************************************/
	//登录消息
	bool			OnSocketGameLogin(CMD_Command* pCommand, void * pBuffer, WORD wDataSize);
	//用户消息
	bool			OnSocketGameUser(CMD_Command* pCommand, void * pBuffer, WORD wDataSize);
	//配置信息
	bool			OnSocketGameInfo(CMD_Command* pCommand, void * pBuffer, WORD wDataSize);
	//状态信息
	bool			OnSocketGameStatus(CMD_Command* pCommand, void * pBuffer, WORD wDataSize);
	//系统消息
	bool            OnSocketGameSystem(CMD_Command* pCommand, void * pBuffer, WORD wDataSize);
	//房间信息
	bool            OnSocketGameServerInfo(CMD_Command* pCommand, void * pBuffer, WORD wDataSize);


	/******************************游戏服务器用户消息*******************************************/
	//用户进入
	bool OnSocketSubUserCome(CMD_Command* pCommand, void * pBuffer, WORD wDataSize);
	//参赛用户进入
	bool OnSocketSubMatchUserCome(CMD_Command* pCommand, void * pBuffer, WORD wDataSize);
	//用户状态
	bool OnSocketSubStatus(CMD_Command* pCommand, void * pBuffer, WORD wDataSize);
	//用户分数
	bool OnSocketSubScore(CMD_Command* pCommand, void * pBuffer, WORD wDataSize);
	//用户名次
	bool OnSocketSubSort(CMD_Command* pCommand, void * pBuffer, WORD wDataSize);
	//用户权限
	bool OnSocketSubRight(CMD_Command* pCommand, void * pBuffer, WORD wDataSize);
	//会员等级
	bool OnSocketSubMemberOrder(CMD_Command* pCommand, void * pBuffer, WORD wDataSize);
	//坐下失败
	bool OnSocketSubSitFailed(CMD_Command* pCommand, void * pBuffer, WORD wDataSize);
	//用户聊天
	bool OnSocketSubChat(CMD_Command* pCommand, void * pBuffer, WORD wDataSize);
	//用户私语
	bool OnSocketSubWisper(CMD_Command* pCommand, void * pBuffer, WORD wDataSize);
	//邀请玩家
	bool OnSocketSubUserInvite(CMD_Command* pCommand, void * pBuffer, WORD wDataSize);
	//参加比赛
	bool OnSocketJoinMatch(CMD_Command* pCommand, void * pBuffer, WORD wDataSize);
	//修改用户金币
	bool OnSocketSubGold(CMD_Command* pCommand, void * pBuffer, WORD wDataSize);
	//修改昵称
	bool OnSocketSubModifyNickName(CMD_Command* pCommand, void * pBuffer, WORD wDataSize);
	//游戏比赛信息
	bool OnSocketGameMatchInfo(CMD_Command* pCommand, void * pBuffer, WORD wDataSize);
	//vip房间密码信息
	bool OnSocketVIPRoomPassWord(CMD_Command* pCommand, void * pBuffer, WORD wDataSize);
	//vip房间密码错误
	bool OnSocketVIPErrorPassWord(CMD_Command* pCommand, void * pBuffer, WORD wDataSize);
	//用户比赛成绩
	bool OnSocketSubMatchScore(CMD_Command* pCommand, void * pBuffer, WORD wDataSize);
	/****************************************************************************************/

	//第三方充值订单返回
	bool			OnSocketGameOrder(void * pBuffer, WORD wDataSize);

	bool OnFrameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize);
	bool OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize);

	//客户端开始游戏
	void OnClientStartGame();

	//开始客户端游戏
	void startClientGame();
public:
	//获取用户自己信息
	CUserItem* getMeUserItem(){ return  m_pMeUserItem;}
	tagUserData* getMeUserDate() { &(m_pMeUserItem->m_UserData); }
public:
	IUserItem* insertItem(tagUserData* tag);
	IUserItem* insertItem(IUserItem* item);

	void removeItem(DWORD dwUserID);
	IUserItem* getUserItem(DWORD dwUserID);

	//从桌子移除
	void removeItemByTable(unsigned int nTableID,unsigned int nChairID);

	IUserItem* getUserItemByChairID(WORD wChairID);

	//更新房间列表状态
	void updateUserStatus(DWORD dwUserID, const tagUserStatus* pUserStatus);

	//更新用户桌子状态
	void updateTableUserStatus(unsigned int nTableID,unsigned int nChairID,IUserItem* pUserItem);

	//更新自己桌子
	void updateMeTable(DWORD nUserID);
	UsrIdList searchUser(WORD wTableID);
	UsrDateList searchUserDate(WORD wTableID);

	// 房间登录子命令码处理       
	//登录成功
	bool OnSocketSubGRLogonSuccess(void *buffer, WORD size);			
	//登录失败
	bool OnSocketSubGRLogonFailure(void *buffer, WORD size);		
	//登录完成
	bool OnSocketSubGRLogonFinish(void *buffer, WORD size);	
	//提示更新
	bool OnSocketSubGRUpdateNotify(void *buffer, WORD size);	
	//大厅登陆成功提示
	bool OnSocketSubGRLogonNotify(void *buffer, WORD size);

	// 配置信息子命令码处理       
	//列表配置
	bool OnSocketSubGRConfigColumn(void *buffer, WORD size);			
	//房间配置
	bool OnSocketSubGRConfigServer(void *buffer, WORD size);		
	//道具配置
	bool OnSocketSubGRConfigPropepty(void *buffer, WORD size);		
	//配置完成
	bool OnSocketSubGRConfigFinish(void *buffer, WORD size);
	
	void setGameIsStart(bool flag){gameIsStart = flag;};
private:
	typedef std::map<DWORD, IUserItem*> UsrDateMap;

	CUserItem* m_pMeUserItem;

	unsigned int m_nChair;
	unsigned int m_nTable;

	BYTE m_cbGameStatus;							//游戏状态
	BYTE m_cbAllowLookon;							//允许旁观

	BYTE m_cbStatus;								//游戏状态信息
	bool gameIsStart;								//游戏是否开始标识

protected:
	UsrDateMap m_UserDateList;
};

#endif /* defined(__Game__RoomFrame__) */
