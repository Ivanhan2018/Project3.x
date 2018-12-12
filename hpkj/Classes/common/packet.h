

#ifndef DEFINE_PACKET_H
#define DEFINE_PACKET_H
#pragma once
#include "Define.h"
#pragma pack(1)

//网络定义
//#define SOCKET_VER						0x66
#define SOCKET_VER						0x01
#define SOCKET_BUFFER					16384								//网络缓冲
#define SOCKET_PACKET					(SOCKET_BUFFER-sizeof(CMD_Head)-2*sizeof(DWORD))

//数据类型
#define DK_MAPPED			0x01						//映射类型
#define DK_ENCRYPT			0x02						//加密类型
#define DK_COMPRESS			0x04						//压缩类型

//长度定义
#define SOCKET_TCP_BUFFER	16384									//网络缓冲
#define SOCKET_TCP_PACKET	(SOCKET_TCP_BUFFER-sizeof(TCP_Head))	//网络缓冲

#define IPC_VER							0x0001
#define IPC_IDENTIFIER					0x0001
#define IPC_PACKAGE						4096
#define IPC_BUFFER						(sizeof(IPC_Head)+IPC_PACKAGE)

#define MDM_GR_MATCH				7									//比赛命令

#define SUB_GR_MATCH_FEE			400									//报名费用
#define SUB_GR_MATCH_NUM			401									//等待人数
#define SUB_GR_LEAVE_MATCH			402									//退出比赛
#define SUB_GR_MATCH_INFO			403									//比赛信息
#define SUB_GR_MATCH_WAIT_TIP		404									//等待提示
#define SUB_GR_MATCH_RESULT			405									//比赛结果
#define SUB_GR_MATCH_STATUS			406									//比赛状态
#define SUB_GR_MATCH_DESC			408									//比赛描述
#define SUB_GR_MATCH_REBUY			409									//重新买入
#define SUB_GR_MATCH_TIME			410									//比赛时间

#define SUB_CM_SYSTEM_MESSAGE		1									//系统消息
#define SUB_CM_ACTION_MESSAGE		2									//动作消息
#define SUB_CM_DOWN_LOAD_MODULE		3									//下载消息

//比赛状态
#define MS_NULL						0x00								//没有状态
#define MS_SIGNUP					0x01								//报名状态
#define MS_MATCHING					0x02								//比赛状态
#define MS_OUT						0x03								//淘汰状态

//////////////////////////////////////////////////////////////////////////////////
//网络消息

#define IPC_CMD_GF_SOCKET			1									//网络消息

#define IPC_SUB_GF_SOCKET_SEND		1									//网络发送
#define IPC_SUB_GF_SOCKET_RECV		2									//网络接收

#pragma mark 内核命令
#define MDM_KN_COMMAND			0						//内核命令
#define SUB_KN_DETECT_SOCKET	1						//检测命令
#define SUB_KN_VALIDATE_SOCKET	2						//验证命令
///////////////////////////////////////////////////////////////////////////
//结构定义
//网络验证
typedef struct
{
	TCHART				szValidateKey[64];							//验证字符
}TCP_Validate;

//比赛人数
struct CMD_GR_Match_Num
{
	DWORD							dwWaitting;							//等待人数
	DWORD							dwTotal;							//开赛人数
	DWORD							dwMax;								//最大人数
};

//系统消息
struct CMD_CM_SystemMessageDZPK
{
	WORD							wType;								//消息类型
	WORD							wLength;							//消息长度
	TCHART							szString[1024];						//消息内容
};
//动作消息
struct CMD_CM_ActionMessage
{
	WORD							wType;								//消息类型
	WORD							wLength;							//消息长度
	DWORD							nButtonType;						//按钮类型
	TCHART							szString[1024];						//消息内容
};

struct CMD_GR_MatchTime
{
	WORD							wNowHour;						//服务器小时
	WORD							wNowMinute;						//服务器分钟
	WORD							wNowSeconds;					//服务器秒
	WORD							wStartHour;						//开赛时间-小时
	WORD							wStartMinute;					//开赛时间-分钟
};

#define MAX_MATCH_DESC				4									//最多描述

//比赛描述
struct CMD_GR_MatchDesc
{
	TCHART							szTitle[MAX_MATCH_DESC][16];		//信息标题
	TCHART							szDescribe[MAX_MATCH_DESC][64];		//描述内容
	DWORD						crTitleColor;						//标题颜色
	DWORD						crDescribeColor;					//描述颜色
};

/////////////////////////////////////////////////////////////////////////////////////////
//内核命令
#define MDM_KN_COMMAND				0									//内核命令
#define SUB_KN_DETECT_SOCKET		1									//检测命令
#define SUB_KN_SHUT_DOWN_SOCKET			2								

//登陆状态
enum enLoginState
{
    enLogin_Server,                     //登陆服务器
    enGame_Server,                      //游戏服务器
};

//网络内核
struct CMD_Info
{
	BYTE								cbVersion;							
	BYTE								cbCheckCode;						//效验字段
	WORD								wPacketSize;						//数据大小
};

//网络内核
struct TCP_Info
{
	BYTE							cbDataKind;							//数据类型
	BYTE							cbCheckCode;						//效验字段
	WORD							wPacketSize;						//数据大小
};

//网络命令
struct CMD_Command
{
	WORD								wMainCmdID;							//主命令码
	WORD								wSubCmdID;							//子命令码
};

//网络命令
struct TCP_Command
{
	WORD							wMainCmdID;							//主命令码
	WORD							wSubCmdID;							//子命令码
};

//网络包头
struct CMD_Head
{
	CMD_Info							CmdInfo;							//基础结构
	CMD_Command							CommandInfo;						//命令信息
};

//网络包头
struct TCP_Head
{
	TCP_Info						TCPInfo;							//基础结构
	TCP_Command						CommandInfo;						//命令信息
};

//网络缓冲
struct CMD_Buffer
{
	CMD_Head							Head;								//数据包头
	BYTE								cbBuffer[SOCKET_PACKET];			//数据缓冲
};

struct CMD_KN_DetectSocket
{
	DWORD								dwSendTickCount;
	DWORD								dwRecvTickCount;
};

//网络接收
struct IPC_GF_SocketRecv
{
	TCP_Command						CommandInfo;						//命令信息
	BYTE							cbBuffer[SOCKET_TCP_PACKET];		//数据缓冲
};

struct IPC_Head
{
	WORD								wVersion;
	WORD								wDataSize;
	WORD								wMainCmdID;
	WORD								wSubCmdID;
};

struct IPC_Buffer
{
	IPC_Head							Head;								
	BYTE								cbBuffer[IPC_PACKAGE];
};

/////////////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif
