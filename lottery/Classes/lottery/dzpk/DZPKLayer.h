#ifndef __DZPKLAYER_SCENE_H__
#define __DZPKLAYER_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "EntityMgr.h"
#include "DZPKOperaLayer.h"
#include "RoomLight.h"//by hxh
#include "trusteeship.h"
#include "RoyalFlush.h"
#include "CMD_DZPK.h"
#include "UserDZPK.h"
#include "DZPKChipPot.h"

// add by hxh 20160919
// #define USE_PRINTTEST
#define USE_PRINTSCORE
#define MAX_TEST_NUM (2)
#define MAX_SCORE_NUM (5)

USING_NS_CC;

enum MyTage
{
	StartTag=0,                                      //开始
	LeaveTag=1,                                      //离开
	ExitTag=2,                                       //退出
	ApTage_03,
	ApTage_04,
    DZPKLayerTag=10,                                 //游戏主场景<by hxh>
};

/*
    游戏主场景
*/
class DZPKLayer : public cocos2d::Layer
{
public:
	DZPKLayer();
	~DZPKLayer();
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	static cocos2d::Scene* scene();
	CREATE_FUNC(DZPKLayer);
public:
	void LoadRes();//dzpk-jhy
	static void dzpk_ToPortrait();//dzpk-jhy
	static void dzpk_ToLandscape();//dzpk-jhy
	static void ExitDZPK();

	// 椅子号转换
	static unsigned int switchViewChairID(unsigned int nChairID);

	//	调位置专用
	static void studioUI(Node* pParent, Node* pChild);

	//	弹出提示调试语
	static void showDebugTips(string sText);

	//	初始化数据
	void initData();

	//	初始化玩家视图
	void initPlayerView();

	//	重置数据
	void resetData();

	//	刷新玩家信息
	void upDatePlayer(tagUserData sData);

	//  更新玩家金币<by hxh>
	void updatePlayerGold(WORD iViewID,LONGLONG lAddScore);

	//	显示D玩家
	void showDPlayer(int nViewID);

	//	显示盲注玩家
	void showBlindPlayer(EDZPKBlindType eType, int nViewID);

	//	隐藏盲注玩家
	void hideBlindPlayer();

	//	显示玩家行动标识
	void onPlayerBehavior(int nViewID, int nType);

	//	取消倒计时
	void cancelCountdown(int nViewID);

	//	下注
	void dealBet(int nViewID, LONGLONG llValue);

	//	汇总
	void dealCollect(int nViewID);

	//	分发
	void dealDistribute(int nViewID, LONGLONG llValue);

	/*
	**	@brief	获取玩家信息
	**	@lnUserID	玩家ID
	**	@return	返回该玩家信息，如果没有找到则返回NULL
	*/	
	UserDZPK* getPlayer(LONGLONG llUserID);

	//	发牌动画
	void aniSendCard();

	//	显示所有玩家手牌
	void showAllPlayerCard(char cHandCard[][2]);

	//	将桌面所有卡牌变灰暗
	void setAllCardDark(char cHandCard[][2]);

	//	显示赢家高亮卡牌
	void showCardHighLight(int nViewID, char cLastCard[]);

	//	处理玩家准备阶段表现
	void dealPlayerReady();

	void AddObject();
	void RemoveObject();
	void winAnimate(int iViewID,BYTE cardtype);
	void onUserStart(Object *pSender);//<add by hxh 20160825>
	void onLoginConnectFaild(Object* obj);
	void onShutNetwork(Object* obj);
	void OtherStarya(Object *obj);
	void onUpdateTable(Object* obj);
	void onMessageGame(Object* obj);
	void onMessageScene(Object* obj);
	void onMessageMatchPlayer(Object* obj);
	void onLoginRoomFailed(Object* obj);
	void onSitFaild(Object* obj);					    //坐下失败<by hxh>
	bool onSceneMessage(unsigned char cbStation,const void* pBuffer,unsigned short wDataSize);
	bool onGameMessage(unsigned short wSubCmdID, const void* pBuffer, unsigned short wDataSize);

	//消息处理
protected:
	//空闲状态<by hxh>
	bool OnGameSceneFree(const void *pBuffer, WORD wDataSize);
	//游戏状态<by hxh>
	bool OnGameScenePlay(const void *pBuffer, WORD wDataSize);
	//游戏开始
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//用户加注
	bool OnSubAddScore(const void * pBuffer, WORD wDataSize);
	//用户放弃
	bool OnSubGiveUp(const void * pBuffer, WORD wDataSize);
	//发牌消息
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//游戏结束
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//开牌消息
	bool OnSubOpenCard(const void * pBuffer, WORD wDataSize);
	
	Menu * createExitMenu();// 【退出】<by hxh>
	Menu * createStartLeaveMenu();// 【开始】、【离开】<by hxh>

	void OnActionThink(int iViewID);		//玩家下注中...<by hxh>

	void updateTest(Object* obj);		    //<by hxh>
    void setScore1(LONGLONG lCellScore);		//设置盲注<by hxh>
    void setScore2(LONGLONG lTurnMaxScore);		//设置最大下注<by hxh>
    void setScore3(LONGLONG lTurnLessScore);    //设置最小下注<by hxh>
    void setScore4(LONGLONG lAddLessScore);    //设置加最小注<by hxh>
	
	void showOperaBtns();//显示下注类型按钮<by hxh>

	// 删除结算标签
	void CloseWinLose(float t);
	// 玩家超时没有准备
	void CloseRoundOpera(float t);	
	// 【开始】、【离开】、【退出】操作
	void LeaveGameResume(Object* obj);
	// 开始下一局
	void restartGame(float t);																

	tagUserData* getMeUserDate() { return EntityMgr::instance()->roomFrame()->getMeUserItem()->getUserData(); }
	WORD getMeChairID(){return EntityMgr::instance()->getDispatch()->m_wChairID;};

	// add by hxh 20160919
#ifdef USE_PRINTTEST
	void PrintTest(Object *obj);
	void PrintTest_callback_once(float t);
	void PrintTest1(Object *obj);
	void PrintTest1_callback_once(float t);
#endif

	// add by hxh 20160919
#ifdef USE_PRINTSCORE
	void PrintScore1(Object *obj);
	void PrintScore1_callback_once(float t);
#endif

	//加注信息
public:
	LONGLONG						m_lCellScore;						//单元下注,这里指小盲注<by hxh>
	LONGLONG						m_lTurnLessScore;					//最小下注
	LONGLONG						m_lTurnMaxScore;					//最大下注
	LONGLONG						m_lAddLessScore;					//最小加注
	LONGLONG						m_lCenterScore;						//中心筹码

	cocos2d::Size                            m_WinSize;                                      //设计分辨率
	RoomLight*                      m_roomLight;                                    //灯光<by hxh>
	DZPKOperaLayer*					m_pOperaLayer;									//按钮操作面板
	DZPKChipPot*					m_pChipPot;										//	筹码池
	Menu*							m_StartLeave;									//【开始】、【离开】
	DZPKCommunityCards*				m_pCommunityCards;								//	公共牌
	
#ifdef USE_PRINTSCORE
	LabelTTF *m_Score[MAX_SCORE_NUM];
#endif

	//Trusteeship*					m_pTrusteeship;									//托管界面
	RoyalFlush*						m_pRoyalFlush;									//皇家同花顺动画

	map<int, UserDZPK*> m_mPlayerList;	//	玩家列表

};

#endif  // __DZPKLAYER_SCENE_H__