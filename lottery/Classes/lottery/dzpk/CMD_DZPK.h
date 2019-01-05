//DZPK命令行类 .h
//Created by xxxx on 14-1-7.

#ifndef GAME_DZPK_CMD_ZJH_H
#define GAME_DZPK_CMD_ZJH_H
#include"Define.h"
#pragma pack(1)//<by hxh>
// 公共宏定义

// add by hxh 20160930
#define USE_NEW_PROTOCOL
//#define AS_PLAYERINFO_FIELD

//	游戏层级枚举
enum EDZPKLayerZOrder
{
	EDZPKLayerZOrder_Table = 1000,	//	牌桌
	EDZPKLayerZOrder_PlayerInfo,	//	玩家信息
	EDZPKLayerZOrder_Card,	//	卡牌
	EDZPKLayerZOrder_Trusteeship,	//	托管
	EDZPKLayerZOrder_Control,	//	控制层（按钮）
	EDZPKLayerZOrder_Dialog,	//	对话框
};

#if 0
#define bkWidth							854
#define bkHeight						480
#define xDelta							50
#define yDelta							50
#endif
#define cardDelta						5
#define MY_VIEW_CHAIR_ID                3									//玩家视图ID
#define DDPKKIND_ID						308									//游戏 I D
#define DZPKGAME_PLAYER					8									//游戏人数
#define GAME_NAME						TEXT("德州扑克")					//游戏名字
#define GAME_GENRE						(GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//游戏类型
#define COMMON_CARDS_COUNT				5						            // 公共牌数目<by hxh>
#define MAX_JETION_AREA                 4                                   // 下注区域
#define MAX_TIMES                       5                                   // 最大赔率


//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码


//扑克类型
#define CT_SINGLE					1									//单牌类型
#define CT_ONE_LONG				    2									//对子类型
#define CT_TWO_LONG			    	3									//两对类型
#define CT_THREE_TIAO				4									//三条类型
#define	CT_SHUN_ZI					5									//顺子类型
#define CT_TONG_HUA					6									//同花类型
#define CT_HU_LU					7									//葫芦类型
#define CT_TIE_ZHI					8									//铁支类型
#define CT_TONG_HUA_SHUN			9									//同花顺型
#define CT_KING_TONG_HUA_SHUN		10									//皇家同花顺

//数目定义
#define DZPK_FULL_COUNT					52									//全牌数目
#define DZPK_HAND_CARD_COUNT			2									//底牌数目<by hxh>
#define MAX_CENTERCOUNT					5									//最大数目


////////////////////////////////////////////////////////////////////////////
// 服务器命令结构
#define SUB_S_GAME_START				100									//游戏开始
#define SUB_S_ADD_SCORE					101									//加注结果
#define SUB_S_GIVE_UP					102									//放弃跟注	
#define SUB_S_SEND_CARD					103									//发牌消息
#define SUB_S_GAME_END					104									//游戏结束
#define SUB_S_SIT_DOWN					105									//用户坐下
#define SUB_S_OPEN_CARD					106									//用户开牌
#define SUB_S_SP_USER					107									//特殊用户

//客户端命令结构
#define SUB_C_ADD_SCORE					1									//用户加注
#define SUB_C_GIVE_UP					2									//放弃消息
#define SUB_C_OPEN_CARD					3									//开牌消息
#define SUB_C_IN_CHIPS					 4001								//兑换筹码
#define SUB_C_CHECK						 4002								//过（第二回合开始才可以有的操作）

// add by hxh 20161009
inline int DZPKGetActionID(BYTE cbJudgeAction)
{
	int actionID=cbJudgeAction+100;
	return actionID;
}

// 测试用<by hxh>
inline const char * DZPKGetCardName(unsigned char cbCardData)
{
	//获取花色={0方块,16梅花,32红桃,48黑桃,64王}
	BYTE cbCardColor=cbCardData&LOGIC_MASK_COLOR;
	//获取数值
	BYTE cbCardValue=cbCardData&LOGIC_MASK_VALUE;

	//有效判断
	bool bLegal=false;
	if ((cbCardData==0x4E)||(cbCardData==0x4F))
		bLegal=true;
	if ((cbCardColor<=0x30)&&(cbCardValue>=0x01)&&(cbCardValue<=0x0D)) 
		bLegal=true;

	if(bLegal)
	{
		static const char * szCardName[5][15]={
			{"方A","方2","方3","方4","方5","方6","方7","方8","方9","方10","方J","方Q","方K","",""},
			{"梅A","梅2","梅3","梅4","梅5","梅6","梅7","梅8","梅9","梅10","梅J","梅Q","梅K","",""},
			{"心A","心2","心3","心4","心5","心6","心7","心8","心9","心10","心J","心Q","心K","",""},
			{"桃A","桃2","桃3","桃4","桃5","桃6","桃7","桃8","桃9","桃10","桃J","桃Q","桃K","",""},
			{"","","","","","","","","","","","","","小王","大王"},
		};
		unsigned char nColorIndex=(cbCardData & 0xf0)>>4;
		unsigned char nValueIndex=(cbCardData & 0x0f)-1;
		return szCardName[nColorIndex][nValueIndex];
	}
	else
		return "";
}

// 测试用<by hxh>
inline const char * DZPKGetCardPicName(unsigned char cbCardData)
{
	//获取花色={0方块,16梅花,32红桃,48黑桃,64王}
	BYTE cbCardColor=cbCardData&LOGIC_MASK_COLOR;
	//获取数值
	BYTE cbCardValue=cbCardData&LOGIC_MASK_VALUE;

	//有效判断
	bool bLegal=false;
	if ((cbCardData==0x4E)||(cbCardData==0x4F))
		bLegal=true;
	if ((cbCardColor<=0x30)&&(cbCardValue>=0x01)&&(cbCardValue<=0x0D)) 
		bLegal=true;

	if(bLegal)
	{
		static const char * szCardName[5][15]={
			{"Ad","2d","3d","4d","5d","6d","7d","8d","9d","Td","Jd","Qd","Kd","",""},
			{"Ac","2c","3c","4c","5c","6c","7c","8c","9c","Tc","Jc","Qc","Kc","",""},
			{"Ah","2h","3h","4h","5h","6h","7h","8h","9h","Th","Jh","Qh","Kh","",""},
			{"As","2s","3s","4s","5s","6s","7s","8s","9s","Ts","Js","Qs","Ks","",""},
			{"","","","","","","","","","","","","","00","00"},
		};
		unsigned char nColorIndex=(cbCardData & 0xf0)>>4;
		unsigned char nValueIndex=(cbCardData & 0x0f)-1;
		return szCardName[nColorIndex][nValueIndex];
	}
	else
		return "00";
}

//空闲状态
struct CMD_S_StatusFree_dzpk
{
	LONGLONG							lCellMinScore;						//最小下注
	LONGLONG							lCellMaxScore;						//最大下注
};

//游戏状态
struct CMD_S_StatusPlay_dzpk
{
	//加注信息
	LONGLONG							lCellScore;								//单元下注
	LONGLONG							lTurnMaxScore;							//最大下注
	LONGLONG							lTurnLessScore;							//最小下注
	LONGLONG							lCellMaxScore;							//最大下注
	LONGLONG							lAddLessScore;							//加最小注
	LONGLONG							lTableScore[DZPKGAME_PLAYER];			//下注数目
	LONGLONG			 				lTotalScore[DZPKGAME_PLAYER];			//累计下注
	LONGLONG							lCenterScore;							//中心筹码

	//状态信息
	WORD								wDUser;									//D玩家
	WORD				 				wCurrentUser;							//当前玩家
	BYTE								cbPlayStatus[DZPKGAME_PLAYER];			//游戏状态,取值0和1
	LONGLONG							lUserMaxScore[DZPKGAME_PLAYER];			//用户金币数<by hxh>
	BYTE							    cbBalanceCount;							//平衡次数

	//扑克信息
	BYTE								cbCenterCardData[MAX_CENTERCOUNT];		//扑克数目
	BYTE								cbHandCardData[DZPK_HAND_CARD_COUNT];	//收到消息玩家的椅子手牌
};

//用户下注
struct CMD_S_AddScore
{
	WORD								wCurrentUser;						//当前用户
	WORD								wAddScoreUser;						//加注用户
	LONGLONG							lAddScoreCount;						//加注数目
	LONGLONG							lTurnLessScore;						//最小下注
	LONGLONG							lTurnMaxScore;						//最大下注
	LONGLONG							lAddLessScore;						//最小加注
	BYTE								cbJudgeAction;                       //判断下注类型   "1过","2跟注","3加注","4下注","5弃牌","6全下
};

//用户加注
struct CMD_C_AddScore
{
	LONGLONG							lScore;								//加注数目
	BYTE								cbJudgeAction;                       //判断下注类型   "1过","2跟注","3加注","4下注","5弃牌","6全下
};
//用户放弃
struct CMD_S_GiveUp
{
	WORD								wGiveUpUser;						//放弃用户
	LONGLONG							lLost;								//输掉金币	
};
//发牌消息数据包
struct CMD_S_SendCard
{
	BYTE								cbPublic;							//是否公牌
	WORD								wCurrentUser;						//当前用户
	BYTE								cbSendCardCount;					//发牌数目={3,4,5}
	BYTE								cbCenterCardData[MAX_CENTERCOUNT];	//中心扑克	
};

//游戏开始
struct CMD_S_GameStart_dzpk
{
	WORD				 				wCurrentUser;						//当前玩家
	WORD								wDUser;								//D玩家
	WORD								wMinChipInUser;						//小盲注玩家
	WORD								wMaxChipInUser;						//大盲注玩家	
	LONGLONG							lCellScore;							//单元下注
	LONGLONG							lTurnMaxScore;						//最大下注
	LONGLONG							lTurnLessScore;						//最小下注
	LONGLONG							lAddLessScore;						//加最小注
#ifdef USE_NEW_PROTOCOL
	LONGLONG							lUserMaxScore[DZPKGAME_PLAYER];			//用户金币数<by hxh>
	BYTE								cbPlayStatus[DZPKGAME_PLAYER];		//游戏状态,取值0和1
#endif
	BYTE								cbCardData[DZPKGAME_PLAYER][DZPK_HAND_CARD_COUNT];	//用户扑克
};

//空结构体
struct CMD_C_NULL
{

};


//游戏结束
struct CMD_S_GameEnd_dzpk
{
	BYTE							cbTotalEnd;							    //强退标志	1：完成正常流程（发完5张公共牌）结束游戏；	0：未完成正常流程结束游戏
	double							lGameTax[DZPKGAME_PLAYER];				//游戏税收
	double							lGameScore[DZPKGAME_PLAYER];			//游戏得分
	BYTE							cbCardData[DZPKGAME_PLAYER][DZPK_HAND_CARD_COUNT];	//用户扑克
	BYTE							cbLastCenterCardData[DZPKGAME_PLAYER][MAX_CENTERCOUNT];//最后扑克
	BYTE							cbLastCardKind[DZPKGAME_PLAYER];        //最后的牌型
};

//用户开牌
struct CMD_S_OpenCard
{
	WORD								wWinUser;							//用户开牌
};


//胜利信息结构
struct UserWinList
{
	BYTE bSameCount;
	WORD wWinerList[DZPKGAME_PLAYER];
};
//分析结构
struct tagAnalyseResult_dzpk
{
	BYTE 							cbFourCount;						//四张数目
	BYTE 							cbThreeCount;						//三张数目
	BYTE 							cbLONGCount;						//两张数目
	BYTE							cbSignedCount;						//单张数目
	BYTE 							cbFourLogicVolue[1];				//四张列表
	BYTE 							cbThreeLogicVolue[1];				//三张列表
	BYTE 							cbLONGLogicVolue[2];				//两张列表
	BYTE 							cbSignedLogicVolue[5];				//单张列表
	BYTE							cbFourCardData[MAX_CENTERCOUNT];			//四张列表
	BYTE							cbThreeCardData[MAX_CENTERCOUNT];			//三张列表
	BYTE							cbLONGCardData[MAX_CENTERCOUNT];		//两张列表
	BYTE							cbSignedCardData[MAX_CENTERCOUNT];		//单张数目
};
#pragma pack()//<by hxh>
#endif