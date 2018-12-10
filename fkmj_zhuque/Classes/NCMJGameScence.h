#pragma once
#include "cocos2d.h"
#include "FvSingleton.h"
#include "WidgetDefine.h"
#include "GameBase.h"
#include "GameDefine.h"
#include "TimeNode.h"


class NCMJPlayer;
struct CMD_S_GameEnd;
struct  CMD_WeaveItem;
struct  NCMJGameRecord;

class NCMJGameScence
	:public GameBase
	,public FvSingleton<NCMJGameScence> 
{
public:
	const static int KIND_ID = 320;
	const static int VERSION_SERVER	= PROCESS_VERSION(6,0,3);				//程序版本
	const static int VERSION_CLIENT	= PROCESS_VERSION(6,0,3);	
	const static int MAX_PLAYER = 4;

	enum 
	{
		NCMJ_STATE_NULL,
		NCMJ_STATE_READY,
		NCMJ_STATE_XIAO_HU,
		NCMJ_STATE_PLAYING,
	};
public:
	NCMJGameScence();
	~NCMJGameScence();
public:
	virtual bool init();
	virtual bool IsInGame();

	void initPrivate();
public:
	void EnterScence();
	void HideAll();
	void defaultState();
	void defaultPlayerActionState();
	void showSaiZi(unsigned int iValue);
	std::string getStringHuRight(dword kValue);
	std::string getStringGang(int nChairID,CMD_WeaveItem* pWeave,int iWeaveCout);
	void showUserAction(int iUserAction);
	void setCurrentPlayer(int iCurrentPlayer,int iUserAction);
	void showJieSuanInfo(CMD_S_GameEnd* pGameEnd);
	void setJieSuanNiaoCard(cocos2d::Node* pNode,BYTE* pNiaoCard,BYTE cbCardNum);
	void setGameEndNiaoCardAni(cocos2d::Node* pNode,BYTE* pNiaoCard,BYTE cbCardNum);
public:
	void initButton();
	void NCMJButton_Ready(cocos2d::Ref*,WidgetUserInfo*);
	void NCMJButton_GuoAction(cocos2d::Ref*,WidgetUserInfo*);
	void NCMJButton_GangAction(cocos2d::Ref*,WidgetUserInfo*);
	void NCMJButton_PengAction(cocos2d::Ref*,WidgetUserInfo*);
	void NCMJButton_ChiAction(cocos2d::Ref*,WidgetUserInfo*);
	void NCMJButton_HuAction(cocos2d::Ref*,WidgetUserInfo*);
	void NCMJButton_XiaoHuAction(cocos2d::Ref*,WidgetUserInfo*);
	void NCMJButtonAction_ShowNext(cocos2d::Ref*,WidgetUserInfo*);
	void NCMJButtonAction_ShowCard(cocos2d::Ref*,WidgetUserInfo*);

	void NCMJButton_JieSuanShare(cocos2d::Ref*,WidgetUserInfo*);
	void NCMJButton_JieSuanStart(cocos2d::Ref*,WidgetUserInfo*);

	void NCMJButton_TalkBegin(cocos2d::Ref*,WidgetUserInfo*);
	void endButtonTalk();
	void NCMJButton_TalkEnd(cocos2d::Ref*,WidgetUserInfo*);
private:
	virtual bool OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize);
	void OnFreeScence(void* data, int dataSize);
	void OnPlayScence(void* data, int dataSize);
public:
	NCMJPlayer* getPlayerByChairID(int iChairID);
	virtual GamePlayer* CreatePlayer(IClientUserItem * pIClientUserItem);
	virtual void DeletePlayer(GamePlayer* pPlayer);
	virtual void upSelfPlayerInfo();
	virtual void OnGFGameClose(int iExitCode);
	virtual void OnEventUserStatus(GamePlayer * pPlayer);

	//私人场
public:
	void defaultPrivateState();
	virtual void OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo);
	virtual void OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo);
	virtual void OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo);
	void NCMJButton_WeiXinFriend(cocos2d::Ref*,WidgetUserInfo*);
	void NCMJButton_WeiXinImagic(cocos2d::Ref*,WidgetUserInfo*);
	void ButtonPlayerHeadClick(cocos2d::Ref*,WidgetUserInfo* pUserInfo);
public:
	void OnMasterHandCard( const void *pBuffer, WORD wDataSize );
	void OnMasterLeftCard( const void *pBuffer, WORD wDataSize );
	void updateUserRight();
	void showMater();
	void initMaster();
	void defaultMaster();
	void setMasterCheakCard(BYTE cbCard);
	void NCMJButton_Master(cocos2d::Ref*,WidgetUserInfo*);
	void Button_MasterClose(cocos2d::Ref*,WidgetUserInfo*);
	void NCMJ_MASTER_LEFTCARD(cocos2d::Ref*,WidgetUserInfo*);
public:
	void initNet();

	void OnSubGameStart(const void * pBuffer, word wDataSize);
	//用户出牌
	void OnSubOutCard(const void * pBuffer, WORD wDataSize);
	//发牌消息
	void OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//操作提示
	void OnSubOperateNotify(const void * pBuffer, WORD wDataSize);
	//操作结果
	void OnSubOperateResult(const void * pBuffer, WORD wDataSize);
	//游戏结束
	void OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//剩余卡
	void OnSubGameEndLastCard(const void * pBuffer, WORD wDataSize);
	//用户托管
	void OnSubTrustee(const void * pBuffer,WORD wDataSize);
	//吃胡消息
	void OnSubUserChiHu( const void *pBuffer,WORD wDataSize );
	//杠得分
	void OnSubGangScore( const void *pBuffer, WORD wDataSize );
public:
	void Command_PlaceBet(int iArea,int iBetScore);
	void SendOutCard(cocos2d::Node* pCard);
public:
	void initTouch();
	bool ccTouchBegan(cocos2d::Vec2 kPos);
	void ccTouchMoved(cocos2d::Vec2 kPos);
	void ccTouchEnded(cocos2d::Vec2 kPos);

public:
	void setGameState(int nState);
	int getGameState();
	void setPlayCount(int nCount);
	int getPlayCount();
	void setPrivateInfo(CMD_GF_Private_Room_Info* pNetInfo);

public:
	void initRecord();
	void defaultRecordState();
	bool StartRecord(datastream kDataStream);
	void NextRecordAction();
	virtual void onGPAccountInfoHttpIP(TAG_UserIndividual* pNetInfo);

	void Button_GameRecordPlay(cocos2d::Ref*,WidgetUserInfo*);
	void Button_GameRecordPase(cocos2d::Ref*,WidgetUserInfo*);
	void Button_GameRecordLeft(cocos2d::Ref*,WidgetUserInfo*);
	void Button_GameRecordRight(cocos2d::Ref*,WidgetUserInfo*);
	void Button_GameRecordExit(cocos2d::Ref*,WidgetUserInfo*);
public:
	void initSetButton();
	void SetBarSider1(cocos2d::Ref*,WidgetUserInfo*);
	void SetBarSider2(cocos2d::Ref*,WidgetUserInfo*);
	void Button_HideSet(cocos2d::Ref*,WidgetUserInfo*);
	void Button_GameSet(cocos2d::Ref* pNode,WidgetUserInfo* pUserInfo);
public:
	void Button_TalkDefine(cocos2d::Ref*,WidgetUserInfo*);
	void Button_Send_TalkStr(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButton_BiaoQing(cocos2d::Ref*,WidgetUserInfo*);
protected:
	int							m_iBankerUser;						//庄家用户
	int							m_iCurrentUser;						//当前用户
	int							m_iUserAction;						//玩家动作

	NCMJPlayer*					m_pLocal;
	NCMJPlayer*					m_pPlayer[MAX_PLAYER];

	cocos2d::Node*				m_pTouchCardNode;
	cocos2d::Vec2				m_kTouchSrcPos;

	int                         m_nGameState;
	int                         m_nPlayCount;

	word						m_wRecordSelfChairID;
	int							m_iActRecordIdex;
	int							m_RecordTimeID;
	float						m_RecordTime;
	NCMJGameRecord*				m_pGameRecord;
	CMD_GF_Private_Room_Info	m_kPrivateRoomInfo;
};