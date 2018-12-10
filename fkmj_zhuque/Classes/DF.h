#ifndef _df_H_
#define _df_H_
#include "types.h"
#include "PacketAide.h"
#include "Platform.h"

#include "CMD_GameServer.h"
#include "CMD_LogonServer.h"

#define COL_N2G(col) (((col & 0xFF) << 16) | (col >> 8))
#define COL_G2N(col) ((col << 8) | (col >> 16))

//////////////////////////////////////////////////////////////////////////
// 对话框按钮
#define DLG_MB_OK                       0x1
#define DLG_MB_CANCEL					0x2
#define DLG_MB_YES						0x4
#define DLG_MB_NO						0x8

//////////////////////////////////////////////////////////////////////////////////
//结构定义

//用户属性
struct tagUserAttribute
{
	//用户属性
	dword							dwUserID;							//用户标识
	word							wTableID;							//桌子号码
	word							wChairID;							//椅子号码

	//权限属性
	dword							dwUserRight;						//用户权限
	dword							dwMasterRight;						//管理权限
};

//游戏属性
struct tagGameAttribute
{
	word							wKindID;							//类型标识
	word							wChairCount;						//椅子数目
	dword							dwClientVersion;					//游戏版本
	char							szGameName[LEN_KIND];				//游戏名字
};

//房间属性
struct tagServerAttribute
{
	word							wKindID;							//类型标识
	word							wServerID;							//房间规则
	char							szServerName[LEN_SERVER];			//房间名称
	word							wAVServerPort;						//视频端口
	dword							dwAVServerAddr;						//视频地址
	word							wServerType;						//房间类型

	word							wTableCount;						//桌子数目
	word							wChairCount;						//椅子数目
};

//////////////////////////////////////////////////////////////////////////
// 辅助定义
//////////////////////////////////////////////////////////////////////////
class DF
{
protected:
	std::string mGameName;
	int			mClientVersion;
	int			mKindID;
	int			mGamePlayers;

public:
	static DF* shared();
	static const char* MD5Encrypt(const char* pszSourceData);
public:
	DF();
	virtual ~DF();

	void init(int iKindID, int iPlayers, int iClientVersion, const char* sGameName);
public:
	
	byte		GetDeviceType();
	const char* GetMachineID();
	const char* GetMobilePhone();	
	uint32		GetPlazaVersion();
	uint32		GetFrameVersion();
	
	uint16		GetPlazzKindID();
	uint32		GetGameVersion();
	uint16		GetGameKindID();
	uint32		GetGamePlayer();
	const char* GetGameName(char szGameName[LEN_KIND]);
};
//////////////////////////////////////////////////////////////////////////
#endif // _df_H_