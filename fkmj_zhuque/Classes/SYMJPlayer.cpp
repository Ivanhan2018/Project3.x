#include "SYMJPlayer.h"
#include "SYMJSoundFun.h"
#include "SYMJGameScence.h"
#include "SYMJGameLogic.h"

SYMJPlayer::SYMJPlayer(int iIdex,cocos2d::Node* pSeatNode )
	:GamePlayer(NULL)
	,m_iIdex(iIdex)
	,m_pSeatNode(pSeatNode)
	,m_kHandCardCout(0)
	,m_kWeaveCount(0)
	,m_pHandOutCard(NULL)
	,m_bGameRecord(false)
	,m_bMingPai(false)
	,m_bHasCSGang(false)
	,m_cbChiHuCard(0)
{
	init();
}

SYMJPlayer::~SYMJPlayer()
{

}

void SYMJPlayer::init()
{
	PlayerLeave();
}
int  SYMJPlayer::getIdex()
{
	return m_iIdex;
}
void SYMJPlayer::defaultState()
{
	m_kHandCardCout = 0;
	m_kWeaveCount = 0;
	m_cbChiHuCard = 0;
	m_pHandOutCard = NULL;
	setActOutCard(-1);
	m_kOutCardList.clear();
	setChiHuCard(0);
	setTuoGuan(false);
	
	WidgetFun::runWidgetAction(m_pSeatNode,"MJPoint","End");
	WidgetFun::setVisible(m_pSeatNode,"Zhuang",false);
	WidgetFun::setVisible(m_pSeatNode,"BigOutNode",false);
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",false);
	WidgetFun::getChildWidget(m_pSeatNode,"CardNode1")->removeAllChildren();
	WidgetFun::getChildWidget(m_pSeatNode,"CardNode2")->removeAllChildren();
	WidgetFun::setVisible(m_pSeatNode,"AnimateNode",false);
	WidgetFun::setVisible(m_pSeatNode,"CSGangCardBg",false);
	WidgetFun::setVisible(m_pSeatNode,"CuiJu",false);

	upPlayerState();
	setMingPai(false);
	setGameRecord(false);
	if (m_iIdex == 0)
	{
		WidgetFun::setVisible(m_pSeatNode,"TingNode",false);
	}
}
void SYMJPlayer::setTuoGuan(bool bTuoGuan)
{
	WidgetFun::setVisible(m_pSeatNode,"TuoGuanImagic",bTuoGuan);
}

void SYMJPlayer::setGameRecord(bool bRecord)
{
	m_bGameRecord = bRecord;
}

void SYMJPlayer::startGameNoTime()
{
	WidgetFun::runWidgetAction(m_pSeatNode,"Head","StartNoTime");
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",false);
}

void SYMJPlayer::startGame()
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
void SYMJPlayer::EndGame()
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

void SYMJPlayer::PlayerEnter()
{
	WidgetFun::setVisible(m_pSeatNode,"Name",true);
	WidgetFun::setVisible(m_pSeatNode,"GoldImagic",true);
	WidgetFun::setVisible(m_pSeatNode,"HeadImagicEmpty",false);
	WidgetFun::setVisible(m_pSeatNode,"HeadImagic",true);
	WidgetFun::setVisible(m_pSeatNode,"Offline",false);

	if (WidgetFun::getChildWidget(m_pSeatNode,"ButtonPlayerHeadClick"))
	{
		WidgetFun::setWidgetUserInfo(m_pSeatNode,"ButtonPlayerHeadClick","UserID",utility::toString((int)GetUserID()));
	}
}

void SYMJPlayer::PlayerLeave()
{
	showFangZhu(false);
	defaultState();
	WidgetFun::setVisible(m_pSeatNode,"HeadImagicEmpty",true);
	WidgetFun::setVisible(m_pSeatNode,"HeadImagic",false);
	WidgetFun::setVisible(m_pSeatNode,"Name",false);
	WidgetFun::setVisible(m_pSeatNode,"GoldImagic",false);
	WidgetFun::setText(m_pSeatNode,"Name","");
	WidgetFun::setText(m_pSeatNode,"GoldTxt","");
}

cocos2d::Node* SYMJPlayer::getPlayerNode()
{
	return m_pSeatNode;
}
void SYMJPlayer::upPlayerInfo()
{
	WidgetFun::setText(m_pSeatNode,"Name",GetNickName());
	if (SYMJGameScence::Instance().HaveOptionRule(SYMJGameScence::GAME_OPTION_RULE_SHOW_WIN_LOSE1) == false
		&&SYMJGameScence::Instance().HaveOptionRule(SYMJGameScence::GAME_OPTION_RULE_SHOW_WIN_LOSE_ZERO) == false)
	{
		WidgetFun::setText(m_pSeatNode,"GoldTxt",(int)GetUserScore());
	}
	static ScriptData<std::string> kHNMJHead("HNMJHeadImgic");
	WidgetFun::setImagic(m_pSeatNode,"HeadImagic",kHNMJHead.Value());
	if (GetHeadHttp() != "")
	{
		ImagicDownManager::Instance().addDown(WidgetFun::getChildWidget(m_pSeatNode,"HeadImagic"),
			GetHeadHttp(),GetUserID());
	}
}
void SYMJPlayer::setZhuang()
{
	WidgetFun::setVisible(m_pSeatNode,"Zhuang",true);
}
void SYMJPlayer::showAddGold(int iGold)
{
	WidgetFun::setText(m_pSeatNode,"AddGoldTxt",iGold);
	WidgetFun::runWidgetAction(m_pSeatNode,"AddGoldTxt","Start");
}
void SYMJPlayer::showEffect(std::string kKey)
{
	cocos2d::Node* pNode = WidgetFun::getChildWidget(m_pSeatNode,"EffectImagic");
	WidgetFun::setPlaceImagicKey(pNode,kKey);
	pNode->stopAllActions();
	WidgetFun::runWidgetAction(pNode,"Start");
}
void SYMJPlayer::showStatusImagic(std::string kKey)
{
	WidgetFun::setPlaceImagicKey(m_pSeatNode,"ReadyState",kKey);
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",true);
}
void SYMJPlayer::upPlayerState()
{
	if (GetUserStatus() == US_READY)
	{
		showStatusImagic("Ready");
	}
	WidgetFun::setVisible(m_pSeatNode,"Offline",GetUserStatus() == US_OFFLINE);
}


void SYMJPlayer::seatDownCard()
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

void SYMJPlayer::ClickCard(cocos2d::Node* pCard)
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

void SYMJPlayer::runAniDianPao()
{
	SYMJSoundFun::playEffectByGender(GetGender(),SYMJSoundFun::EFFECT_HU);
	WidgetFun::setPlaceImagicKey(m_pSeatNode,"AnimateNode","HuAni");
	WidgetFun::runWidgetAction(m_pSeatNode,"AnimateNode","Start");
}
void SYMJPlayer::runAniZiMo()
{
	SYMJSoundFun::playEffectByGender(GetGender(),SYMJSoundFun::EFFECT_ZIMO);
	WidgetFun::setPlaceImagicKey(m_pSeatNode,"AnimateNode","HuAni");
	WidgetFun::runWidgetAction(m_pSeatNode,"AnimateNode","Start");
}
void SYMJPlayer::runAniPeng()
{
	SYMJSoundFun::playEffectByGender(GetGender(),SYMJSoundFun::EFFECT_PENG);
	WidgetFun::setPlaceImagicKey(m_pSeatNode,"AnimateNode","PengAni");
	WidgetFun::runWidgetAction(m_pSeatNode,"AnimateNode","Start");
}
void SYMJPlayer::runAniMingGang()
{
	SYMJSoundFun::playEffectByGender(GetGender(),SYMJSoundFun::EFFECT_GANG);
	WidgetFun::setPlaceImagicKey(m_pSeatNode,"AnimateNode","GangAni");
	WidgetFun::runWidgetAction(m_pSeatNode,"AnimateNode","Start");
}
void SYMJPlayer::runAniAnGang()
{
	SYMJSoundFun::playEffectByGender(GetGender(),SYMJSoundFun::EFFECT_GANG);
	WidgetFun::setPlaceImagicKey(m_pSeatNode,"AnimateNode","GangAni");
	WidgetFun::runWidgetAction(m_pSeatNode,"AnimateNode","Start");
}

void SYMJPlayer::runAniChi()
{
	SYMJSoundFun::playEffectByGender(GetGender(),SYMJSoundFun::EFFECT_CHI);
	WidgetFun::setPlaceImagicKey(m_pSeatNode,"AnimateNode","ChiAni");
	WidgetFun::runWidgetAction(m_pSeatNode,"AnimateNode","Start");
}

void SYMJPlayer::showFangZhu(bool bShow)
{
	if (WidgetFun::getChildWidget(m_pSeatNode,"FangZhu"))
	{
		WidgetFun::setVisible(m_pSeatNode,"FangZhu",bShow);
	}
}

void SYMJPlayer::showFanBei(int nBei)
{
	if (nBei>1)
	{
		if (WidgetFun::getChildWidget(m_pSeatNode,"CuiJu"))
		{
			WidgetFun::setVisible(m_pSeatNode,"CuiJu",true);
		}
	}
	else
	{
		if (WidgetFun::getChildWidget(m_pSeatNode,"CuiJu"))
		{
			WidgetFun::setVisible(m_pSeatNode,"CuiJu",false);
		}
	}
}

void SYMJPlayer::showTalkState(CMD_GR_C_TableTalk* pNetInfo)
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
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		NoticeMsg::Instance().ShowTopMsg(kStrNet);
#endif
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

