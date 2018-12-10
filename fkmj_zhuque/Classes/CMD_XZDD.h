#ifndef CMD_SPARROW_HEAD_FILE
#define CMD_SPARROW_HEAD_FILE

#include "GameDefine.h"

//组合子项
struct XZDD_CMD_WeaveItem
{
	BYTE							cbWeaveKind;						//组合类型
	BYTE							cbCenterCard;						//中心扑克
	BYTE							cbPublicCard;						//公开标志
	WORD							wProvideUser;						//供应用户

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(cbWeaveKind);
		Stream_VALUE(cbCenterCard);
		Stream_VALUE(cbPublicCard);
		Stream_VALUE(wProvideUser);
	}
};


#pragma pack(1)

//组件属性
#define GAME_PLAYER					4									//游戏人数

#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_MATCH|GAME_GENRE_GOLD)	//游戏类型

//游戏状态
#define GS_MJ_FREE					GAME_STATUS_FREE					//空闲状态
#define GS_MJ_PLAY					(GAME_STATUS_PLAY+1)				//游戏状态

#define GS_MJ_HUANPAI                 (GAME_STATUS_PLAY+2)						//换牌状态
#define GS_MJ_XUANQUE                 (GAME_STATUS_PLAY+3)						//选缺状态

#define TIME_START_GAME				30									//开始定时器
#define TIME_OPERATE_CARD				15									//操作定时器

#define		GAME_TYPE_SICHUAN  0                      //四川麻将
#define		GAME_TYPE_CHENGDU  1					   //成都麻将
#define		GAME_TYPE_YICHANG  2

#define		GAME_RULE_NO_WAIT				1
#define		GAME_RULE_XZ					2
#define		GAME_RULE_XL					3
#define		GAME_RULE_HUAN3				4
#define		GAME_RULE_DIN_QUE				5
#define		GAME_RULE_3FAN				6
#define		GAME_RULE_4FAN				7
#define		GAME_RULE_ZIMO_DI				8
#define		GAME_RULE_ZIMO_FAN			9
#define		GAME_RULE_YAOJIU_JIANGDUI		10
#define		GAME_RULE_TIANDI_HU			11
#define		GAME_RULE_5FAN				12
#define		GAME_RULE_DAODAO_HU			13
#define		GAME_RULE_DIANPAO				14
#define		GAME_RULE_ZIMOHU				15
#define		GAME_RULE_QIDUI				16

#define MAX_GAME_RULE				6		//最多规则
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_GAME_START			100									//游戏开始
#define SUB_S_OUT_CARD				101									//出牌命令
#define SUB_S_SEND_CARD				102									//发送扑克
#define SUB_S_OPERATE_NOTIFY		104									//操作提示
#define SUB_S_OPERATE_RESULT		105									//操作命令
#define SUB_S_GAME_END				106									//游戏结束
#define SUB_S_TRUSTEE				107									//用户托管
#define SUB_S_CHI_HU				108									//
#define SUB_S_GANG_SCORE			110									//

//2016.5.20
#define SUB_S_HUANPAI 120				//换牌
#define SUB_S_XUANQUE 121				//选缺
#define SUB_S_XUANQUE_NOTICE 122		//通知开始选缺
#define SUB_S_HUANPAI_CHENGDU 123				//换牌

#define SUB_S_HUANPAI_NOTICE  125				//换牌

#define SUB_S_MASTER_HANDCARD		220									//
#define SUB_S_MASTER_LEFTCARD		221									//剩余牌堆


#define ZI_PAI_COUNT	7												//堆立全牌

#define MASK_CHI_HU_RIGHT			0x0fffffff							//最大权位DWORD个数			

//常量定义
#define MAX_WEAVE					4									//最大组合
#define MAX_INDEX					34									//最大索引
#define MAX_COUNT					14									//最大数目
#define MAX_REPERTORY				108									//最大库存


//游戏状态
struct XZDD_CMD_S_StatusFree
{
	LONGLONG						lCellScore;							//基础金币
	WORD							wBankerUser;						//庄家用户
	bool							bTrustee[GAME_PLAYER];				//是否托管
};

//游戏状态
struct XZDD_CMD_S_StatusPlay
{
	//游戏变量
	LONGLONG						lCellScore;									//单元积分
	WORD							wBankerUser;								//庄家用户
	WORD							wCurrentUser;								//当前用户

	//状态变量
	BYTE							cbActionCard;								//动作扑克
	BYTE							cbActionMask;								//动作掩码
	BYTE							cbLeftCardCount;							//剩余数目
	bool							bTrustee[GAME_PLAYER];						//是否托管
	WORD							wWinOrder[GAME_PLAYER];						//
	BYTE							cbWinCout;									//获胜数目

	//出牌信息
	WORD							wOutCardUser;								//出牌用户
	BYTE							cbOutCardData;								//出牌扑克
	BYTE							cbDiscardCount[GAME_PLAYER];				//丢弃数目
	BYTE							cbDiscardCard[GAME_PLAYER][60];				//丢弃记录

	//扑克数据
	BYTE							cbCardCount;								//扑克数目
	BYTE							cbCardData[MAX_COUNT];						//扑克列表
	BYTE							cbSendCardData;								//发送扑克

	//组合扑克
	BYTE							cbWeaveCount[GAME_PLAYER];					//组合数目
	XZDD_CMD_WeaveItem					WeaveItemArray[GAME_PLAYER][MAX_WEAVE];		//组合扑克

	//堆立信息
	WORD							wHeapHand;									//堆立头部
	WORD							wHeapTail;									//堆立尾部
	BYTE							cbHeapCardInfo[GAME_PLAYER][2];				//堆牌信息


	BYTE							nQueColor[GAME_PLAYER];						//缺色
	BYTE							nChiHuCard[GAME_PLAYER][60];
	WORD							nChiHuCardNum[GAME_PLAYER];

	bool                         bHasHu[GAME_PLAYER];
};

//游戏开始
struct XZDD_CMD_S_GameStart
{
	QYLONG							lSiceCount;									//骰子点数
	WORD							wBankerUser;								//庄家用户
	WORD							wCurrentUser;								//当前用户
	BYTE							cbUserAction;								//用户动作
	BYTE							cbCardData[MAX_COUNT*GAME_PLAYER];			//扑克列表
	WORD							wHeapHand;									//堆立牌头
	WORD							wHeapTail;									//堆立牌尾
	BYTE							cbHeapCardInfo[GAME_PLAYER][2];				//堆立信息
	BYTE							cbLeftCardCount;							//

	BYTE							nLeftTime;                        //换牌倒计时
};

struct XZDD_CMD_S_StateHuanPai
{
	WORD							wBankerUser;								//庄家用户
	BYTE							cbCardData[MAX_COUNT];						//扑克列表
	BYTE							cbLeftCardCount;							//
	BYTE							nLeftTime;									//换牌倒计时
	bool                            bHuan;									    //是否已经换了
};

struct XZDD_CMD_S_StateXuanQue
{
	WORD							wBankerUser;								//庄家用户
	BYTE							cbCardData[MAX_COUNT];						//扑克列表
	BYTE							cbLeftCardCount;							//
	BYTE							nLeftTime;									//XuanQue倒计时
	BYTE                            nQueColor;									
};
//出牌命令
struct XZDD_CMD_S_OutCard
{
	WORD							wOutCardUser;						//出牌用户
	BYTE							cbOutCardData;						//出牌扑克
};

//发送扑克
struct XZDD_CMD_S_SendCard
{
	BYTE							cbCardData;							//扑克数据
	BYTE							cbActionMask;						//动作掩码
	WORD							wCurrentUser;						//当前用户
	bool							bTail;								//末尾发牌

	BYTE                           nLeftTime;

	bool                        bHasHuPai;					//是否已经胡
};


//操作提示
struct XZDD_CMD_S_OperateNotify
{
	WORD							wResumeUser;						//还原用户
	BYTE							cbActionMask;						//动作掩码
	BYTE							cbActionCard;						//动作扑克
	bool                         bHasHuPai;
};

//操作命令
struct XZDD_CMD_S_OperateResult
{
	WORD							wOperateUser;						//操作用户
	WORD							wProvideUser;						//供应用户
	BYTE							cbOperateCode;						//操作代码
	BYTE							cbOperateCard;						//操作扑克
};


//用户托管
struct XZDD_CMD_S_Trustee
{
	bool							bTrustee;							//是否托管
	WORD							wChairID;							//托管用户
};


//2016.5.20
struct XZDD_CMD_S_HuanPai
{
	WORD   wChairId;
	BYTE   cbHuanCard[3];
};
struct XZDD_CMD_S_HuanPaiChengDu
{
	WORD   wChairId;
	BYTE   cbRemoveHuanCard[3];
	BYTE   cbHuanCard[3];
	WORD   wSice;
};
struct XZDD_CMD_S_XuanQue
{
	BYTE nQueColor[4];
	BYTE nLeftTime;
};

struct XZDD_CMD_S_XuanQueNotice
{
	WORD   wChairId;
	BYTE nLeftTime;
	BYTE nQueColor;
};
//////////////////////////////////////////////////////////////////////////
//客户端命令结构

#define SUB_C_OUT_CARD				1									//出牌命令
#define SUB_C_OPERATE_CARD			3									//操作扑克
#define SUB_C_TRUSTEE				4									//用户托管

#define SUB_C_MASTER_LEFTCARD		6									//剩余牌堆
#define SUB_C_MASTER_CHEAKCARD		7									//选择的牌
#define SUB_C_MASTER_ZHANIAO		8									//扎鸟
//2016.5.20
#define SUB_C_HUANPAI 11				//换牌
#define SUB_C_XUANQUE 12				//选缺

#define CARD_COLOR_NULL			0
#define CARD_COLOR_TONG			1
#define CARD_COLOR_WAN			2
#define CARD_COLOR_TIAO			3
//出牌命令
struct XZDD_CMD_C_OutCard
{
	BYTE							cbCardData;							//扑克数据
};

//操作命令
struct XZDD_CMD_C_OperateCard
{
	BYTE							cbOperateCode;						//操作代码
	BYTE							cbOperateCard;						//操作扑克
};

//用户托管
struct XZDD_CMD_C_Trustee
{
	bool							bTrustee;							//是否托管	
};
//2016.5.20
struct XZDD_CMD_C_HuanPai
{
	BYTE    cbHuanCard[3];
	BYTE		nMasttKindColor;
};
struct XZDD_CMD_C_XuanQue
{
	BYTE nQueChoice;
};
//////////////////////////////////////////////////////////////////////////


struct  XZDD_Card_Info
{
	int		nLen;
	int		nColor;
	std::vector<BYTE> kCardList;

	bool   operator <  (const   XZDD_Card_Info&   rhs   )  const   //升序排序时必须写的函数
	{  
		return   nLen   <   rhs.nLen;
	}
};

struct XZDD_MaterCheckCard
{
	BYTE							cbCheakCard;						//操作代码
};

struct XZDD_MaterNiaoCout
{
	BYTE							cbNiaoCout;							//操作代码
};

struct XZDD_MasterLeftCard
{
	BYTE      kMasterLeftIndex[MAX_INDEX];
	BYTE      kMasterCheakCard;
};


#pragma pack()


struct  XZDDGameRecordPlayer
{
	DWORD dwUserID;
	std::string kHead;
	std::string kNickName;
	std::vector<BYTE> cbCardData;
	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(dwUserID);
		Stream_VALUE(kHead);
		Stream_VALUE(kNickName);
		Stream_VECTOR(cbCardData);
	}
};

struct  XZDDGameRecordOperateResult
{
	enum Type
	{
		TYPE_NULL,
		TYPE_OperateResult,
		TYPE_SendCard,
		TYPE_OutCard,
		TYPE_ChiHu,
	};
	XZDDGameRecordOperateResult()
	{
		cbActionType = 0;
		wOperateUser = 0;
		wProvideUser = 0;
		cbOperateCode = 0;
		cbOperateCard = 0;
	}
	BYTE							cbActionType;
	WORD							wOperateUser;						//操作用户
	WORD							wProvideUser;						//供应用户
	BYTE							cbOperateCode;						//操作代码
	BYTE							cbOperateCard;						//操作扑克
	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(cbActionType);
		Stream_VALUE(wOperateUser);
		Stream_VALUE(wProvideUser);
		Stream_VALUE(cbOperateCode);
		Stream_VALUE(cbOperateCard);
	}
};
struct  XZDDGameRecord
{
	std::vector<XZDDGameRecordPlayer> kPlayers;
	std::vector<XZDDGameRecordOperateResult> kAction;
	void StreamValue(datastream& kData,bool bSend)
	{
		StructVecotrMember(XZDDGameRecordPlayer,kPlayers);
		StructVecotrMember(XZDDGameRecordOperateResult,kAction);
	}

};


struct XZDD_MasterHandCardInfo
{
	int nChairId;
	std::vector<BYTE>    kMasterHandCard;

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(nChairId);
		Stream_VECTOR(kMasterHandCard);
	}
};

struct XZDD_MasterHandCard
{
	std::vector<XZDD_MasterHandCardInfo>    kMasterHandCardList;
	void StreamValue(datastream& kData,bool bSend)
	{
		StructVecotrMember(XZDD_MasterHandCardInfo,kMasterHandCardList);
	}
};


struct XZDD_CMD_S_ChiHu
{
	WORD							wChiHuUser;							//
	WORD							wProviderUser;						//
	BYTE							cbChiHuCard;						//
	std::vector<SCORE>            lGameScore;
	DWORD						dwChiHuRight;			//胡牌类型
	BYTE							cbGenCount;							//

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(wChiHuUser);
		Stream_VALUE(wProviderUser);
		Stream_VALUE(cbChiHuCard);
		Stream_VECTOR(lGameScore);
		Stream_VALUE(dwChiHuRight);
		Stream_VALUE(cbGenCount);
	}	
	void resizeData()
	{
		lGameScore.resize(GAME_PLAYER);
	}
};

//
struct XZDD_CMD_S_GangScore
{
	WORD							wChairId;							//
	WORD							wPravadeId;							//
	BYTE                         cbXiaYu;
	std::vector<SCORE>            lGangScore;

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(wChairId);
		Stream_VALUE(wPravadeId);
		Stream_VALUE(cbXiaYu);
		Stream_VECTOR(lGangScore);
	}
	void resizeData()
	{
		lGangScore.resize(GAME_PLAYER);
	}
};

struct XZDD_CMD_CardInfoList
{
	std::vector<BYTE>					kHandCardList;
	std::vector<XZDD_CMD_WeaveItem>    kWeaveItemList;
	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VECTOR(kHandCardList);
		StructVecotrMember(XZDD_CMD_WeaveItem,kWeaveItemList);
	}
};

struct XZDD_AllEndInfo
{
	std::vector<SCORE>            lGameScoreTotal;
	std::vector<SCORE>            lHuaZhuScore;
	std::vector<SCORE>            lChaJiaoScore;
	std::vector<BYTE>				cbGenCount;
	std::vector<BYTE>				cbFanShu;
	std::vector<XZDD_CMD_S_ChiHu>		kChiHuInfoList;
	std::vector<XZDD_CMD_S_GangScore>   kGangInfoList;

	std::vector<XZDD_CMD_CardInfoList>   kCardInfoList;
	
	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VECTOR(lGameScoreTotal);
		Stream_VECTOR(lHuaZhuScore);
		Stream_VECTOR(lChaJiaoScore);
		Stream_VECTOR(cbGenCount);
		Stream_VECTOR(cbFanShu);
		StructVecotrMember(XZDD_CMD_S_ChiHu,kChiHuInfoList);
		StructVecotrMember(XZDD_CMD_S_GangScore,kGangInfoList);
		StructVecotrMember(XZDD_CMD_CardInfoList,kCardInfoList);
	}
	void resizeData()
	{
		lGameScoreTotal.resize(GAME_PLAYER);
		lHuaZhuScore.resize(GAME_PLAYER);
		lChaJiaoScore.resize(GAME_PLAYER);
		cbGenCount.resize(GAME_PLAYER);
		cbFanShu.resize(GAME_PLAYER);
		kCardInfoList.resize(GAME_PLAYER);
	}
};


#endif
