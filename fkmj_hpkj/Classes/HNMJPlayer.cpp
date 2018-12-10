#include "HNMJPlayer.h"
#include "HNMJSoundFun.h"
#include "HNMJGameScence.h"
#include "HNMJGameLogic.h"

HNMJPlayer::HNMJPlayer(int iIdex,cocos2d::Node* pSeatNode )
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

HNMJPlayer::~HNMJPlayer()
{

}

void HNMJPlayer::init()
{
	PlayerLeave();
}
int  HNMJPlayer::getIdex()
{
	return m_iIdex;
}
void HNMJPlayer::defaultState()
{
	m_kHandCardCout = 0;
	m_kWeaveCount = 0;
	m_cbChiHuCard = 0;
	m_pHandOutCard = NULL;
	setActOutCard(-1);
	m_kOutCardList.clear();
	setTuoGuan(false);
	
	WidgetFun::runWidgetAction(m_pSeatNode,"MJPoint","End");
	WidgetFun::setVisible(m_pSeatNode,"Zhuang",false);
	WidgetFun::setVisible(m_pSeatNode,"BigOutNode",false);
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",false);
	WidgetFun::getChildWidget(m_pSeatNode,"CardNode1")->removeAllChildren();
	WidgetFun::getChildWidget(m_pSeatNode,"CardNode2")->removeAllChildren();
	WidgetFun::setVisible(m_pSeatNode,"CSGangCardBg",false);

	upPlayerState();
	setMingPai(false);
	setGameRecord(false);
	setHasCSGang(false);
	if (m_iIdex == 0)
	{
		WidgetFun::setVisible(m_pSeatNode,"TingNode",false);
	}
}
void HNMJPlayer::setTuoGuan(bool bTuoGuan)
{
	WidgetFun::setVisible(m_pSeatNode,"TuoGuanImagic",bTuoGuan);
}

void HNMJPlayer::setGameRecord(bool bRecord)
{
	m_bGameRecord = bRecord;
}

void HNMJPlayer::startGameNoTime()
{
	WidgetFun::runWidgetAction(m_pSeatNode,"Head","StartNoTime");
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",false);
}

void HNMJPlayer::startGame()
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
void HNMJPlayer::EndGame()
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

void HNMJPlayer::PlayerEnter()
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

void HNMJPlayer::PlayerLeave()
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

cocos2d::Node* HNMJPlayer::getPlayerNode()
{
	return m_pSeatNode;
}
void HNMJPlayer::upPlayerInfo()
{
	if (HNMJGameScence::Instance().HaveOptionRule(HNMJGameScence::GAME_OPTION_SHOW_STAR_VALUE))
	{
		std::string kTxt = WidgetFun::getWidgetUserInfo(m_pSeatNode,"Name","Txt");
		WidgetFun::setText(m_pSeatNode,"Name",utility::toString(GetNickName(),"(",GetUserStarValue(),kTxt,")"));
	}
	else
	{
		WidgetFun::setText(m_pSeatNode,"Name",GetNickName());
	}
	if (HNMJGameScence::Instance().HaveOptionRule(HNMJGameScence::GAME_OPTION_RULE_SHOW_WIN_LOSE1) == false
		&&HNMJGameScence::Instance().HaveOptionRule(HNMJGameScence::GAME_OPTION_RULE_SHOW_WIN_LOSE_ZERO) == false)
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
void HNMJPlayer::setZhuang()
{
	WidgetFun::setVisible(m_pSeatNode,"Zhuang",true);
}
void HNMJPlayer::showAddGold(int iGold)
{
	WidgetFun::setText(m_pSeatNode,"AddGoldTxt",iGold);
	WidgetFun::runWidgetAction(m_pSeatNode,"AddGoldTxt","Start");
}
void HNMJPlayer::showEffect(std::string kKey)
{
	cocos2d::Node* pNode = WidgetFun::getChildWidget(m_pSeatNode,"EffectImagic");
	WidgetFun::setPlaceImagicKey(pNode,kKey);
	pNode->stopAllActions();
	WidgetFun::runWidgetAction(pNode,"Start");
}
void HNMJPlayer::showStatusImagic(std::string kKey)
{
	WidgetFun::setPlaceImagicKey(m_pSeatNode,"ReadyState",kKey);
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",true);
}
void HNMJPlayer::upPlayerState()
{
	if (GetUserStatus() == US_READY)
	{
		showStatusImagic("Ready");
	}
	WidgetFun::setVisible(m_pSeatNode,"Offline",GetUserStatus() == US_OFFLINE);
}


void HNMJPlayer::seatDownCard()
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

void HNMJPlayer::ClickCard(cocos2d::Node* pCard)
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

void HNMJPlayer::runAniDianPao()
{
	HNMJSoundFun::playEffectByKind(GetGender(),HNMJSoundFun::EFFECT_HU);
	cocos2d::Node* pNode = WidgetFun::getChildWidget(m_pSeatNode,"AnimateNode");
	cocos2d::Node* pTemp = WidgetManager::Instance().createWidget("THJActionAni",pNode);
	WidgetFun::setPlaceImagicKey(pTemp,"HuAni");
}
void HNMJPlayer::runAniZiMo()
{
	HNMJSoundFun::playEffectByKind(GetGender(),HNMJSoundFun::EFFECT_ZIMO);
	cocos2d::Node* pNode = WidgetFun::getChildWidget(m_pSeatNode,"AnimateNode");
	cocos2d::Node* pTemp = WidgetManager::Instance().createWidget("THJActionAni",pNode);
	WidgetFun::setPlaceImagicKey(pTemp,"HuAni");
}
void HNMJPlayer::runAniPeng()
{
	HNMJSoundFun::playEffectByKind(GetGender(),HNMJSoundFun::EFFECT_PENG);
	cocos2d::Node* pNode = WidgetFun::getChildWidget(m_pSeatNode,"AnimateNode");
	cocos2d::Node* pTemp = WidgetManager::Instance().createWidget("THJActionAni",pNode);
	WidgetFun::setPlaceImagicKey(pTemp,"PengAni");
}
void HNMJPlayer::runAniMingGang()
{
	HNMJSoundFun::playEffectByKind(GetGender(),HNMJSoundFun::EFFECT_GANG);
	cocos2d::Node* pNode = WidgetFun::getChildWidget(m_pSeatNode,"AnimateNode");
	cocos2d::Node* pTemp = WidgetManager::Instance().createWidget("THJActionAni",pNode);
	WidgetFun::setPlaceImagicKey(pTemp,"GangAni");
}
void HNMJPlayer::runAniAnGang()
{
	HNMJSoundFun::playEffectByKind(GetGender(),HNMJSoundFun::EFFECT_GANG);
	cocos2d::Node* pNode = WidgetFun::getChildWidget(m_pSeatNode,"AnimateNode");
	cocos2d::Node* pTemp = WidgetManager::Instance().createWidget("THJActionAni",pNode);
	WidgetFun::setPlaceImagicKey(pTemp,"GangAni");
}

void HNMJPlayer::runAniBu()
{
	HNMJSoundFun::playEffectByKind(GetGender(),HNMJSoundFun::EFFECT_BU);
	cocos2d::Node* pNode = WidgetFun::getChildWidget(m_pSeatNode,"AnimateNode");
	cocos2d::Node* pTemp = WidgetManager::Instance().createWidget("THJActionAni",pNode);
	WidgetFun::setPlaceImagicKey(pTemp,"BuAni");
}

void HNMJPlayer::runAniChi()
{
	HNMJSoundFun::playEffectByKind(GetGender(),HNMJSoundFun::EFFECT_CHI);
	cocos2d::Node* pNode = WidgetFun::getChildWidget(m_pSeatNode,"AnimateNode");
	cocos2d::Node* pTemp = WidgetManager::Instance().createWidget("THJActionAni",pNode);
	WidgetFun::setPlaceImagicKey(pTemp,"ChiAni");
}

void HNMJPlayer::runAniXiaoHu(DWORD nCode)
{
	cocos2d::Node* pNode = WidgetFun::getChildWidget(m_pSeatNode,"AnimateNode");
	float fTotal = 0.f;
	float fPosY = 0.f;
	if (nCode&CHR_XIAO_DA_SI_XI)
	{
		fTotal += 120;
	}
	if (nCode&CHR_XIAO_BAN_BAN_HU)
	{
		fTotal += 120;
	}
	if (nCode&CHR_XIAO_QUE_YI_SE)
	{
		fTotal += 120;
	}
	if (nCode&CHR_XIAO_LIU_LIU_SHUN)
	{
		fTotal += 120;
	}
	fTotal = -(fTotal - 120)/2;
	if (nCode&CHR_XIAO_DA_SI_XI)
	{
		cocos2d::Node* pTemp = WidgetManager::Instance().createWidget("THJActionAni",pNode);
		WidgetFun::setPlaceImagicKey(pTemp,"DaSiXiAni");
		pTemp->setPositionY(fPosY+fTotal);
		fPosY+=120;
	}
	if(nCode&CHR_XIAO_BAN_BAN_HU)
	{
		cocos2d::Node* pTemp = WidgetManager::Instance().createWidget("THJActionAni",pNode);
		WidgetFun::setPlaceImagicKey(pTemp,"BanBanHuAni");
		pTemp->setPositionY(fPosY+fTotal);
		fPosY+=120;
	}
	if(nCode&CHR_XIAO_QUE_YI_SE)
	{
		cocos2d::Node* pTemp = WidgetManager::Instance().createWidget("THJActionAni",pNode);
		WidgetFun::setPlaceImagicKey(pTemp,"QueYiSeAni");
		pTemp->setPositionY(fPosY+fTotal);
		fPosY+=120;
	}
	if(nCode&CHR_XIAO_LIU_LIU_SHUN)
	{
		cocos2d::Node* pTemp = WidgetManager::Instance().createWidget("THJActionAni",pNode);
		WidgetFun::setPlaceImagicKey(pTemp,"66ShunAni");
		pTemp->setPositionY(fPosY+fTotal);
		fPosY+=120;
	}
}

void HNMJPlayer::runAniNiaoFly(float fTime)
{
	cocos2d::Node* pNiaoNode =	WidgetFun::getChildWidget(m_pSeatNode,"NiaoFlyNode");
	WidgetFun::setWidgetUserInfo(pNiaoNode,"ShowDelayTime",utility::toString(fTime));
	if (pNiaoNode)
	{
		WidgetFun::runWidgetAction(pNiaoNode,"Start");
	}
}

void HNMJPlayer::showFangZhu(bool bShow)
{
	if (WidgetFun::getChildWidget(m_pSeatNode,"FangZhu"))
	{
		WidgetFun::setVisible(m_pSeatNode,"FangZhu",bShow);
	}
}

void HNMJPlayer::showTalkState(CMD_GR_C_TableTalk* pNetInfo)
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

void HNMJPlayer::setHasCSGang()
{
	setHasCSGang(m_bHasCSGang);
}
void HNMJPlayer::setHasCSGang( bool bHasCSGang )
{
	m_bHasCSGang = bHasCSGang;
	blackAllHandCard(m_bHasCSGang);
}

void HNMJPlayer::showXiaoHu(DWORD dwCode)
{
	runAniXiaoHu(dwCode);
}


