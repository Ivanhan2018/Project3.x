#pragma once
#include "cocos2d.h"
#include "FvSingleton.h"
#include "WidgetDefine.h"
#include "GameBase.h"
#include "GameDefine.h"


class HNMJPlayer;
struct CMD_S_GameEnd;
struct  CMD_WeaveItem;
struct  HNMJGameRecord;

class HNMJGameScence
	:public GameBase
	,public FvSingleton<HNMJGameScence> 
{
public:
	const static int KIND_ID = 303;
	const static int VERSION_SERVER	= PROCESS_VERSION(6,0,3);				//程序版本
	const static int VERSION_CLIENT	= PROCESS_VERSION(6,0,3);	
	const static int MAX_PLAYER = 4;

	enum 
	{
		HNMJ_STATE_NULL,
		HNMJ_STATE_READY,
		HNMJ_STATE_XIAO_HU,
		HNMJ_STATE_PLAYING,
	};

	enum 
	{
		GAME_OPTION_TYPE_HN = 1,
		GAME_OPTION_TYPE_HZ,
		GAME_OPTION_TYPE_THJ,
	};
	enum 
	{
		GAME_OPTION_RULE_AUTO_CARD = 1,
		GAME_OPTION_RULE_SHOW_WIN_LOSE1,
		GAME_OPTION_RULE_AUTO_HU ,
		GAME_OPTION_RULE_SHOW_WIN_LOSE_ZERO,

		GAME_OPTION_RULE_HIDE_GAMETYPE,
		GAME_OPTION_RULE_HIDE_OTHER,

		GAME_OPTION_RULE_HIDE_GUO,
		GAME_OPTION_RULE_PRIVATAEND_RETURN_HOME,

		GAME_OPTION_RULE_SHOW_TING_CARD,
		GAME_OPTION_RULE_USERID_ADD,
		GAME_OPTION_RULE_NIAO_DIRECT,
		GAME_OPTION_RULE_NIAO_ANIM_2,
		GAME_OPTION_RULE_SHOW_LAST_CARD,
		GAME_OPTION_RULE_ONLY_CREATE,
		GAME_OPTION_TING_CS_GANG,
		GAME_OPTION_SHOW_STAR_VALUE,
		GAME_OPTION_RULE_GPS_COLOR,
		GAME_OPTION_RULE_SHOW_EXIT_AND_SAVE,
		GAME_OPTION_RULE_NIAO_FLY,
		GAME_OPTION_RULE_SHOW_TIME,
		GAME_OPTION_SHOW_PRIVATE_END_INFO,
	};
public:
	HNMJGameScence();
	HNMJGameScence(int iGameType,unsigned int iOption);
	~HNMJGameScence();
public:
	virtual bool init();

	void initPrivate();
	virtual bool IsInGame();
public:
	void EnterScence();
	void HideAll();
	void defaultState();
	bool isHZ();
	bool isTHJ();
	void defaultPlayerActionState();
	void showSaiZi(unsigned int iValue);
	std::string getStringHuRight(dword kValue);
	std::string getStringGang(int nChairID,CMD_WeaveItem* pWeave,int iWeaveCout);
	void remmoveAutoAction();
	void AutoCard();
	void HuPaiAutoAction();
	void OnPlayWaring();
	void removeWaringSound();
	void upSelfFreeReadState();
	void setCurrentPlayer(int iCurrentPlayer,DWORD iUserAction,int nActionCard = 0);
	void setCSgangAction(cocos2d::Node* pNode,DWORD cbActionCard,DWORD cbActionMask);
	void showCSGangAction(int iResumeUser,BYTE cbActionCard1,DWORD cbActionMask1,BYTE cbActionCard2,DWORD cbActionMask2);
	void checkActionBtns(DWORD iUserAction,int nActionCard);
	void checkActionBtn(bool icheck,const std::string&BtnName,int nCard,cocos2d::Vec2& kPos);
	void setActionBtnCard(cocos2d::Node* pActionBtnCard,int nCard,std::string kImagicFront);
	void showJieSuanInfo(CMD_S_GameEnd* pGameEnd);
	void setJieSuanNiaoCard(cocos2d::Node* pNode,BYTE* pNiaoCard,BYTE cbCardNum);
	void setGameEndNiaoCardAni1(cocos2d::Node* pNode,BYTE* pNiaoCard,BYTE cbCardNum,float& delayTime);
	void setGameEndNiaoCardAni2(cocos2d::Node* pNode,BYTE* pNiaoCard,BYTE cbCardNum,float& delayTime);
	void setGameEndNiaoCardAniFly(CMD_S_GameEnd* pGameEnd,float& delayTime);
	

	void showChiCheckNode(cocos2d::Node* pCheckNode, BYTE cbAction,BYTE cbCard,int nChiCount);
	void sendOperateCMD(int nCode,int nCard,int iCSGangIdex);
public:
	void initButton();
	void HNMJButton_Ready(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButton_GuoAction(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButton_GangAction(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButtonCS_GangCS(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButtonCS_BuAction(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButton_PengAction(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButton_ChiAction(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButton_HuAction(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButton_XiaoHuAction(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButton_GangCS(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButton_BuAction(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButton_ChiCardList(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButton_QuXiaoTuoGuan(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButton_HaiDiYao(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButton_HaiDiGuo(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButton_GangActionNode(cocos2d::Ref*,WidgetUserInfo*);
	
	void HNMJButtonAction_ShowNext(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButtonAction_ShowCard(cocos2d::Ref*,WidgetUserInfo*);

	void HNMJButton_JieSuanShare(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButton_JieSuanStart(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButton_JieSuanClose(cocos2d::Ref*,WidgetUserInfo*);

	void HNMJButton_TalkBegin(cocos2d::Ref*,WidgetUserInfo*);
	void endButtonTalk();
	void HNMJButton_TalkEnd(cocos2d::Ref*,WidgetUserInfo*);

	void HNMJButton_PiaoBei(cocos2d::Ref*,WidgetUserInfo*);

private:
	virtual bool OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize);
	void OnFreeScence(void* data, int dataSize);
	void OnPlayScence(void* data, int dataSize);
public:
	HNMJPlayer* getPlayerByChairID(int iChairID);
	virtual GamePlayer* CreatePlayer(IClientUserItem * pIClientUserItem);
	virtual void DeletePlayer(GamePlayer* pPlayer);
	virtual void upSelfPlayerInfo();
	virtual void OnGFGameClose(int iExitCode);
	virtual void OnEventUserStatus(GamePlayer * pPlayer);
	virtual bool BackKey();
	//私人场
public:
	void defaultPrivateState();
	virtual void OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo);
	virtual void OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo);
	virtual void OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo);
	virtual void Button_WeiXinFriend(cocos2d::Ref*,WidgetUserInfo*);
	virtual void onWeiXinShareFriend();
	void Button_WeiXinImagic(cocos2d::Ref*,WidgetUserInfo*);
	void ButtonPlayerHeadClick(cocos2d::Ref*,WidgetUserInfo* pUserInfo);
public:
	void initNet();
	void setGangNodeInfo(cocos2d::Node* pNode,BYTE pCardData[4],BYTE cbCardCout,int iAction);
	void setGangInfo(BYTE pCardData[4],BYTE cbCardCout);

	void OnSubGameStart(const void * pBuffer, word wDataSize);
	//用户出牌
	void OnSubOutCard(const void * pBuffer, WORD wDataSize);
	//用户出牌
	void OnSubOutCardCSGang(const void * pBuffer, WORD wDataSize);
	//发牌消息
	void OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//长沙杠补牌
	void OnSubSendCard_CSGang(const void * pBuffer, WORD wDataSize);
	//操作提示
	void OnSubOperateNotify(const void * pBuffer, WORD wDataSize);
	//操作结果
	void OnSubOperateResult(const void * pBuffer, WORD wDataSize);
	//游戏结束
	void OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//剩余牌
	void OnSubGameEndLastCard(const void * pBuffer, WORD wDataSize);
	//用户托管
	void OnSubTrustee(const void * pBuffer,WORD wDataSize);
	//吃胡消息
	void OnSubUserChiHu( const void *pBuffer,WORD wDataSize );
	//杠得分
	void OnSubGangScore( const void *pBuffer, WORD wDataSize );
	void OnSubXiaoHu(const void *pBuffer, WORD wDataSize);
	//海底牌
	void OnSubUseHaiDiCard(const void *pBuffer, WORD wDataSize);
	//海底牌
	void OnSubPiaoNotice(const void *pBuffer, WORD wDataSize);
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
	void getGameRuleStr(std::vector<std::string>& kRuleList,dword	dwGameRuleIdex);
	void upDateTime();
	void setPrivateInfo(CMD_GF_Private_Room_Info* pNetInfo);
	void setOutCardWaiteTime(int iOutCardWaiteTime);
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
	void OnMasterHandCard( const void *pBuffer, WORD wDataSize );
	void OnMasterLeftCard( const void *pBuffer, WORD wDataSize );
	void updateUserRight();
	void showMater();
	void initMaster();
	void defaultMaster(bool bRestZhaNiao = false);
	void setMasterCheakCard(BYTE cbCard);
	void HNMJButton_Master(cocos2d::Ref*,WidgetUserInfo*);
	void Button_MasterClose(cocos2d::Ref*,WidgetUserInfo*);
	void NHMJ_MASTER_LEFTCARD(cocos2d::Ref*,WidgetUserInfo*);
	void MasterZhaNiao(cocos2d::Ref*,WidgetUserInfo*);
public:
	void Button_TalkDefine(cocos2d::Ref*,WidgetUserInfo*);
	void Button_Send_TalkStr(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButton_BiaoQing(cocos2d::Ref*,WidgetUserInfo*);
public:
	bool OnXiaoHuEnd(int nChairID);
public:
	void initSetButton();
	void HNMJ_Button_Hua(cocos2d::Ref*,WidgetUserInfo*);
	void SetBarSider1(cocos2d::Ref*,WidgetUserInfo*);
	void SetBarSider2(cocos2d::Ref*,WidgetUserInfo*);
	void Button_HideSet(cocos2d::Ref*,WidgetUserInfo*);
	void Button_GameSet(cocos2d::Ref* pNode,WidgetUserInfo* pUserInfo);

public:
	static void pushGameRule(std::vector<std::string>&kRuleList, dword dwGameRule,int nRuleTag,bool bShow);
protected:
	int							m_iBankerUser;						//庄家用户
	int							m_iCurrentUser;						//当前用户
	int							m_iUserAction;						//玩家动作

	HNMJPlayer*					m_pLocal;
	HNMJPlayer*					m_pPlayer[MAX_PLAYER];

	cocos2d::Node*				m_pTouchCardNode;
	cocos2d::Vec2				m_kTouchSrcPos;

	int                         m_nGameState;
	int                         m_nPlayCount;

	word						m_wRecordSelfChairID;
	int							m_iActRecordIdex;
	int							m_RecordTimeID;
	float						m_RecordTime;
	HNMJGameRecord*				m_pGameRecord;

	int							m_iOutCardWaiteTime;
	CMD_GF_Private_Room_Info    m_kRoomInfo;

	BYTE						m_kcbGangCardCount;					//杠牌数目
	BYTE						m_kcbGangCardData[4];					//杠牌数据
};