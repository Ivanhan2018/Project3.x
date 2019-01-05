//  [3/19/2014 wucan]
#ifndef		__CC_SCENELOGIC_WUCAN__
#define		 __CC_SCENELOGIC_WUCAN__
#include "cocos2d.h"
#include "SceneData.h"
// #include "SceneView.h"
#include "CardManager.h"
#include "OperalayerBJL.h"
#include "ChipManager.h"
#include "CardTableLayer.h"
#include "ViewMenuLayer.h"

USING_NS_CC;

//	百家乐——游戏状态
enum EBJLGameState
{
	EBJLGameState_Null = 0,	//	无
	EBJLGameState_Start,	//	游戏开始
	EBJLGameState_Idle,		//	空闲时间
	EBJLGameState_Bet,		//	下注时间
	EBJLGameState_End		//	游戏结束
};

class SceneLogic:public Node
{
public:
	CREATE_FUNC(SceneLogic);
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	//消息函数
	void onMessageGame(Object* obj);			
	void onMessageScene(Object* obj);		
	void onUpdateTable(Object* obj);		
	void onUpdataMatch(Object* obj);		//this Messager is userd to change gear
	void onViewMessage(Object* obj);
	bool onSceneMessage(unsigned char cbStation,const void* pBuffer,WORD wDataSize);
	bool onGameMessage(WORD wSubCmdID, const void* pBuffer, WORD wDataSize);

	CC_SYNTHESIZE(EBJLGameState, m_eGameState, GameState);

	//返回游戏状态值
	EBJLGameState	GetGameState();

	//系统函数
	// 获取数值
	static int GetCardValue(int nCardData) {
		return nCardData & MASK_VALUE;
	}

	// 获取花色
	static int GetCardColor(int nCardData) {
		return nCardData & MASK_COLOR;
	}

	// 逻辑数值
	static int GetCardLogicValue(int nCardData) {
		// 数值花色
		int nCardValue = SceneLogic::GetCardValue(nCardData);
		int nCardColor = SceneLogic::GetCardColor(nCardData);

		// 转换
		if (nCardColor == 0x40) {
			return nCardValue + 2;
		}
		return (nCardValue <= 2) ? (nCardValue + 13) : nCardValue;
	}
private:
	tagUserData* getMeUserDate() { return EntityMgr::instance()->roomFrame()->getMeUserItem()->getUserData(); }
	WORD getMeChairID(){return getMeUserDate()->wChairID;}
	WORD getMeChairID1(){
		WORD meChairID = EntityMgr::instance()->getDispatch()->m_wChairID;
		return meChairID;
	}
	int	switchViewChairID(unsigned int nChairID);
	//_state取值：0空闲时间、1下注时间、可以下注提示、2开牌时间<by hxh 20160610>
	void setGameStateTip(int _state);

	//庄家和闲家的下注提示不一样<by hxh 20160719>
	void setCallScoreTip(bool isMeBanker);
	//j取值：1由您做庄、2轮换庄家、3无人做庄<by hxh 20160720>
	void setChangeBankerTip(int j);
	void ChangeBankerTip_callback_once(float t);
	void sendSeverData(WORD wSubCmdID);
	void sendSeverData(WORD wSubCmdID,void* pData,WORD wDataSize);
	void sendFrameData( WORD wSubCmdID,void* pData,WORD wDataSize);
	void checkNet(float ft);
	void onLinkResult(Object* obj);
	void onUpdateScoreAndGoldEgg(Object* obj);
	int getUserRealChairID(DWORD userID);
	//功能函数
private:
	// 收到游戏结束包后先缓存起来，发完牌后才新增游戏记录、刷新金币、成绩、局数<by hxh 20160811>
	void AddGameRecord(Object *obj);

	void onUserStart(Object *pSender);
	bool onUserStartView(const void * pBuffer, WORD wDataSize);
	bool onUserBEeiLv(const void * pBuffer, WORD wDataSize);
	bool onUserAdd(const void * pBuffer, WORD wDataSize);
	bool onSubSendCard(const void * pBuffer, WORD wDataSize);
	bool onSubLandScore(const void * pBuffer, WORD wDataSize);
	bool onSubGameStart(const void * pBuffer, WORD wDataSize);
	bool onSubOutCard(const void * pBuffer, WORD wDataSize);
	bool onSubPassCard(const void * pBuffer, WORD wDataSize);
	//游戏结束;
	bool onSubGameEnd(const void * pBuffer, WORD wDataSize);
	//申请 请求
	bool onUserExpression(const void * pBuffer, WORD wDataSize);
	//提示消息<by hxh>
	bool onSubTipInfo( const void * pBuffer, WORD wDataSize);
	//游戏记录
	bool OnSubCheatCard(const void * pBuffer, WORD wDataSize);
	//取消坐庄
	bool OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize);
	//游戏空闲
	bool OnSubGameFree(const void *pBuffer, WORD wDataSize);
	//空闲状态<by hxh>
	bool OnGameSceneFree(const void *pBuffer, WORD wDataSize);
	//下注阶段<by zengxin>
	bool OnGameSceneOnBet(const void *pBuffer, WORD wDataSize);
	//游戏状态<by hxh>
	bool OnGameScenePlay(const void *pBuffer, WORD wDataSize);

	//<add by hxh 20160720>
	void showMoveingLabelTips(const char* str);

	cocos2d::Size					size;// 设计分辨率大小<by hxh>
	ViewMenuLayer					*m_menuLayergic;
	CardManager*					m_pCardManager;									//牌管理类
	OperaLayer *                    m_OperaLayer;
	ChipManager*                    m_chipManager;                                   //筹码管理类
	Sprite *SpriteGameStare;        //游戏开始，可以下注标签
	Sprite *SpriteGameTime;         //时间标志
	Sprite *SpriteChangeBanker;     //切换庄家标志<by hxh>
	OperaLayer *operlayer;

	CC_SYNTHESIZE(CardTableLayer*, m_cardTableLayer, CardTableLayer);

private:
	//--手牌
	BYTE						m_handCardData[GAME_PLAYER][MAX_COUNT];						//扑克列表
	WORD						m_handCardNum[GAME_PLAYER];
	//--底牌
	BYTE						m_BackCardData[GAME_PLAYER];								//底牌
	//--桌面最大牌
	BYTE						m_outCardData[MAX_COUNT];
	WORD						m_outCardNum;
	WORD						m_beilv;
	BYTE						m_bLastCallScore;													
	WORD						m_wCurrentUser;												//当前玩家
	WORD						m_wTempUser;												//上家
	WORD						m_nBankerUser;												//地主
	bool						m_wOpenUser[GAME_PLAYER];									//明牌玩家
	bool                        m_isMeBanker;// 是否坐庄<by hxh>
	int                         m_ChangeBanker;// 切换庄家标志<by hxh>
	WORD						m_baseScore;												//游戏基础积分
	bool						m_isOut;
	//个人信息
protected:
	LONGLONG						m_lPlayBetScore;					//玩家最大下注
	LONGLONG						m_lPlayFreeSocre;					//玩家自由积分

	//下注数
protected:
	LONGLONG						m_lAllBet[AREA_MAX];				//总下注
	LONGLONG						m_lPlayBet[AREA_MAX];				//玩家下注
	LONGLONG						m_lPlayScore[AREA_MAX];				//玩家输赢

	//庄家信息
protected:
	WORD							m_wBankerUser;						//当前庄家
	LONGLONG						m_lBankerScore;						//庄家积分
	//LONGLONG						m_lBankerWinScore;					//庄家赢分
	//WORD							m_wBankerTime;						//庄家局数
	//bool							m_bEnableSysBanker;					//系统做庄

	//用户下注
	LONGLONG m_lBetScore[8];							//加注数目

	//游戏结束
protected:
	CMD_S_GameEnd                   m_GameEnd;                              //游戏结束<by hxh>
	//扑克信息
	BYTE							m_ucHandCardData[3];					//闲家手牌<by hxh>
	BYTE							m_ucHandCardData1[3];					//庄家手牌<by hxh>

	BYTE							m_cbTableCardArray[2][3];				//桌面扑克

	//庄家信息
	//LONGLONG						m_plBankerScore;						//庄家成绩
	//LONGLONG						m_plBankerTotallScore;					//庄家成绩
	//int								m_pnBankerTime;						//做庄次数

	//玩家成绩
	//LONGLONG						m_plPlayScore[AREA_MAX];				//玩家成绩
	//LONGLONG						m_plPlayAllScore;						//玩家成绩

	//全局信息
	//LONGLONG						m_plRevenue;							//游戏税收
};		
#endif
