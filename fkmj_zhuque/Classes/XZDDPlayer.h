
#pragma once

#include "GamePlayer.h"
#include "GameLib.h"
#include "cocos2d.h"
#include "CMD_XZDD.h"

struct OutCardInfo
{
	OutCardInfo(cocos2d::Node* pCard,int nData)
		:pCardNode(pCard)
		,nCardDarta(nData)
	{

	}
	cocos2d::Node* pCardNode;
	int           nCardDarta;
};
//游戏逻辑
class XZDDPlayer:public GamePlayer
{
public:
	const static int CARD_DWON_POSY = 70;
	const static int CARD_UP_POSY = 110;
	const static int HUAN_CARD_NUM = 3;
public:
	XZDDPlayer(int iIdex,cocos2d::Node* pSeatNode);
	~XZDDPlayer();

	void init();
	int  getIdex();
	void defaultState();
	void setGameRecord(bool bRecord);

	cocos2d::Node* getPlayerNode();
	void startGame();
	void startGameNoTime();//不要时间动画
	void EndGame();
	void setZhuang();
	void showAddGold(int iGold);
	void showEffect(std::string kKey);
	void showStatusImagic(std::string kKey);
public:
	virtual void PlayerEnter();
	virtual void PlayerLeave();
	virtual void upPlayerInfo();
	virtual void upPlayerState();
	virtual void showTalkState(CMD_GR_C_TableTalk* pNetInfo);
public:
	void setHandCard(BYTE* pCardData,int iCout);
	void setWeaveItem(XZDD_CMD_WeaveItem* pWeave,int iCout);
	void setOperateResoult(XZDD_CMD_S_OperateResult* pNetInfo);
	void addNewInCard(int iCard);
	void sendOutCard(int iCard);
	void showMJPoint();
	void setActOutCard(int iCard);
	cocos2d::Node* addHandOutCard(int iCard);
	void showHandCard();
	void showHandCard(XZDD_CMD_WeaveItem* pWeave,int iWeaveCout,BYTE* pHandCard,int iHandCout);
	void setCardImagic(cocos2d::Node* pNode,int kValue,std::string kImagicFront);
	cocos2d::Node* getTouchCardNode(cocos2d::Vec2 kTouchPos);
	BYTE getTouchCardVlaue(cocos2d::Node* pNode);
	void showHuPai(bool bGaizhu);
	void showJieSuanCard(cocos2d::Node* pCardNode,XZDD_CMD_WeaveItem* pWeave,int iWeaveCout,BYTE* pHandCard,int iHandCout);
	void setChiHuCard(BYTE cbCard);
	void clearChiHuCard();
	BYTE getChiHuCard();
	void showChiHuCard();
	void removeHandCard(BYTE cbCard);
public:
	void removeHandOutCard( BYTE cbCardData );
	void sortHandCard();
	void showHuanPai(int nGameType = GAME_TYPE_CHENGDU);
	void standUpCard(BYTE cbCard,bool nOnly = true);
	void seatDownCard();
	cocos2d::Node* getCardNodeByCard(BYTE cbCard);
	void getCardNodeByCard(std::vector<BYTE> CardDataList,std::vector<cocos2d::Node*>& kCardList);
	bool getHuanCards(BYTE nHuanCard[HUAN_CARD_NUM]);
	int getDefuaultHuanValue(int nIndex);//获取第几个默认换牌的值
	int getMastKindColor(); //最多张牌花色
	void showCard();
	void showCard(BYTE* cbCardData,BYTE cbCardCount);
	void setHuanPai(BYTE* nHuan);
	void saveRemoveHuanPai(BYTE* nHuan);
	void saveHuanPai(BYTE* nHuan);
	void setHuanPai_ChengDu();

	void setQueColor(int nColor);
	BYTE getQueColor();
	void showXuanQue(bool bShow);
	void showQueIcon_NoAni();
	void showQueIconAni();
	void showQueIconAni_Reset();

	void blackAllHandCard(bool bBlack);
	void blackHandCardByQueColor();
	void blackHandCardByColor(int nColor);

	void runAniDianPao();
	void runAniZiMo();
	void runAniPeng();
	void runAniMingGang();
	void runAniAnGang();

	XZDD_CMD_WeaveItem* getWeaveItemArray();
	BYTE getWeaveCount();

	std::vector<BYTE>  setHuanPaiSiChuan(std::vector<XZDD_Card_Info> kCardInfoList);
	std::vector<BYTE>  setHuanPaiChengDu(std::vector<XZDD_Card_Info> kCardInfoList);

public:
	BYTE getGangCard(BYTE currentCard);
	void setHasHu(bool bHasHu);
	bool getHasHu();
public:
	void ClickCard(cocos2d::Node* pCard);
	int getUpCardList(std::vector<BYTE>& kCardDataList);

public:
	void setHandCard_EX(std::vector<BYTE> kCardList);
	void setMaskHandCard(std::vector<XZDD_MasterHandCardInfo> kMasterHandCardInfo);
	void setMingPai(bool bValue);
public:
	template<typename T>
	bool isHave(std::vector<T*> kCardList,T* pItem)
	{
		for (int i=0;i<(int)kCardList.size();i++)
		{
			if(kCardList[i] == pItem)
				return true;
		}
		return false;
	}

private:
	int						m_iIdex;
	cocos2d::Node*			m_pSeatNode;

	BYTE					m_kHandCardData[MAX_COUNT];
	BYTE					m_kHandCardCout;

	XZDD_CMD_WeaveItem			m_kWeaveItemArray[MAX_WEAVE];		//组合扑克
	BYTE					m_kWeaveCount;					//组合数目

	BYTE				         m_nQueColor;
	BYTE				         m_pHuanCards[3];
	std::vector<cocos2d::Node*> m_pHuanCardNodeList;
	BYTE						 m_nMastKindColor;

	BYTE						m_pSaveHuanPai[3];
	std::vector<OutCardInfo>	m_kOutCardList;

	bool						m_bGameRecord;

	BYTE                  m_cbChiHuCard;//所胡的牌
	cocos2d::Node*         m_pHandOutCard;

	std::vector<BYTE>		m_kHuCardList;

	bool					m_bMingPai;
	bool                 m_bHasHu;
};

