#pragma once
#include "cocos2d.h"
#include "FvSingleton.h"
#include "WidgetDefine.h"
#include "GameBase.h"
#include "GameDefine.h"
#include "TimeNode.h"


class NNPlayer;
struct CMD_S_GameStart;

class NNGameScence
	:public GameBase
	,public FvSingleton<NNGameScence> 
{
public:
	const static int KIND_ID = 27;
	const static int VERSION_SERVER	= PROCESS_VERSION(6,0,3);				//程序版本
	const static int VERSION_CLIENT	= PROCESS_VERSION(6,0,3);	
	const static int MAX_PLAYER = 5;

	enum 
	{
		GAME_OPTION_RULE_SHOW_WIN_LOSE1,
	};
public:
	NNGameScence();
	~NNGameScence();
public:
	virtual bool init();
	virtual bool IsInGame();

	void initPrivate();
public:
	void EnterScence();
	void HideAll();
	void defaultState();

	bool isSelfBanker();
	bool isSelfInGame();
public:
	void initButton();
	void NNButton_Ready(cocos2d::Ref*,WidgetUserInfo*);
	void NNButton_QiangZhuang(cocos2d::Ref*,WidgetUserInfo*);
	void NNButton_BuQiang(cocos2d::Ref*,WidgetUserInfo*);
	void NNButton_Bet(cocos2d::Ref*,WidgetUserInfo*);
	void NNButton_YouNiu(cocos2d::Ref*,WidgetUserInfo*);
	void NNButton_MeiYou(cocos2d::Ref*,WidgetUserInfo*);

	void NNButton_SendCardEnd(cocos2d::Ref*,WidgetUserInfo*);
	void NNButton_CheakZhuangEnd(cocos2d::Ref*,WidgetUserInfo*);

	void HNMJButton_TalkBegin(cocos2d::Ref*,WidgetUserInfo*);
	void endButtonTalk();
	void HNMJButton_TalkEnd(cocos2d::Ref*,WidgetUserInfo*);
private:
	virtual bool OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize);
	void OnFreeScence(void* data, int dataSize);
	void OnPlayScence(void* data, int dataSize);
	void OnCallBankerScence(void* data, int dataSize);
	void OnBetScoreScence(void* data, int dataSize);

	//消息处理
protected:
	void initNet();
	//发牌
	void OnSubAllCard(void * pBuffer, int wDataSize);
	//用户叫庄
	void OnSubCallBanker(void * pBuffer, int wDataSize);
	//游戏开始
	void OnSubGameStart(void * pBuffer, int wDataSize);
	//用户加注
	void OnSubAddScore(void * pBuffer, int wDataSize);
	//发牌消息
	void OnSubSendCard(void * pBuffer, int wDataSize);
	//用户摊牌
	void OnSubOpenCard(void * pBuffer, int wDataSize);
	//用户强退
	void OnSubPlayerExit(void * pBuffer, int wDataSize);
	//游戏结束
	void OnSubGameEnd(void * pBuffer, int wDataSize);
	//控制
	void OnSubAdminControl(void * pBuffer, int wDataSize);
public:
	void initTouch();
	bool ccTouchBegan(cocos2d::Vec2 kPos);
	void ccTouchMoved(cocos2d::Vec2 kPos);
	void ccTouchEnded(cocos2d::Vec2 kPos);
public:
	NNPlayer* getPlayerByChairID(int iChairID);
	virtual GamePlayer* CreatePlayer(IClientUserItem * pIClientUserItem);
	virtual void DeletePlayer(GamePlayer* pPlayer);
	virtual void upSelfPlayerInfo();
	virtual void OnGFGameClose(int iExitCode);
	virtual void OnEventUserStatus(GamePlayer * pPlayer);
	virtual void LocalPlayerEnter();

	//私人场
public:
	void defaultPrivateState();
	void setPrivateInfo(CMD_GF_Private_Room_Info* pNetInfo);
	virtual void OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo);
	virtual void OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo);
	virtual void OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo);
	void XZDDButton_WeiXinFriend(cocos2d::Ref*,WidgetUserInfo*);
	void XZDDButton_WeiXinImagic(cocos2d::Ref*,WidgetUserInfo*);
	void ButtonPlayerHeadClick(cocos2d::Ref*,WidgetUserInfo* pUserInfo);
public:
	void showBoxNumNode(bool bShow);
	void updateUpCardList(cocos2d::Node* pCard);
	void updateBoxNum();
	void checkZhuang(CMD_S_GameStart * pNetInfo);
	void showJieSuanInfo(LONGLONG lScore);
protected:
	NNPlayer*					m_pLocal;
	NNPlayer*					m_pPlayer[MAX_PLAYER];
	CMD_GF_Private_Room_Info    m_kRoomInfo;

	cocos2d::Node*				m_pTouchCardNode;

	word						m_wRecordSelfChairID;
	int							m_iActRecordIdex;
	int							m_RecordTimeID;
	float						m_RecordTime;

	BYTE						m_cbPlayStatus[MAX_PLAYER];			//游戏状态
	BYTE						m_cbBankerChairID;

	std::vector<cocos2d::Node*>     m_kUpCardList;
};