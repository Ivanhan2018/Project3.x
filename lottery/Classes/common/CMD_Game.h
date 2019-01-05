#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE
#pragma once
#include "Define.h"
#pragma pack(1)

//////////////////////////////////////////////////////////////////////////
//��??o????��???��?

#define MDM_GR_LOGON				1									//��?o���??o

//#define SUB_GR_LOGON_ACCOUNTS		1									//��?a?��??o
//#define SUB_GR_LOGON_USERID			2									//I D ��??o
								//									

#define SUB_GR_LOGON_SUCCESS		100									//��??o�ݡ���?
#define SUB_GR_LOGON_ERROR			101									//��??o??��?
#define SUB_GR_LOGON_FINISH			102									//��??o?���ݡ�


//#define MDM_GF_GAME						100								//��Ϸ��Ϣ
//#define MDM_GF_FRAME					101								//�����Ϣ

//��?o�롯?�ҡ֦�??o
struct CMD_GR_LogonByAccounts
{
	DWORD							dwPlazaVersion;						//��?�ݡ�ި���?
	DWORD							dwProcessVersion;					//������?�ި���?
	unsigned char					szAccounts[NAME_LEN];				//��??o��?�ҡ�
	unsigned char					szPassWord[PASS_LEN];				//��??o��???
};

//��?o�� ID ��??o
struct CMD_GR_LogonByUserID
{
	DWORD							dwPlazaVersion;						//��?�ݡ�ި���?
	DWORD							dwProcessVersion;					//������?�ި���?
	DWORD							dwUserID;							//����a? I D
	unsigned char					szPassWord[PASS_LEN];				//��??o��???
};

//��??o�ݡ���????��
/*struct CMD_GR_LogonSuccess
{
	DWORD							dwUserID;							//����a? I D
	//WORD							wJoinMatch;							//��???  
};*/

//��??o??��?
struct CMD_GR_LogonError
{
	LONG							lErrorCode;							//����??���B??
	char							szErrorDescribe[128];				//����?????��
};

//��???
enum enEnterRoomType
{
	FIRST_LOGON  =0,                  //��?��a��?��??o
	RECONNECT_LOGON =1,               //��??����??o
};

//////////////////////////////////////////////////////////////////////////
//����a?????��???��?

//#define MDM_GR_USER					2									//����a?�C��?��

#define SUB_GR_USER_SIT_REQ			1									//?��??????
#define SUB_GR_USER_LOOKON_REQ		2									//�֡��Ѐ????
#define SUB_GR_USER_STANDUP_REQ		3									//???��????
#define SUB_GR_USER_LEFT_GAME_REQ	4									//????��??��

#define SUB_GR_USER_COME			100									//����a?����??
//#define SUB_GR_USER_STATUS			101									//����a??��?��
//#define SUB_GR_USER_SCORE			102									//����a?�ơ�??
#define SUB_GR_SIT_FAILED			103									//?��????��?
#define SUB_GR_USER_RIGHT			104									//����a?????
#define SUB_GR_MEMBER_ORDER			105									//a��������?o?
#define SUB_GR_USER_SORT			106									//����a?��?��?
#define SUB_GR_USER_MATCH_SCORE     107                                 //��???����a?��?��? 20109.29 by gaoshan
#define SUB_GR_MATCH_USER_COM       108                                 //��???����a?����??2010.10.5 by gaoshan

//#define SUB_GR_USER_CHAT			200									//???????��
#define SUB_GR_USER_WISPER			205									//������????��
//#define SUB_GR_USER_RULE			202									//����a?�Ш�����

//#define SUB_GR_USER_INVITE			300									//��??????��
//#define SUB_GR_USER_INVITE_REQ		301									//��???????


#define SUB_GR_JOIN_MATCH			400									//��?o����???
//#define SUB_GR_JOIN_MATCH_REQ		401									//��???????
#define SUB_GR_USER_START_QUEUE_REQ 401                                 //�֡�?��????           2010.9.13 by gaoshan add

#define SUB_GR_MODIFY_GOLD			402									//�C?��?��?����
#define SUB_GR_GAME_MATCH_INFO_REQ	403									//��??�ơ�???�C��?��????
#define SUB_GR_GAME_MATCH_INFO		404									//��??�ơ�???�C��?��
#define SUB_GR_CLOSE_GAME_ROOM		405									//��?������??�ơ�?o��
#define SUB_GR_CLIENT_READY         406                               //??a??������?��?��???����? 2010.9.14 by gaoshan add
#define SUB_GR_CLIENT_START_GAME    407                               //??a??��???o��??�� 2010.9.14 by gaoshan add
//////////////////////////////////////////////?��?���ꡧ9.15
#define SUB_GR_SET_VIPROOM_RATE     408									//��?�¡�VIP��?o���??????��
#define SUB_GR_OK_PASSWORD_REQ		409									//a���̡̡�???
#define SUB_GR_ERROR_PASSWORD		410									//��???����??
#define SUB_GR_START_GAME_INFO		411									//��??��???o�C��?��//?��?���ꡧ20110105
#define SUB_GR_END_GAME_INFO		412									//��??�Ʀ���?���C��?��//?��?���ꡧ20110105
#define SUB_GR_DRAW_VIPRATE			413									//a��a�١�?o���???//?��?���ꡧ20110818

//a��������?o?
struct CMD_GR_MemberOrder
{
	DWORD							dwUserID;							//?????? ID
	BYTE							cbMemberOrder;						//a��������?o?
};

//???��?��?��//?��?���ꡧ20110105
struct CMD_GR_TaskSystem
{
	WORD							wKindID;							//��??��ID
	WORD							wProblemID;							//????ID
	WORD							wEggNumber;							//��?�̡�????
	bool							bDrawAnimation;						//??�ƨ�?��????a��
};
//����a????����??��				
struct tagUserTaskParameter
{
	DWORD	dwTaskID;				//???��ID
	DWORD	dwCondition;			//����?��??o?
	DWORD	dwRewardA;				//����?��A
	DWORD	dwRewardB;				//����?��B
	DWORD	dwTaskKind;				//???��??�C?�ꡧ0 �C??�� 1��?o?�ꡧ2?��??��?o??��???��
	DWORD	dwFinish;				//?���ݡ�
	DWORD	dwDoingValue;			//???��?���ݡ�??�¦�

	char	szTaskName[128];		//��?��?
	char	szDescript[128];		//???����???
	char	szRewardContent[128];	//����?��
};

//����a?????
struct CMD_GR_UserRight
{
	DWORD							dwUserID;							//?????? ID
	DWORD							dwUserRight;						//����a?????
};

//����a??��?��
//struct CMD_GR_UserStatus
//{
//	DWORD							dwUserID;							//?????? ID
//	WORD							wTableID;							//???��?a�¡�
//	WORD							wChairID;							//��??��?a�¡�
//	BYTE							cbUserStatus;						//����a??��?��
//};

//����a?�ơ�??
struct CMD_GR_UserScore
{
	//LONG							lLoveliness;						//����a?????
	LONG							lWeekWinCount;						//?��??   2010-12-30 by zhanghua
	LONG							lWeekMingci;						//��?�֡֡�? 2010-12-30 by zhanghua
	DWORD							dwUserID;							//����a? I D
	tagUserScore					UserScore;							//a?�ơ¨C��?��
};

//����a?��???a?�ơ�2010.9.29 by gaoshan
struct CMD_GR_UserMatchScore
{
	DWORD                           dwUserID;                          //����a?ID
	LONG                            lMatchScore;                       //��???a?�ơ�
};

//����a?��?��?
struct CMD_GR_UserSort
{
	DWORD								dwUserID;							//����a? I D
	WORD								wSortID;							//�֡֡�?
	EN_MatchStatus						enMatchStatus;						//��????��?��
};

struct CMD_GR_AddBlood_Gold
{
	DWORD                               dwUserID;                          //����a?ID
	WORD                                wSubGold;
};

//?????��??
struct CMD_GR_UserSitReq
{
	//WORD							wTeamID;							//�C��?���ҡ�??  2010.9.14 by gaoshan 
	WORD							wTableID;							//???��?a�¡�
	WORD							wChairID;							//��??��?a�¡�
	BYTE							cbPassLen;							//��???�ݡ�??
	char							szTablePass[PASS_LEN];				//???����???
};

//��???����a? 
struct CMD_GR_UserInviteReq
{
	WORD							wTableID;							//???���ҡ�??
	DWORD							dwUserID;							//����a? I D
};

//?��????��?
struct CMD_GR_SitFailed
{
	char							szFailedDescribe[256];				//����??��???
};

//????�����Ц� 
struct CMD_GR_UserChat
{
	WORD							wChatLength;						//�C��?��ݡ�??
	//COLORREF						crFontColor;						//�C��?�顪������
	DWORD							dwSendUserID;						//�ơ騤?����a?
	DWORD							dwTargetUserID;						//??��������a?
	char							szChatMessage[MAX_CHAT_LEN];		//????�C��?��
};

//������?�����Ц� 
struct CMD_GR_Wisper
{
	WORD							wChatLength;						//�C��?��ݡ�??
	//COLORREF						crFontColor;						//�C��?�顪������
	DWORD							dwSendUserID;						//�ơ騤?����a?
	DWORD							dwTargetUserID;						//??��������a?
	char							szChatMessage[MAX_CHAT_LEN];		//????�C��?��
};

//����a?�Ш�����
struct CMD_GR_UserRule
{
	bool							bPassword;							//��?�¡̡�???
	bool							bLimitWin;							//??��??��??
	bool							bLimitFlee;							//??��?????
	bool							bLimitScore;						//??��?�ơ�??
	bool							bCheckSameIP;						//�C?������?�¡�
	WORD							wWinRate;							//??��??��??
	WORD							wFleeRate;							//??��??����?
	LONG							lMaxScore;							//?����?�ơ�?? 
	LONG							lLessScore;							//?����?�ơ�??
	char							szPassword[PASS_LEN];				//???����???
};

//��???����a? 
struct CMD_GR_UserInvite
{
	WORD							wTableID;							//???���ҡ�??
	DWORD							dwUserID;							//����a? I D
	
};

//��?o����???
struct CMD_GR_JoinMatch
{
	WORD							wJoin;								//�C��?��??
	char							szVIPPassWord[PASS_LEN];			//VIP��?o���???///////////?��?���ꡧ9.21
	LONG							lCellScore;							//��???////////////?��?���ꡧ101004
};
/////////////////////////////////////////?��?���ꡧ9.15
struct CMD_GR_Draw_VIPRate
{
	LONG							nRate;								//VIP��?o���???
	
};

struct CMD_GR_VIPRoom_PassWord
{
	char							szPassword[PASS_LEN];				//VIP��?o���???
    LONG							lCellScore;							//��???
};
/////////////////////////////////////////?��?���ꡧ9.15

//��??�ơ�???�C��?��
struct CMD_GR_GameMatchInfo
{
	WORD							wSignUpCount;						//��?��??��??	
	WORD							wJoinCount;							//��????��??
	WORD							wPlayeCount;						//o�졯��?��??
	DWORD                           dwUserID;                           //����a?ID
	char							szMatchTime[24];					//��?��?��????��o��
	char							szMatchStatus[16];					//��????��?��
};
//////////////////////////////////////////////////////////////////////////
//�֡�¡̨C��?��????��?

#define MDM_GR_INFO					3									//�֡�¡̨C��?��

#define SUB_GR_SERVER_INFO			100									//��?o��֡�¡�
#define SUB_GR_ORDER_INFO			101									//��?o?�֡�¡�
#define SUB_GR_MEMBER_INFO			102									//a�������֡�¡�
#define SUB_GR_COLUMN_INFO			108									//?�C�����֡�¡�
//#define SUB_GR_CONFIG_FINISH		104									//�֡�¡�?���ݡ�

//��??�ơ�?o��C��?��
struct CMD_GR_ServerInfo
{
	//��?o��?���C��
	WORD							wKindID;							//??�C? I D
	WORD							wTableCount;						//???��????
	WORD							wChairCount;						//��??��????
	DWORD							dwVideoAddr;						//?��?�̦�?�¡�

	//??���С֡�¡�
	WORD							wGameGenre;							//��??��??�C?
	BYTE							cbHideUserInfo;						//��?��?�C��?��
	//WORD							wMatchType;							//��?????�C? 2010.9.13 by gaoshan
	BYTE                            cbLimitDraw;                        //??�ƨ�??��?��??? 2010.9.17 by gaoshan add                          
};

//�ơ�??��???�C��?��
struct CMD_GR_ScoreInfo
{
	WORD							wDescribeCount;						//��???????
	WORD							wDataDescribe[16];					//????������?
};

//��?o?��???�����Ц�
struct tagOrderItem
{
	LONG							lScore;								//��?o?a?�ơ�
	WORD							wOrderDescribe[16];					//��?o?��???
};

//��?o?��???�C��?��
struct CMD_GR_OrderInfo
{
	WORD							wOrderCount;						//��?o?????
	tagOrderItem					OrderItem[128];						//��?o?��???
};

//?�C����?����???�����Ц�
/*struct tagColumnItem
{
	WORD							wColumnWidth;						//?�C����?��??
	WORD							wDataDescribe;						//?��????�C?
	char							szColumnName[16];					//?�C������??��
};*/

//?�C������???�C��?��
struct CMD_GR_ColumnInfo
{
	WORD							wColumnCount;						//?�C����????
	tagColumnItem					ColumnItem[32];						//?�C������???
};

//////////////////////////////////////////////////////////////////////////
//��?o��?��?��????��?

#define MDM_GR_STATUS				4									//?��?���C��?��

#define SUB_GR_TABLE_INFO			100									//???���C��?��
#define SUB_GR_TABLE_STATUS			101									//???��?��?��


//???��?��?�������Ц�
/*struct tagTableStatus
{
	BYTE							bTableLock;							//����???��?��
	BYTE							bPlayStatus;						//��??��?��?��
};*/

//???��?��?��???��
struct CMD_GR_TableInfo
{
	WORD							wTableCount;						//???��????
	tagTableStatus					TableStatus[512];					//?��?��???��
};

//???��?��?���C��?��
/*struct CMD_GR_TableStatus
{
	WORD							wTableID;							//???���ҡ�??
	BYTE							bTableLock;							//����???��?��
	BYTE							bPlayStatus;						//��??��?��?��
	LONG							lCellScore;							//???����???
};*/

//////////////////////////////////////////////////////////////////////////
//��??��????��?

#define MDM_GR_MANAGER				5									//��??����??��

#define SUB_GR_SEND_WARNING			1									//�ơ騤???�Ǩ�
#define SUB_GR_SEND_MESSAGE			2									//�ơ騤????��
#define SUB_GR_LOOK_USER_IP			3									//�ܨ�?����?�¡�
#define SUB_GR_KILL_USER			4									//??��?����a?
#define SUB_GR_LIMIT_ACCOUNS		5									//��?���̡�?a?
#define SUB_GR_SET_USER_RIGHT		6									//????��?�¡�
//#define SUB_GR_OPTION_SERVER		7									//��?o�롭?�¡�

//�ơ騤???�Ǩ�
struct CMD_GR_SendWarning
{
	WORD							wChatLength;						//�C��?��ݡ�??
	DWORD							dwTargetUserID;						//??��������a?
	char							szWarningMessage[MAX_CHAT_LEN];		//??�Ǩ�???��
};

//?��?��???��
struct CMD_GR_SendMessage
{
	BYTE							cbGame;								//��??��???��
	BYTE							cbRoom;								//��??��???��
	WORD							wChatLength;						//�C��?��ݡ�??
	char							szSystemMessage[MAX_CHAT_LEN];		//?��?��???��
};

//�ܨ�?����?�¡�
struct CMD_GR_LookUserIP
{
	DWORD							dwTargetUserID;						//??��������a?
};

//??��?����a?
struct CMD_GR_KillUser
{
	DWORD							dwTargetUserID;						//??��������a?
};

//��?���̡�?a?
struct CMD_GR_LimitAccounts
{
	DWORD							dwTargetUserID;						//??��������a?
};

//????��?�¡�
struct CMD_GR_SetUserRight
{
	//??��������a?
	DWORD							dwTargetUserID;						//??��������a?

	//��???����??
	BYTE							cbGameRight;						//��?�ҡ�????
	BYTE							cbAccountsRight;					//��?�ҡ�????

	//????����a?
	BYTE							cbLimitRoomChat;					//��???????
	BYTE							cbLimitGameChat;					//��??��????
	BYTE							cbLimitPlayGame;					//��??��????
	BYTE							cbLimitSendWisper;					//�ơ騤????��
	BYTE							cbLimitLookonGame;					//�֡��Ѐ????
};

//�C?��?��???????
struct CMD_CS_MatchData
{
	DWORD								dwUserID;							//����a? I D
	tagUserGold							UserGold;							//����a?��?����

};
//��?�¡̡�����?
#define OSF_ROOM_CHAT				1									//��???????
#define OSF_GAME_CHAT				2									//��??��????
#define OSF_ROOM_WISPER				3									//��???����??
#define OSF_ENTER_GAME				4									//����??��??��
#define OSF_ENTER_ROOM				5									//����??��?o��
#define OSF_SHALL_CLOSE				6									//o�������?����

//��?o�롭?�¡�
struct CMD_GR_OptionServer
{
	BYTE							cbOptionFlags;						//��?�¡̡�����?
	BYTE							cbOptionValue;						//��?�¡̡�����?
};

//////////////////////////////////////////////////////////////////////////
//?��?��????��?

#define MDM_GR_SYSTEM				10									//?��?�ݨC��?��

#define SUB_GR_MESSAGE				100									//?��?��???��

//???��??�C?
#define SMT_INFO					0x0001								//�C��?��???��
#define SMT_EJECT					0x0002								//��?��????��
#define SMT_GLOBAL					0x0004								//?��?��???��

#define SMT_CONNECT_NEXT			0x0008								//??��??������??��a��?��??��??
#define SMT_TABLE_ROLL				0x0010							   //��??��??�̨���??????�� 2009-12-9 hjw add
#define SMT_TABLE_FIX				0x0020							   //��??��??�̨���??????�� 2009-12-11 hjw add
#define SMT_TABLE_WINTIP			0x0040							   //��?��?o��?��??
#define SMT_TABLE_ANI				0x0080							   //a�ޡơ���???a��

#define SMT_CLOSE_ROOM				0x1000								//��?������?o��
#define SMT_INTERMIT_LINE			0x4000								//�¨C???������

//???��????��?
struct CMD_GR_Message
{
	WORD							wMessageType;						//???��??�C?
	WORD							wMessageLength;						//???��ݡ�??
	char							szContent[1024];					//???��????
};

////�汾��Ϣ
//struct CMD_GF_Info
//{
//	BYTE								bAllowLookon;					//�Թ۱�־
//};
//////////////////////////////////////////////////////////////////////////
//��?o��????��?

#define MDM_GR_SERVER_INFO			11									//��?o��C��?��

#define SUB_GR_ONLINE_COUNT_INFO	100									//��???�C��?��

//?��??�C��?��
struct tagOnLineCountInfo
{
	WORD							wKindID;							//??�C?����??
	DWORD							dwOnLineCount;						//��????��??
};

/////////////////////////////////////////////////////////////////?��?���ꡧ9.21
//VIP???���C��?��
struct tagVIPTableInfo
{
	WORD							wTableID;							//???���ҡ�
	char							szPassWord[PASS_LEN];				//???����???
	bool							bTablePassWord;						//����???��??�ƨ����C��???
	LONG							lCellScore;							//��???
};


//////////////////////////////////////////////////////////////////////////
//�����¼�
struct NTY_ControlEvent
{
	WORD							wControlID;							//���Ʊ�ʶ
};

//��ʱ���¼�
//struct NTY_TimerEvent
//{
//	DWORD							dwTimerID;							/��ʶ
//	WPARAM							dwBindParameter;					//�󶨲���
//};

//���ݿ��¼�
struct NTY_DataBaseEvent
{
	WORD							wRequestID;							//�����ʶ
	DWORD							dwContextID;						//�����ʶ
};

//��ȡ�¼�
struct NTY_TCPSocketReadEvent
{
	WORD							wDataSize;							//���ݴ�С
	WORD							wServiceID;							//�����ʶ
	CMD_Command						Command;							//������Ϣ
};

//�ر��¼�
struct NTY_TCPSocketCloseEvent
{
	WORD							wServiceID;							//�����ʶ
	BYTE							cbShutReason;						//�ر�ԭ��
};

//�����¼�
struct NTY_TCPSocketConnectEvent
{
	int								nErrorCode;							//�������
	WORD							wServiceID;							//�����ʶ
};

//Ӧ���¼�
struct NTY_TCPNetworkAcceptEvent
{
	DWORD							dwSocketID;							//�����ʶ
	DWORD							dwClientIP;							//���ӵ�ַ
};

//��ȡ�¼�
struct NTY_TCPNetworkReadEvent
{
	WORD							wDataSize;							//���ݴ�С
	DWORD							dwSocketID;							//�����ʶ
	CMD_Command						Command;							//������Ϣ
};

//�ر��¼�
struct NTY_TCPNetworkCloseEvent
{
	DWORD							dwSocketID;							//�����ʶ
	DWORD							dwClientIP;							//���ӵ�ַ
	DWORD							dwActiveTime;						//����ʱ��
};

#pragma pack()
#endif