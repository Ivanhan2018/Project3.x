/*
 *  CMD_LogonServer.h
 *  test
 *
 *  Created by Aden on 14-11-20.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */
#include "packet.h"
#pragma once
#pragma pack(1)
/*
////////////////////////////////////////////////////////////////////////////////////////
// 系统消息
#define MDM_CM_SYSTEM					1000						//系统命令

#define SUB_CM_SYSTEM_MESSAGE			1							//系统消息
#define SUB_CM_ACTION_MESSAGE			2							//动作消息
#define SUB_CM_DOWN_LOAD_MODULE			3							//下载消息

//类型掩码
#define SMT_CHAT						0x0001						//聊天消息
#define SMT_POPMSG						0x0002						//弹出消息
#define SMT_GLOBAL						0x0004						//全局消息
#define SMT_PROMPT						0x0008						//提示消息
#define SMT_TABLE_ROLL					0x0010						//滚动消息

//控制掩码
#define SMT_CLOSE_ROOM					0x0100						//关闭房间
#define SMT_CLOSE_GAME					0x0200						//关闭游戏
#define	SMT_CLOSE_LINK					0x0400						//中断连接
*/

////系统消息
//struct CMD_CM_SystemMessage
//{
//	WORD							wType;								//消息类型
//	WORD							wLength;							//消息长度
//	TCHART							szString[1024];						//消息内容
//};






//#pragma mark -
//#pragma mark 游戏ID

#define GAME_LORDS						200							//斗地主
#define GAME_BEEF						300							//牛牛
















#pragma pack()
