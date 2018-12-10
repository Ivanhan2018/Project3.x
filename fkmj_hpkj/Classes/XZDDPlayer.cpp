#include "XZDDPlayer.h"
#include "XZDDSoundFun.h"
#include "XZDDGameScence.h"


XZDDPlayer::XZDDPlayer(int iIdex,cocos2d::Node* pSeatNode )
	:GamePlayer(NULL)
	,m_iIdex(iIdex)
	,m_pSeatNode(pSeatNode)
	,m_kHandCardCout(0)
	,m_kWeaveCount(0)
	,m_bGameRecord(false)
	,m_nQueColor(CARD_COLOR_NULL)
	,m_pHandOutCard(NULL)
	,m_bMingPai(false)
	,m_bHasHu(false)
{
	init();
}

XZDDPlayer::~XZDDPlayer()
{

}

void XZDDPlayer::init()
{
	PlayerLeave();
}
int  XZDDPlayer::getIdex()
{
	return m_iIdex;
}
void XZDDPlayer::defaultState()
{
	setGameRecord(false);
	setChiHuCard(0);
	m_pHandOutCard = NULL;
	m_kHandCardCout = 0;
	m_kWeaveCount = 0;
	setActOutCard(-1);

	WidgetFun::runWidgetAction(m_pSeatNode,"MJPoint","End");
	WidgetFun::setVisible(m_pSeatNode,"Zhuang",false);
	WidgetFun::setVisible(m_pSeatNode,"Offline",false);
	WidgetFun::setVisible(m_pSeatNode,"BigOutNode",false);
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",false);
	WidgetFun::getChildWidget(m_pSeatNode,"CardNode1")->removeAllChildren();
	WidgetFun::getChildWidget(m_pSeatNode,"CardNode2")->removeAllChildren();
	WidgetFun::getChildWidget(m_pSeatNode,"AnimateNode")->removeAllChildren();

	zeromemory(m_pHuanCards,sizeof(m_pHuanCards));
	zeromemory(m_pSaveHuanPai,sizeof(m_pSaveHuanPai));
	
	m_pHuanCardNodeList.clear();

	WidgetFun::setVisible(m_pSeatNode,"QueIcon_Img",false);
	showXuanQue(false);
	upPlayerState();
	m_kOutCardList.clear();
	clearChiHuCard();
	setQueColor(CARD_COLOR_NULL);
	setMingPai(false);
	setHasHu(false);
}
void XZDDPlayer::setGameRecord(bool bRecord)
{
	m_bGameRecord = bRecord;
}

cocos2d::Node* XZDDPlayer::getPlayerNode()
{
	return m_pSeatNode;
}
void XZDDPlayer::startGame()
{
	WidgetFun::runWidgetAction(m_pSeatNode,"Head","Start");
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",false);

}

void XZDDPlayer::startGameNoTime()
{
	WidgetFun::runWidgetAction(m_pSeatNode,"Head","StartNoTime");
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",false);

}

void XZDDPlayer::EndGame()
{
	WidgetFun::runWidgetAction(m_pSeatNode,"Head","End");
	if (m_pUserItem == NULL )
	{
		return;
	}
}

void XZDDPlayer::PlayerEnter()
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

void XZDDPlayer::PlayerLeave()
{
	defaultState();
	WidgetFun::setVisible(m_pSeatNode,"HeadImagicEmpty",true);
	WidgetFun::setVisible(m_pSeatNode,"HeadImagic",false);
	WidgetFun::setVisible(m_pSeatNode,"Name",false);
	WidgetFun::setVisible(m_pSeatNode,"GoldImagic",false);
	WidgetFun::setText(m_pSeatNode,"Name","");
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",false);


	WidgetFun::setWidgetUserInfo(m_pSeatNode,"ButtonPlayerHeadClick","");}

void XZDDPlayer::upPlayerInfo()
{
	WidgetFun::setText(m_pSeatNode,"Name",GetNickName());

	if (XZDDGameScence::Instance().HaveOptionRule(XZDDGameScence::GAME_OPTION_RULE_SHOW_WIN_LOSE1) == false
		&&XZDDGameScence::Instance().HaveOptionRule(XZDDGameScence::GAME_OPTION_RULE_SHOW_WIN_LOSE_ZERO) == false)
	{
		WidgetFun::setText(m_pSeatNode,"GoldTxt",(int)GetUserScore());
	}


	if (GetHeadHttp() != "")
	{
		ImagicDownManager::Instance().addDown(WidgetFun::getChildWidget(m_pSeatNode,"HeadImagic"),
			GetHeadHttp(),GetUserID());
	}
}
void XZDDPlayer::setZhuang()
{
	WidgetFun::setVisible(m_pSeatNode,"Zhuang",true);
}
void XZDDPlayer::showAddGold(int iGold)
{
	if (iGold>0)
	{
		WidgetFun::setAtlasTxt(m_pSeatNode,"AddGoldTxtWin",utility::toString("+",iGold));
		WidgetFun::runWidgetAction(m_pSeatNode,"AddGoldTxtWin","Start");
	}
	else if(iGold<0)
	{
		WidgetFun::setAtlasTxt(m_pSeatNode,"AddGoldTxtLose",utility::toString(iGold));
		WidgetFun::runWidgetAction(m_pSeatNode,"AddGoldTxtLose","Start");
	}
}
void XZDDPlayer::showEffect(std::string kKey)
{
	cocos2d::Node* pNode = WidgetFun::getChildWidget(m_pSeatNode,"EffectImagic");
	WidgetFun::setPlaceImagicKey(pNode,kKey);
	pNode->stopAllActions();
	WidgetFun::runWidgetAction(pNode,"Start");
}
void XZDDPlayer::showStatusImagic(std::string kKey)
{
	WidgetFun::setPlaceImagicKey(m_pSeatNode,"ReadyState",kKey);
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",true);
}

void XZDDPlayer::showXuanQue(bool bShow)
{
	WidgetFun::setVisible(m_pSeatNode,"XuanQueNode",bShow);

	if (m_nQueColor == CARD_COLOR_NULL)
	{
		return;
	}

	if (m_iIdex == 0)
	{
		WidgetFun::setVisible(m_pSeatNode,"Img_QueColor2",false);
		WidgetFun::setVisible(m_pSeatNode,"Img_QueColor1",false);
		WidgetFun::setVisible(m_pSeatNode,"Img_QueColor3",false);

		std::string  kColorShow = utility::toString("Img_QueColor",(int)m_nQueColor);
		WidgetFun::setVisible(m_pSeatNode,kColorShow,true);
	}
}


void XZDDPlayer::setQueColor( int nColor )
{
	assert( nColor<= CARD_COLOR_TIAO);
	m_nQueColor = nColor;	
}
BYTE XZDDPlayer::getQueColor()
{
	return m_nQueColor;
}

void XZDDPlayer::showQueIcon_NoAni()
{
	if (m_nQueColor == CARD_COLOR_NULL)
	{
		return;
	}
	WidgetFun::setPlaceImagicKey(m_pSeatNode,"QueIcon_Img",utility::toString("QueIcon",(int)m_nQueColor));

	WidgetFun::setVisible(m_pSeatNode,"QueIcon_Img",true);
	WidgetFun::runWidgetAction(m_pSeatNode,"QueIcon_Img","QueColor_NoAni");

}
void XZDDPlayer::showQueIconAni()
{
	if (m_nQueColor == CARD_COLOR_NULL)
	{
		return;
	}

	WidgetFun::setPlaceImagicKey(m_pSeatNode,"QueIcon_Img",utility::toString("QueIcon",(int)m_nQueColor));
	WidgetFun::setVisible(m_pSeatNode,"QueIcon_Img",true);
	WidgetFun::runWidgetAction(m_pSeatNode,"QueIcon_Img","QueColorAni");
}
void XZDDPlayer::showQueIconAni_Reset()
{
	if (m_nQueColor == CARD_COLOR_NULL)
	{
		return;
	}

	WidgetFun::setPlaceImagicKey(m_pSeatNode,"QueIcon_Img",utility::toString("QueIcon",(int)m_nQueColor));
	WidgetFun::setVisible(m_pSeatNode,"QueIcon_Img",true);
	WidgetFun::runWidgetAction(m_pSeatNode,"QueIcon_Img","QueColorAni_Reset");
}

void XZDDPlayer::upPlayerState()
{
	if (GetUserStatus() == US_READY)
	{
		showStatusImagic("Ready");
	}
	WidgetFun::setVisible(m_pSeatNode,"Offline",GetUserStatus() == US_OFFLINE);
}

void XZDDPlayer::showTalkState(CMD_GR_C_TableTalk* pNetInfo)
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

int XZDDPlayer::getMastKindColor()
{
	return m_nMastKindColor;
}

void XZDDPlayer::runAniDianPao()
{
	cocos2d::Node* AnimateNode = WidgetFun::getChildWidget(m_pSeatNode,"AnimateNode");
	AnimateNode->removeAllChildren();
	WidgetManager::Instance().createWidget("XZDDDianPaoAni",AnimateNode);

	XZDDSoundFun::playEffectEX(XZDDSoundFun::EX_FANGPAO);
	XZDDSoundFun::playEffectByGender(GetGender(),XZDDSoundFun::EFFECT_HU);
}
void XZDDPlayer::runAniZiMo()
{
	cocos2d::Node* AnimateNode = WidgetFun::getChildWidget(m_pSeatNode,"AnimateNode");
	AnimateNode->removeAllChildren();
	WidgetManager::Instance().createWidget("XZDDZiMoAni",AnimateNode);

	XZDDSoundFun::playEffectEX(XZDDSoundFun::EX_ZIMO);
	XZDDSoundFun::playEffectByGender(GetGender(),XZDDSoundFun::EFFECT_ZIMO);
}
void XZDDPlayer::runAniPeng()
{
	cocos2d::Node* AnimateNode = WidgetFun::getChildWidget(m_pSeatNode,"AnimateNode");
	AnimateNode->removeAllChildren();
	WidgetManager::Instance().createWidget("XZDDPengAni",AnimateNode);

	XZDDSoundFun::playEffectEX(XZDDSoundFun::EX_PENG);
	XZDDSoundFun::playEffectByGender(GetGender(),XZDDSoundFun::EFFECT_PENG);
}
void XZDDPlayer::runAniMingGang()
{
	cocos2d::Node* AnimateNode = WidgetFun::getChildWidget(m_pSeatNode,"AnimateNode");
	AnimateNode->removeAllChildren();
	WidgetManager::Instance().createWidget("XZDDMingGangAni",AnimateNode);

	XZDDSoundFun::playEffectEX(XZDDSoundFun::EX_GANG);
	XZDDSoundFun::playEffectByGender(GetGender(),XZDDSoundFun::EFFECT_GANG);
}
void XZDDPlayer::runAniAnGang()
{
	cocos2d::Node* AnimateNode = WidgetFun::getChildWidget(m_pSeatNode,"AnimateNode");
	AnimateNode->removeAllChildren();
	WidgetManager::Instance().createWidget("XZDDAnGangAni",AnimateNode);

	XZDDSoundFun::playEffectEX(XZDDSoundFun::EX_GANG);
	XZDDSoundFun::playEffectByGender(GetGender(),XZDDSoundFun::EFFECT_AN_GANG);
}
XZDD_CMD_WeaveItem* XZDDPlayer::getWeaveItemArray()
{
	return m_kWeaveItemArray;
}
BYTE XZDDPlayer::getWeaveCount()
{
	return m_kWeaveCount;
}

void XZDDPlayer::setHasHu(bool bHasHu)
{
	m_bHasHu = bHasHu;
}

bool XZDDPlayer::getHasHu()
{
	return m_bHasHu;
}
