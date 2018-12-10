#include "NNPlayer.h"
#include "NNSoundFun.h"
#include "NNGameScence.h"

NNPlayer::NNPlayer(int iIdex,cocos2d::Node* pSeatNode )
	:GamePlayer(NULL)
	,m_iIdex(iIdex)
	,m_pSeatNode(pSeatNode)
	,m_bGameRecord(false)
{
	zeromemory(m_cbCardData,sizeof(m_cbCardData));
	init();
}

NNPlayer::~NNPlayer()
{

}

void NNPlayer::init()
{
	PlayerLeave();
}
int  NNPlayer::getIdex()
{
	return m_iIdex;
}
void NNPlayer::defaultState()
{
	HideClock();
	upPlayerState();

	setGameRecord(false);

	WidgetFun::setVisible(m_pSeatNode,"Zhuang",false);
	WidgetFun::setVisible(m_pSeatNode,"FangZhu",false);
	WidgetFun::setVisible(m_pSeatNode,"Offline",false);
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",false);
	WidgetFun::setVisible(m_pSeatNode,"NN_CardType",false);
	
	zeromemory(m_cbCardData,sizeof(m_cbCardData));
	for (int i = 0;i < 5;i++)
	{
		WidgetFun::setVisible(m_pSeatNode,utility::toString("Card",i),false);
	}
	showJiaoZhuang(false);
	showQiangZhuangIcon(false);
}

void NNPlayer::setGameRecord(bool bRecord)
{
	m_bGameRecord = bRecord;
}


void NNPlayer::PlayerEnter()
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

void NNPlayer::PlayerLeave()
{
	defaultState();
	WidgetFun::setVisible(m_pSeatNode,"HeadImagicEmpty",true);
	WidgetFun::setVisible(m_pSeatNode,"HeadImagic",false);
	WidgetFun::setVisible(m_pSeatNode,"Name",false);
	WidgetFun::setVisible(m_pSeatNode,"GoldImagic",false);
	WidgetFun::setText(m_pSeatNode,"Name","");
	WidgetFun::setText(m_pSeatNode,"GoldTxt","");
}

cocos2d::Node* NNPlayer::getPlayerNode()
{
	return m_pSeatNode;
}
void NNPlayer::showClock(int iTime)
{
	showJiaoZhuang(false);
	showQiangZhuangIcon(false);
	WidgetFun::setVisible(m_pSeatNode,"PlayerClock",true);
	cocos2d::Node* pNode = WidgetFun::getChildWidget(m_pSeatNode,"ClockTime");
	pNode->stopAllActions();
	pNode->runAction(cocos2d::MoveExTxtTime::create(iTime,iTime,0));
}

void NNPlayer::showClockTimeCallBack(int nSecond,const std::function<void()>& pCallBack,float fCheakTime)
{	
	showJiaoZhuang(false);
	showQiangZhuangIcon(false);
	WidgetFun::setVisible(m_pSeatNode,"PlayerClock",true);

	cocos2d::Node* pNode = WidgetFun::getChildWidget(m_pSeatNode,"ClockTime");
	pNode->stopAllActions();
	pNode->runAction(cocos2d::MoveExTxtTimeCallBack::create(nSecond,nSecond,0,pCallBack,fCheakTime));
}
void NNPlayer::HideClock()
{
	if (m_iIdex != 0)
	{
		return;
	}
	WidgetFun::setVisible(m_pSeatNode,"PlayerClock",false);
}
void NNPlayer::upPlayerInfo()
{
	WidgetFun::setText(m_pSeatNode,"Name",GetNickName());
	if (NNGameScence::Instance().HaveOptionRule(NNGameScence::GAME_OPTION_RULE_SHOW_WIN_LOSE1) == false)
	{
		WidgetFun::setText(m_pSeatNode,"GoldTxt",(int)GetUserScore());
	}
	if (GetHeadHttp() != "")
	{
		ImagicDownManager::Instance().addDown(WidgetFun::getChildWidget(m_pSeatNode,"HeadImagic"),
			GetHeadHttp(),GetUserID());
	}
}
void NNPlayer::setZhuang(bool bAni)
{
	if (!bAni)
	{
		WidgetFun::setVisible(m_pSeatNode,"Zhuang",true);
		return;
	}
	std::string rootPos =WidgetFun::getWidgetUserInfo(m_pSeatNode,"ZhuangRootPos");
	WidgetFun::setWidgetUserInfo(m_pSeatNode,"Zhuang","FromPos",rootPos);
	WidgetFun::runWidgetAction(m_pSeatNode,"Zhuang","MoveAni");
}
void NNPlayer::showAddGold(int iGold)
{
	if (iGold>=0)
	{
		WidgetFun::setAtlasTxt(m_pSeatNode,"AddGoldTxtWin",utility::toString("+",iGold));
		WidgetFun::runWidgetAction(m_pSeatNode,"AddGoldTxtWin","Start");
	}
	else
	{
		WidgetFun::setAtlasTxt(m_pSeatNode,"AddGoldTxtLose",utility::toString(iGold));
		WidgetFun::runWidgetAction(m_pSeatNode,"AddGoldTxtLose","Start");
	}

}
void NNPlayer::showStatusImagic(std::string kKey)
{
	WidgetFun::setPlaceImagicKey(m_pSeatNode,"ReadyState",kKey);
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",true);
}
void NNPlayer::upPlayerState()
{
	if (GetUserStatus() == US_READY)
	{
		showStatusImagic("Ready");
	}
	WidgetFun::setVisible(m_pSeatNode,"Offline",GetUserStatus() == US_OFFLINE);
}

void NNPlayer::showJiaoZhuang(bool bShow)
{
	WidgetFun::setVisible(m_pSeatNode,"BeiImg",bShow);
}

void NNPlayer::showQiangZhuangIcon(bool bShow)
{
	WidgetFun::setVisible(m_pSeatNode,"QaingZhuangIcon",bShow);
}

void NNPlayer::setJiaoZhuangFen(int nFen,bool bDingZhuang)
{
	showJiaoZhuang(true);
	WidgetFun::setAtlasTxt(m_pSeatNode,"NNBeiNum",utility::toString(nFen));

	if (bDingZhuang)
	{
		showQiangZhuangIcon(true);
		WidgetFun::setVisible(m_pSeatNode,"BuQiang",true);
		WidgetFun::setVisible(m_pSeatNode,"Qiang",nFen>0);
	}
}

void NNPlayer::clickCard(cocos2d::Node* pCardNode)
{
	ASSERT(pCardNode);
	int nPosY = pCardNode->getPositionY();
	if (nPosY == POSY_DOWN)
	{
		pCardNode->setPositionY(POSY_UP);
	}
	else
	{
		pCardNode->setPositionY(POSY_DOWN);
	}
}

void NNPlayer::runQiangZhuangAni(bool bBlink)
{
	if (bBlink)
	{
		WidgetFun::runWidgetAction(m_pSeatNode,"Qiang","BlinkAni");
	}
	else
	{
		WidgetFun::runWidgetAction(m_pSeatNode,"Qiang","ShowAni");
	}
}

