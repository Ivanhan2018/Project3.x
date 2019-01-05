//  [3/19/2014 wucan]
#ifndef		DDZ___CC_SCENELOGIC_WUCAN__
#define		DDZ___CC_SCENELOGIC_WUCAN__
#include "cocos2d.h"
#include "DDZSceneData.h"
#include "EntityMgr.h"

USING_NS_CC;
class DDZSceneView;
class DDZSceneLogic:public Node
{
public:
	CREATE_FUNC(DDZSceneLogic);
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

	CC_SYNTHESIZE(DDZSceneView*,m_DDZSceneView,DDZSceneView);

	//	调位置专用
	static void studioUI(Node* pParent, Node* pChild);


	//	获取游戏状态
	DDZGame_State getGameState() const;


   //游戏命令
    bool gameMain(WORD wSubCmdID, const void *pData, WORD wDataSize);


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
		int nCardValue = DDZSceneLogic::GetCardValue(nCardData);
		int nCardColor = DDZSceneLogic::GetCardColor(nCardData);

		// 转换
		if (nCardColor == 0x40) {
			return nCardValue + 2;
		}
		return (nCardValue <= 2) ? (nCardValue + 13) : nCardValue;
	}
	static int	switchViewChairID(unsigned int nChairID); 
	BYTE							m_BackCardData[DDZGAME_PLAYER];								//底牌

private:
	tagUserData* getMeUserDate() { return EntityMgr::instance()->roomFrame()->getMeUserItem()->getUserData(); }
	WORD getMeChairID(){return getMeUserDate()->wChairID;}
	WORD getMeChairID1(){
		WORD meChairID = EntityMgr::instance()->getDispatch()->m_wChairID;
		return meChairID;
	}
	void sendSeverData(WORD wSubCmdID);
	void sendSeverData(WORD wSubCmdID,void* pData,WORD wDataSize);
	void sendFrameData( WORD wSubCmdID,void* pData,WORD wDataSize);
	void checkNet(float ft);
	void onLinkResult(Object* obj);
	void onUpdateScoreAndGoldEgg(Object* obj);
	int getUserRealChairID(DWORD userID);
	//功能函数
private:
    //游戏开始
    bool OnSubGameStart(const void * pdata, WORD wDataSize);
#if 0
    //用户叫地主
    bool OnSubCallBanker(const void * pdata, WORD wDataSize);
    //用户抢地主
    bool OnSubRodBanker(const void * pdata, WORD wDataSize);
    //加倍信息
    bool OnSubDouble(const void * pdata, WORD wDataSize);
    //用户明牌
    bool OnSubValidCard(const void * pdata, WORD wDataSize);
    //庄家信息
    bool OnSubBankerInfo(const void * pdata, WORD wDataSize);
    //用户出牌
    bool OnSubOutCard(const void * pdata, WORD wDataSize);
    //用户放弃
    bool OnSubPassCard(const void * pdata, WORD wDataSize);
    //出牌开始
    bool OnSubOutCardStart(const void * pdata, WORD wDataSize);
    //游戏结束
    bool OnSubGameConclude(const void * pdata, WORD wDataSize);
    //设置基数
    bool OnSubSetBaseScore(const void * pdata, WORD wDataSize);
    //新的地主
    bool OnSubNewBank(const void * pdata, WORD wDataSize);
    //用户托管
    bool OnSubTrustee(const void * pdata, WORD wDataSize);
#endif
	void onUserStart(Object *pSender);
	bool onUserStartView(const void * pBuffer, WORD wDataSize);
	bool onUserBEeiLv(const void * pBuffer, WORD wDataSize);
	bool onUserAdd(const void * pBuffer, WORD wDataSize);
	bool onSubSendCard(const void * pBuffer, WORD wDataSize);
	bool onSubOpenCard(const void * pBuffer, WORD wDataSize);
	bool onSubLandScore(const void * pBuffer, WORD wDataSize);
	bool onSubGameStart(const void * pBuffer, WORD wDataSize);
	bool onSubOutCard(const void * pBuffer, WORD wDataSize);
	bool onSubPassCard(const void * pBuffer, WORD wDataSize);
		//游戏结束;
	bool onSubGameEnd(const void * pBuffer, WORD wDataSize);
	bool onUserOnLine(const void * pBuffer, WORD wDataSize);
	bool onUserChat( const void * pBuffer, WORD wDataSize );
	bool onUserExpression(const void * pBuffer, WORD wDataSize);
	bool onRoomLimit(const void * pBuffer, WORD wDataSize);

	//add by zengxin 20161102 叫地主
	bool onCallLord(const void * pBuffer, WORD wDataSize);

	//add by zengxin 20161102 抢地主
	bool onRobLord(const void * pBuffer, WORD wDataSize);

	//add by zengxin 20161008  斗地主恢复对局处理   打牌阶段
	bool OnGameScenePlay(const void *pBuffer, WORD wDataSize);

	//add by zengxin 20161008 斗地主恢复对局   叫地主阶段
	bool OnGameSceneCallLord(const void *pBuffer, WORD wDataSize);

	//add by zengxin 20161104 斗地主恢复对局   抢地主阶段
	bool OnGameSceneRobLord(const void *pBuffer, WORD wDataSize);

	// <add by zengxin 20160923>  用户为第一个叫地主用户
	void showMyFirstJiaoDiZhuBtnViews(float dt);

public:
	//--手牌
	BYTE						m_handCardData[DDZGAME_PLAYER][MAX_COUNT];			//	扑克列表
	WORD						m_handCardNum[DDZGAME_PLAYER];						//	手牌数目
	WORD						m_nBankerUser;										//	地主
private:
	//--底牌
	//--桌面最大牌
	BYTE							m_outCardData[MAX_COUNT];
	WORD						m_outCardNum;											//出牌数目
	WORD						m_beilv;
	BYTE						m_bLastCallScore;													
	WORD						m_wCurrentUser;															//当前玩家
	WORD						m_wStartUser;											//	开始玩家
	WORD						m_wTempUser;																//上家
	bool							m_wOpenUser[DDZGAME_PLAYER];									//明牌玩家
	DDZGame_State				m_eGameState;																//游戏状态
	WORD						m_baseScore;																//游戏基础积分
	bool							m_isOut;																		//游戏已经出牌了，用于加倍

	bool						m_isCallLord;											//当前局是否有人叫过地主, 用来控制展示叫地主还是抢地主按钮

	//各种状态的操作步长
	//时间信息
	BYTE							m_cbTimeCallBanker;					//叫地主时间
	BYTE							m_cbTimeRodBanker;					//抢地主时间
	BYTE							m_cbTimeOutCard;						//出牌时间
	BYTE							m_cbTimeStartGame;					//开始时间
	BYTE							m_cbTimeHeadOutCard;					//首出时间

	LONG							m_lCellScore;							//基础积分
};		
#endif
