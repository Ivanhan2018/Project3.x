#include "NCMJPlayer.h"
#include "NCMJSoundFun.h"


NCMJPlayer::NCMJPlayer(int iIdex,cocos2d::Node* pSeatNode )
	:GamePlayer(NULL)
	,m_iIdex(iIdex)
	,m_pSeatNode(pSeatNode)
	,m_kHandCardCout(0)
	,m_kWeaveCount(0)
	,m_pHandOutCard(NULL)
	,m_bGameRecord(false)
	,m_bMingPai(false)
{
	init();
}

NCMJPlayer::~NCMJPlayer()
{

}

void NCMJPlayer::init()
{
	PlayerLeave();
}
int  NCMJPlayer::getIdex()
{
	return m_iIdex;
}
void NCMJPlayer::defaultState()
{
	m_kHandCardCout = 0;
	m_kWeaveCount = 0;
	m_pHandOutCard = NULL;
	setActOutCard(-1);
	m_kOutCardList.clear();
	setChiHuCard(0);
	setGameRecord(false);

	WidgetFun::runWidgetAction(m_pSeatNode,"MJPoint","End");
	WidgetFun::setVisible(m_pSeatNode,"Zhuang",false);
	WidgetFun::setVisible(m_pSeatNode,"BigOutNode",false);
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",false);
	WidgetFun::getChildWidget(m_pSeatNode,"CardNode1")->removeAllChildren();
	WidgetFun::getChildWidget(m_pSeatNode,"CardNode2")->removeAllChildren();
	WidgetFun::setVisible(m_pSeatNode,"AnimateNode",false);
	upPlayerState();
}

void NCMJPlayer::setGameRecord(bool bRecord)
{
	m_bGameRecord = bRecord;
}

void NCMJPlayer::startGameNoTime()
{
	WidgetFun::runWidgetAction(m_pSeatNode,"Head","StartNoTime");
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",false);
}

void NCMJPlayer::startGame()
{
	WidgetFun::runWidgetAction(m_pSeatNode,"Head","Start");
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",false);

	if (m_iIdex == 0)
	{
		WidgetFun::setVisible(m_pSeatNode,"Name",true);
//		WidgetFun::runWidgetAction(m_pSeatNode,"GoldImagic","Start");
		WidgetFun::setVisible(m_pSeatNode,"GoldImagic",true);
	}
	else
	{
		WidgetFun::setVisible(m_pSeatNode,"Name",true);
		WidgetFun::setVisible(m_pSeatNode,"GoldImagic",true);
	}
}
void NCMJPlayer::EndGame()
{
	WidgetFun::runWidgetAction(m_pSeatNode,"Head","End");
	if (m_pUserItem == NULL )
	{
		return;
	}
	if (m_iIdex == 0)
	{
		WidgetFun::setVisible(m_pSeatNode,"Name",true);
		WidgetFun::runWidgetAction(m_pSeatNode,"GoldImagic","End");
	}
	else
	{
		WidgetFun::setVisible(m_pSeatNode,"Name",true);
		WidgetFun::setVisible(m_pSeatNode,"GoldImagic",true);
	}
}

void NCMJPlayer::PlayerEnter()
{
	WidgetFun::setVisible(m_pSeatNode,"Name",true);
	WidgetFun::setVisible(m_pSeatNode,"GoldImagic",true);
	WidgetFun::setVisible(m_pSeatNode,"HeadImagicEmpty",false);
	WidgetFun::setVisible(m_pSeatNode,"HeadImagic",true);

	if (WidgetFun::getChildWidget(m_pSeatNode,"ButtonPlayerHeadClick"))
	{
		WidgetFun::setWidgetUserInfo(m_pSeatNode,"ButtonPlayerHeadClick","UserID",utility::toString((int)GetUserID()));
	}
}

void NCMJPlayer::PlayerLeave()
{
	defaultState();
	showFangZhu(false);
	WidgetFun::setVisible(m_pSeatNode,"HeadImagicEmpty",true);
	WidgetFun::setVisible(m_pSeatNode,"HeadImagic",false);
	WidgetFun::setVisible(m_pSeatNode,"Name",false);
	WidgetFun::setVisible(m_pSeatNode,"GoldImagic",false);
	WidgetFun::setText(m_pSeatNode,"Name","");
	WidgetFun::setText(m_pSeatNode,"GoldTxt","");
}

cocos2d::Node* NCMJPlayer::getPlayerNode()
{
	return m_pSeatNode;
}
void NCMJPlayer::upPlayerInfo()
{
	WidgetFun::setText(m_pSeatNode,"Name",GetNickName());
	WidgetFun::setImagic(m_pSeatNode,"HeadImagic","Head/Head0.png");
	if (GetHeadHttp() != "")
	{
		ImagicDownManager::Instance().addDown(WidgetFun::getChildWidget(m_pSeatNode,"HeadImagic"),
			GetHeadHttp(),GetUserID());
	}
}
void NCMJPlayer::setZhuang()
{
	WidgetFun::setVisible(m_pSeatNode,"Zhuang",true);
}
void NCMJPlayer::showAddGold(int iGold)
{
	WidgetFun::setText(m_pSeatNode,"AddGoldTxt",iGold);
	WidgetFun::runWidgetAction(m_pSeatNode,"AddGoldTxt","Start");
}
void NCMJPlayer::showEffect(std::string kKey)
{
	cocos2d::Node* pNode = WidgetFun::getChildWidget(m_pSeatNode,"EffectImagic");
	WidgetFun::setPlaceImagicKey(pNode,kKey);
	pNode->stopAllActions();
	WidgetFun::runWidgetAction(pNode,"Start");
}
void NCMJPlayer::showStatusImagic(std::string kKey)
{
	WidgetFun::setPlaceImagicKey(m_pSeatNode,"ReadyState",kKey);
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",true);
}
void NCMJPlayer::upPlayerState()
{
	if (GetUserStatus() == US_READY)
	{
		showStatusImagic("Ready");
	}
	WidgetFun::setVisible(m_pSeatNode,"Offline",GetUserStatus() == US_OFFLINE);
}
void NCMJPlayer::showTalkState(CMD_GR_C_TableTalk* pNetInfo)
{

	std::string kStrNet = pNetInfo->strString;
	if (pNetInfo->cbType == CMD_GR_C_TableTalk::TYPE_FILE)
	{
		if (!WidgetFun::getChildWidget(m_pSeatNode,"TalkFile"))
		{
			return;
		}
		WidgetFun::runWidgetAction(m_pSeatNode,"TalkFile","Start");
	}
	if (pNetInfo->cbType == CMD_GR_C_TableTalk::TYPE_WORD)
	{
		if (!WidgetFun::getChildWidget(m_pSeatNode,"TalkTxtNode"))
		{
			return;
		}
		utility::runPaoMaDeng(m_pSeatNode,"TalkTxt","LayoutNode",
			kStrNet,NULL);
		cocos2d::Node* pTalkNode = WidgetFun::getChildWidget(m_pSeatNode,"TalkTxtNode");
		pTalkNode->setVisible(true);
		pTalkNode->stopAllActions();
		pTalkNode->runAction(cocos2d::CCVisibleAction::create(6.0f,false));
	}
	if (pNetInfo->cbType == CMD_GR_C_TableTalk::TYPE_BIAOQING)
	{
		if (!WidgetFun::getChildWidget(m_pSeatNode,"BiaoQingNode"))
		{
			return;
		}
		WidgetFun::setImagic(m_pSeatNode,"BiaoQingPic",kStrNet);
		cocos2d::Node* pTalkNode = WidgetFun::getChildWidget(m_pSeatNode,"BiaoQingNode");
		pTalkNode->setVisible(true);
		pTalkNode->stopAllActions();
		pTalkNode->runAction(cocos2d::CCVisibleAction::create(4.0f,false));
	}
	if (pNetInfo->cbType == CMD_GR_C_TableTalk::TYPE_DEFINE)
	{
		if (!WidgetFun::getChildWidget(m_pSeatNode,"TalkFile"))
		{
			return;
		}
		std::vector<std::string> kStr = utility::split(kStrNet,":");

		ASSERT(kStr.size()==2);

		WidgetFun::runWidgetAction(m_pSeatNode,"TalkFile","Start");
		SoundFun::Instance().playEffect(kStr[0]);

		utility::runPaoMaDeng(m_pSeatNode,"TalkTxt","LayoutNode",
			kStr[1],NULL);
		cocos2d::Node* pTalkNode = WidgetFun::getChildWidget(m_pSeatNode,"TalkTxtNode");
		pTalkNode->setVisible(true);
		pTalkNode->stopAllActions();
		pTalkNode->runAction(cocos2d::CCVisibleAction::create(6.0f,false));

	}
}


void NCMJPlayer::seatDownCard()
{
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
	for (int i = 0;i<pRootNode->getChildrenCount();i++)
	{
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pRootNode->getChildren().at(i));
		if (pSprite == NULL || pSprite->getTag() != 1)
		{
			continue;
		}
		pSprite->setPositionY(CARD_DWON_POSY);
	}
}

void NCMJPlayer::ClickCard(cocos2d::Node* pCard)
{
	if (pCard == NULL )return;

	int nPosY = pCard->getPositionY();
	if (nPosY == CARD_DWON_POSY)
	{
		pCard->setPositionY(CARD_UP_POSY);
	}
	else
	{
		pCard->setPositionY(CARD_DWON_POSY);
	}
}

void NCMJPlayer::runAniDianPao()
{
	NCMJSoundFun::playEffectByGender(GetGender(),NCMJSoundFun::EFFECT_HU);
//	WidgetFun::setPlaceImagicKey(m_pSeatNode,"AnimateNode","HuAni");
//	WidgetFun::runWidgetAction(m_pSeatNode,"AnimateNode","Start");
}
void NCMJPlayer::runAniZiMo()
{
	NCMJSoundFun::playEffectByGender(GetGender(),NCMJSoundFun::EFFECT_ZIMO);
	WidgetFun::setPlaceImagicKey(m_pSeatNode,"AnimateNode","HuAni");
	WidgetFun::runWidgetAction(m_pSeatNode,"AnimateNode","Start");
}
void NCMJPlayer::runAniPeng()
{
	NCMJSoundFun::playEffectByGender(GetGender(),NCMJSoundFun::EFFECT_PENG);
	WidgetFun::setPlaceImagicKey(m_pSeatNode,"AnimateNode","PengAni");
	WidgetFun::runWidgetAction(m_pSeatNode,"AnimateNode","Start");
}
void NCMJPlayer::runAniMingGang()
{
	NCMJSoundFun::playEffectByGender(GetGender(),NCMJSoundFun::EFFECT_GANG);
	WidgetFun::setPlaceImagicKey(m_pSeatNode,"AnimateNode","GangAni");
	WidgetFun::runWidgetAction(m_pSeatNode,"AnimateNode","Start");
}
void NCMJPlayer::runAniAnGang()
{
	NCMJSoundFun::playEffectByGender(GetGender(),NCMJSoundFun::EFFECT_GANG);
	WidgetFun::setPlaceImagicKey(m_pSeatNode,"AnimateNode","GangAni");
	WidgetFun::runWidgetAction(m_pSeatNode,"AnimateNode","Start");
}

void NCMJPlayer::runAniChi()
{
	NCMJSoundFun::playEffectByGender(GetGender(),NCMJSoundFun::EFFECT_CHI);
	WidgetFun::setPlaceImagicKey(m_pSeatNode,"AnimateNode","ChiAni");
	WidgetFun::runWidgetAction(m_pSeatNode,"AnimateNode","Start");
}

void NCMJPlayer::showFangZhu(bool bShow)
{
	WidgetFun::setVisible(m_pSeatNode,"FangZhu",bShow);
}
