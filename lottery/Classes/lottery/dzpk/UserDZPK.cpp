#include "UserDZPK.h"
#include "DZPKLayer.h"
#include "SoundControl.h"
#include "MyNSString.h"
#include "MyConfig.h"
#include "bjlGlobalDef.h"//by hxh
#include "ToolKit.h"

// 用于调试<add by hxh 20161027>
#if 0
bool g_LabTest=true;
bool g_LabTest2=true;
#else
bool g_LabTest=false;
bool g_LabTest2=false;
#endif

UserDZPK::UserDZPK()
{
	m_pBGMask = nullptr;
	m_pBGPlayerData = nullptr;

	m_pIconFace = nullptr;
	m_pIconNetwork = nullptr;
	m_pIconPBehavior = nullptr;
	m_pPlayerChip = nullptr;
	m_pIconVIP = nullptr;
	m_pIconWait = nullptr;
	m_pIconBanker = nullptr;
	m_pIconReady = nullptr;

	m_pLabName = nullptr;
	m_pLabGold = nullptr;

	m_pHandCard = nullptr;

	m_llCurrGold = 0;
	memset(&m_sPlayerData, 0, sizeof(m_sPlayerData));

	m_bOffline = false;
	m_bShowMask = false;
	m_eBehaviorState = EDZPKPlayerBehaviorType_Null;
}

UserDZPK::~UserDZPK()
{

}

bool UserDZPK::init()
{
	bool ret = false;
	do 
	{
		CC_BREAK_IF(!Node::init());

		initUI();
		resetPlayer();

		ret = true;
	} while (0);

	return ret;
}

void UserDZPK::initUI()
{
	MyConfig& myConfig=MyConfig::Instance();
	int platform=(myConfig.m_platform>=0 && myConfig.m_platform<MAX_PLATFORM_NUM?myConfig.m_platform:0);

	//	背景——遮罩
	m_pBGMask = Sprite::createWithSpriteFrameName(texture_name::s_info_bg_mask);
	addChild(m_pBGMask, EDZPKUserZOrder_Mask);
	setContentSize(m_pBGMask->getContentSize());
	cocos2d::Size sFSize = this->getContentSize();

	//	图标——玩家头像
	m_pIconFace = Sprite::createWithSpriteFrameName(texture_name::s_nullHead);
	addChild(m_pIconFace, EDZPKUserZOrder_Normal);

	//	倒计时
	m_pCountdown = ProgressTimer::create(Sprite::createWithSpriteFrameName(texture_name::s_cd_bg_other));
	m_pCountdown->setType(ProgressTimer::Type::RADIAL);
	m_pCountdown->setReverseDirection(true);
	addChild(m_pCountdown, EDZPKUserZOrder_Mask);

	//	图标——网络状态
	m_pIconNetwork = Sprite::createWithSpriteFrameName(texture_name::s_offline);
	m_pIconFace->addChild(m_pIconNetwork);
	m_pIconNetwork->setPosition(Vec2(sFSize.width * 0.25, sFSize.height * 0.75));

	//	图标——玩家行为
	m_pIconPBehavior = Sprite::create();
	m_pIconFace->addChild(m_pIconPBehavior);

	//	图标——VIP
	m_pIconVIP = Sprite::create();
	m_pIconFace->addChild(m_pIconVIP);
	m_pIconVIP->setPosition(Vec2(sFSize.width * 0.75, sFSize.height * 0.75));

	//	图标——盲注
	m_pIconBlind = Sprite::create();
	m_pIconFace->addChild(m_pIconBlind);

	//	图标——庄家（D玩家）
	m_pIconBanker = Sprite::createWithSpriteFrameName(texture_name::s_mk_bg_makers);
	m_pIconFace->addChild(m_pIconBanker);

	//	图标——牌型
	m_pIconCardGroupType = Sprite::create();
	m_pIconFace->addChild(m_pIconCardGroupType);

	//	图标——准备
	m_pIconReady = Sprite::create(texture_name::s_ready);
	m_pIconFace->addChild(m_pIconReady);

	//	背景——玩家信息
	m_pBGPlayerData = Sprite::createWithSpriteFrameName(texture_name::s_info_bg_chipvalue);
	m_pIconFace->addChild(m_pBGPlayerData, EDZPKUserZOrder_Normal);
	m_pBGPlayerData->setPosition(Vec2(sFSize.width * 0.5, sFSize.height * 0.15));
	cocos2d::Size sBGPDSize = m_pBGPlayerData->getContentSize();

	//	图标——等待下一局
	m_pIconWait = Sprite::createWithSpriteFrameName(texture_name::s_next_round);
	m_pBGPlayerData->addChild(m_pIconWait);
	m_pIconWait->setPosition(Vec2(sBGPDSize / 2));

	//	文本——名称
	m_pLabName = Label::create();
	m_pBGPlayerData->addChild(m_pLabName);
	m_pLabName->setPosition(Vec2(sBGPDSize.width * 0.5, sBGPDSize.height * 0.7));
	m_pLabName->setSystemFontSize(25);
	m_pLabName->setTextColor(platform==0?Color4B::WHITE:Color4B::RED);

	//	文本——玩家持有金币
	m_pLabGold = Label::create();
	m_pBGPlayerData->addChild(m_pLabGold);
	m_pLabGold->setPosition(Vec2(sBGPDSize.width * 0.5, sBGPDSize.height * 0.3));
	m_pLabGold->setSystemFontSize(25);
	m_pLabGold->setTextColor(Color4B::YELLOW);

	m_pLabTest = Label::create();
	m_pIconFace->addChild(m_pLabTest);
	m_pLabTest->setSystemFontSize(22);
	m_pLabTest->setTextColor(Color4B::WHITE);

	m_pLabTest2= Label::create();
	m_pIconFace->addChild(m_pLabTest2);
	m_pLabTest2->setSystemFontSize(22);
	m_pLabTest2->setTextColor(Color4B::WHITE);

	//	筹码
	m_pPlayerChip = DZPKPlayerChip::create();
	this->addChild(m_pPlayerChip);

	//	玩家手牌
	m_pHandCard = DZPKHandCard::create();
	m_pIconFace->addChild(m_pHandCard, EDZPKUserZOrder_Card);
}

void UserDZPK::resetPlayer()
{
	m_llCurrGold = 0;
	memset(&m_sPlayerData, 0, sizeof(m_sPlayerData));
	m_bOffline = false;
	m_bShowMask = false;

	m_pIconFace->setSpriteFrame(texture_name::s_nullHead);
	m_pIconNetwork->setVisible(m_bOffline);
	m_pIconPBehavior->setVisible(false);
	m_pPlayerChip->setVisible(false);
	m_pIconVIP->setVisible(false);
	m_pIconWait->setVisible(false);
	m_pIconBlind->setVisible(false);
	m_pIconBanker->setVisible(false);
	m_pIconCardGroupType->setVisible(false);
	m_pIconReady->setVisible(false);
	m_pBGPlayerData->setVisible(false);
	m_pLabName->setVisible(false);
	m_pLabGold->setVisible(false);
	m_pBGMask->setVisible(m_bShowMask);
// 	m_pHandCard->setVisible(false);
	m_pLabTest->setVisible(false);
}

void UserDZPK::autoPositioning()
{
	cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();
	cocos2d::Size sFSize = this->getContentSize();
	switch (m_eViewID)
	{
	case EDZPKPlayerViewID_0:
		{
			m_pHandCard->setPosition(Vec2(0, sFSize.height * 0.5));
			m_pHandCard->setScale(DZPK_HAND_CARD_PRIVATE_SCALE);
			m_pIconBanker->setPosition(Vec2(sFSize.width, sFSize.height * 0.5));
			m_pIconBlind->setPosition(Vec2(sFSize.width * 0.5, -sFSize.height * 0.2));
			m_pIconPBehavior->setPosition(Vec2(sFSize.width * 0.5, sFSize.height * 1.15));
			m_pIconCardGroupType->setPosition(Vec2(sFSize.width * 0.5, sFSize.height * 1.15));
			m_pIconReady->setPosition(Vec2(sFSize.width * 0.5, sFSize.height * 1.15));
			m_pLabTest->setPosition(Vec2(sFSize.width * 0.5, sFSize.height * 1));
			m_pLabTest2->setPosition(Vec2(sFSize.width * 0.5, sFSize.height * 1.35));
			setPosition(Vec2(sVSize.width * 0.9, sVSize.height * 0.7));

			Vec2 vBetPos, vChipPotPos, vPlayerPos;
			vBetPos = Vec2(sFSize.width * -1, sFSize.height * -0.4);
			vChipPotPos = m_pPlayerChip->convertToNodeSpace(m_vChipPotPosition);
			vPlayerPos = m_pPlayerChip->convertToNodeSpace(this->getPosition());
			m_pPlayerChip->setChipPosition(vBetPos, vPlayerPos, vChipPotPos);
		}
		break;
	case EDZPKPlayerViewID_1:
		{
			m_pHandCard->setPosition(Vec2(0, sFSize.height * 0.5));
			m_pHandCard->setScale(DZPK_HAND_CARD_PRIVATE_SCALE);
			m_pIconBanker->setPosition(Vec2(sFSize.width, sFSize.height * 0.5));
			m_pIconBlind->setPosition(Vec2(sFSize.width * 0.5, -sFSize.height * 0.2));
			m_pIconPBehavior->setPosition(Vec2(sFSize.width * 0.5, sFSize.height * 1.15));
			m_pIconCardGroupType->setPosition(Vec2(sFSize.width * 0.5, sFSize.height * 1.15));
			m_pIconReady->setPosition(Vec2(sFSize.width * 0.5, sFSize.height * 1.15));
			m_pLabTest->setPosition(Vec2(sFSize.width * 0.5, sFSize.height * 1));
			m_pLabTest2->setPosition(Vec2(sFSize.width * 0.5, sFSize.height * 1.35));
			setPosition(Vec2(sVSize.width * 0.9, sVSize.height * 0.3));

			Vec2 vBetPos, vChipPotPos, vPlayerPos;
			vBetPos = Vec2(sFSize.width * -1, sFSize.height * 0.6);
			vChipPotPos = m_pPlayerChip->convertToNodeSpace(m_vChipPotPosition);
			vPlayerPos = m_pPlayerChip->convertToNodeSpace(this->getPosition());
			m_pPlayerChip->setChipPosition(vBetPos, vPlayerPos, vChipPotPos);
		}
		break;
	case EDZPKPlayerViewID_2:
		{
			m_pHandCard->setPosition(Vec2(0, sFSize.height * 0.5));
			m_pHandCard->setScale(DZPK_HAND_CARD_PRIVATE_SCALE);
			m_pIconBanker->setPosition(Vec2(sFSize.width, sFSize.height * 0.5));
			m_pIconBlind->setPosition(Vec2(-sFSize.width * 0.3, sFSize.height * 0.05));
			m_pIconPBehavior->setPosition(Vec2(sFSize.width * 0.5, sFSize.height * 1.15));
			m_pIconCardGroupType->setPosition(Vec2(sFSize.width * 0.5, sFSize.height * 1.15));
			m_pIconReady->setPosition(Vec2(sFSize.width * 0.5, sFSize.height * 1.15));
			m_pLabTest->setPosition(Vec2(sFSize.width * 0.5, sFSize.height * 1));
			m_pLabTest2->setPosition(Vec2(sFSize.width * 0.5, sFSize.height * 1.35));
			setPosition(Vec2(sVSize.width * 0.7, sVSize.height * 0.25));

			Vec2 vBetPos, vChipPotPos, vPlayerPos;
			vBetPos = Vec2(sFSize.width * -0.5, sFSize.height * 0.7);
			vChipPotPos = m_pPlayerChip->convertToNodeSpace(m_vChipPotPosition);
			vPlayerPos = m_pPlayerChip->convertToNodeSpace(this->getPosition());
			m_pPlayerChip->setChipPosition(vBetPos, vPlayerPos, vChipPotPos);
		}
		break;
	case EDZPKPlayerViewID_3:
		{
			m_pHandCard->setPosition(Vec2(sFSize.width * 1.3, sFSize.height * 0.5));
			m_pHandCard->setScale(DZPK_HAND_CARD_PUBLIC_SCALE);
			m_pIconBanker->setPosition(Vec2(0, sFSize.height * 0.5));
			m_pIconBlind->setPosition(Vec2(sFSize.width * 1.3, sFSize.height * 0.05));
			m_pIconPBehavior->setPosition(Vec2(sFSize.width * 0.5, sFSize.height * 1.15));
			m_pIconCardGroupType->setPosition(Vec2(sFSize.width * 0.5, sFSize.height * 1.15));
			m_pIconReady->setPosition(Vec2(sFSize.width * 0.5, sFSize.height * 1.15));
			m_pLabTest->setPosition(Vec2(sFSize.width * 0.5, sFSize.height * 1));
			m_pLabTest2->setPosition(Vec2(sFSize.width * 0.5, sFSize.height * 1.35));
			setPosition(Vec2(sVSize.width * 0.3, sVSize.height * 0.25));

			Vec2 vBetPos, vChipPotPos, vPlayerPos;
			vBetPos = Vec2(sFSize.width * 0.5, sFSize.height * 0.7);
			vChipPotPos = m_pPlayerChip->convertToNodeSpace(m_vChipPotPosition);
			vPlayerPos = m_pPlayerChip->convertToNodeSpace(this->getPosition());
			m_pPlayerChip->setChipPosition(vBetPos, vPlayerPos, vChipPotPos);
		}
		break;
	case EDZPKPlayerViewID_4:
		{
			m_pHandCard->setPosition(Vec2(sFSize.width, sFSize.height * 0.5));
			m_pHandCard->setScale(DZPK_HAND_CARD_PRIVATE_SCALE);
			m_pIconBanker->setPosition(Vec2(0, sFSize.height * 0.5));
			m_pIconBlind->setPosition(Vec2(sFSize.width * 0.5, -sFSize.height * 0.2));
			m_pIconPBehavior->setPosition(Vec2(sFSize.width * 0.5, sFSize.height * 1.15));
			m_pIconCardGroupType->setPosition(Vec2(sFSize.width * 0.5, sFSize.height * 1.15));
			m_pIconReady->setPosition(Vec2(sFSize.width * 0.5, sFSize.height * 1.15));
			m_pLabTest->setPosition(Vec2(sFSize.width * 0.5, sFSize.height * 1));
			m_pLabTest2->setPosition(Vec2(sFSize.width * 0.5, sFSize.height * 1.35));
			setPosition(Vec2(sVSize.width * 0.08, sVSize.height * 0.3));

			Vec2 vBetPos, vChipPotPos, vPlayerPos;
			vBetPos = Vec2(sFSize.width * 1, sFSize.height * 0.6);
			vChipPotPos = m_pPlayerChip->convertToNodeSpace(m_vChipPotPosition);
			vPlayerPos = m_pPlayerChip->convertToNodeSpace(this->getPosition());
			m_pPlayerChip->setChipPosition(vBetPos, vPlayerPos, vChipPotPos);
		}
		break;
	case EDZPKPlayerViewID_5:
		{
			m_pHandCard->setPosition(Vec2(sFSize.width, sFSize.height * 0.5));
			m_pHandCard->setScale(DZPK_HAND_CARD_PRIVATE_SCALE);
			m_pIconBanker->setPosition(Vec2(0, sFSize.height * 0.5));
			m_pIconBlind->setPosition(Vec2(sFSize.width * 0.5, -sFSize.height * 0.2));
			m_pIconPBehavior->setPosition(Vec2(sFSize.width * 0.5, sFSize.height * 1.15));
			m_pIconCardGroupType->setPosition(Vec2(sFSize.width * 0.5, sFSize.height * 1.15));
			m_pIconReady->setPosition(Vec2(sFSize.width * 0.5, sFSize.height * 1.15));
			m_pLabTest->setPosition(Vec2(sFSize.width * 0.5, sFSize.height * 1));
			m_pLabTest2->setPosition(Vec2(sFSize.width * 0.5, sFSize.height * 1.35));
			setPosition(Vec2(sVSize.width * 0.08, sVSize.height * 0.7));

			Vec2 vBetPos, vChipPotPos, vPlayerPos;
			vBetPos = Vec2(sFSize.width * 1, sFSize.height * -0.4);
			vChipPotPos = m_pPlayerChip->convertToNodeSpace(m_vChipPotPosition);
			vPlayerPos = m_pPlayerChip->convertToNodeSpace(this->getPosition());
			m_pPlayerChip->setChipPosition(vBetPos, vPlayerPos, vChipPotPos);
		}
		break;
	case EDZPKPlayerViewID_6:
		{
			m_pHandCard->setPosition(Vec2(sFSize.width, sFSize.height * 0.5));
			m_pHandCard->setScale(DZPK_HAND_CARD_PRIVATE_SCALE);
			m_pIconBanker->setPosition(Vec2(0, sFSize.height * 0.5));
			m_pIconBlind->setPosition(Vec2(sFSize.width * 1.3, sFSize.height * 0.05));
			m_pIconPBehavior->setPosition(Vec2(sFSize.width * 0.5, -sFSize.height * 0.2));
			m_pIconCardGroupType->setPosition(Vec2(sFSize.width * 0.5, -sFSize.height * 0.2));
			m_pIconReady->setPosition(Vec2(sFSize.width * 0.5, -sFSize.height * 0.2));
			m_pLabTest->setPosition(Vec2(sFSize.width * 0.5, 0));
			m_pLabTest2->setPosition(Vec2(sFSize.width * 0.5, -sFSize.height * 0.35));
			setPosition(Vec2(sVSize.width * 0.3, sVSize.height * 0.87));

			Vec2 vBetPos, vChipPotPos, vPlayerPos;
			vBetPos = Vec2(sFSize.width * 0.5, sFSize.height * -0.8);
			vChipPotPos = m_pPlayerChip->convertToNodeSpace(m_vChipPotPosition);
			vPlayerPos = m_pPlayerChip->convertToNodeSpace(this->getPosition());
			m_pPlayerChip->setChipPosition(vBetPos, vPlayerPos, vChipPotPos);
		}
		break;
	case EDZPKPlayerViewID_7:
		{
			m_pHandCard->setPosition(Vec2(0, sFSize.height * 0.5));
			m_pHandCard->setScale(DZPK_HAND_CARD_PRIVATE_SCALE);
			m_pIconBanker->setPosition(Vec2(sFSize.width, sFSize.height * 0.5));
			m_pIconBlind->setPosition(Vec2(-sFSize.width * 0.3, sFSize.height * 0.05));
			m_pIconPBehavior->setPosition(Vec2(sFSize.width * 0.5, -sFSize.height * 0.2));
			m_pIconCardGroupType->setPosition(Vec2(sFSize.width * 0.5, -sFSize.height * 0.2));
			m_pIconReady->setPosition(Vec2(sFSize.width * 0.5, -sFSize.height * 0.2));
			m_pLabTest->setPosition(Vec2(sFSize.width * 0.5, 0));
			m_pLabTest2->setPosition(Vec2(sFSize.width * 0.5, -sFSize.height * 0.35));
			setPosition(Vec2(sVSize.width * 0.7, sVSize.height * 0.87));

			Vec2 vBetPos, vChipPotPos, vPlayerPos;
			vBetPos = Vec2(sFSize.width * -0.5, sFSize.height * -0.8);
			vChipPotPos = m_pPlayerChip->convertToNodeSpace(m_vChipPotPosition);
			vPlayerPos = m_pPlayerChip->convertToNodeSpace(this->getPosition());
			m_pPlayerChip->setChipPosition(vBetPos, vPlayerPos, vChipPotPos);
		}
		break;
	default:
		break;
	}
}

void UserDZPK::setPlayerInfor(tagUserData sUserData)
{
	TGender gender = sUserData.cbGender;
	tagUserData* pSelfData = EntityMgr::instance()->roomFrame()->getMeUserDate();
	string sPlayerName = sUserData.szNickName;
	if (sUserData.wChairID != pSelfData->wChairID)
	{
		sPlayerName = ToolKit::dealStringOfPrivacy(sUserData.szNickName);
	}

	m_pLabName->setString(sPlayerName);
	do 
	{
		if (m_sPlayerData.cbUserStatus == US_PLAYING && m_sPlayerData.cbUserStatus == sUserData.cbUserStatus)
		{
			/*	游戏开始后会再发一次用户信息过来，这会重置持有金币额
			**	从而导致游戏进行中的持有金币额不正确
			**	这里作特殊处理
			*/
			break;
		}
		setPlayerGold(sUserData.lScore);
	} while (0);
	WORD wFaceID = sUserData.wFaceID;
	int nLevel = sUserData.cbMemberOrder;
	int nVIPLevel = (0 < nLevel && nLevel < 7) ? nLevel : 1;

	static const char *szArr[9]={"离开","站立","坐下","同意","旁观","游戏","断线","US_AGAIN_GAME","US_LEFT"};
	string str=String::createWithFormat("%d(%d),%d[%s],%d,%d,%lld",
		sUserData.wChairID,
		m_eViewID,
		sUserData.cbUserStatus,
		szArr[sUserData.cbUserStatus],
		sUserData.wFaceID,
		sUserData.cbGender,
		(LONGLONG)sUserData.lScore)->getCString();
	MyNSString::GBKToUTF8(str);
	m_pLabTest->setString(str);

	//	玩家头像
	char buf[256];
	if (wFaceID==0xff && gender==0xff)
	{
		sprintf(buf,texture_name::s_nullHead);
	}
	else if (gender == 1)	//	男
	{
		wFaceID = wFaceID%6;
		wFaceID++;
		sprintf(buf,texture_name::s_tx_boy,wFaceID);
	}
	else
	{
		wFaceID -= 21;	
		sprintf(buf,texture_name::s_tx_girl,wFaceID % 6 + 1);
	}
	m_pIconFace->setSpriteFrame(buf);

	//	VIP
	char fileNameVIP[32];
	sprintf(fileNameVIP, texture_name::s_table_level, nVIPLevel);
	m_pIconVIP->setSpriteFrame(fileNameVIP);

	m_sPlayerData = sUserData;
	dealPlayerState();
}

void UserDZPK::setPlayerGold(LONGLONG lScore)
{
	m_llCurrGold = lScore;
	char cBufGold[128];
	sprintf(cBufGold, "%lld", m_llCurrGold);
	m_pLabGold->setString(cBufGold);
}

LONGLONG UserDZPK::getPlayerGold() const
{
	return m_llCurrGold;
}

void UserDZPK::updateTest(const char* szName,int idx)
{
	m_pLabTest2->setString(szName);
}

void UserDZPK::dealPlayerState()
{
	switch (m_sPlayerData.cbUserStatus)
	{
	case US_NULL:	//	离开
		{
			resetPlayer();
			m_pLabTest->setVisible(g_LabTest);
		}
		break;
	case US_FREE:
		{
			resetPlayer();
			m_pLabTest->setVisible(g_LabTest);
		}
		break;
	case US_SIT:
		{
			m_bOffline = false;
			m_bShowMask = false;
			showNormal();
			SoundControl::sharedSoundControl()->playSoundEffect(sound_name::s_dzpk_gangjinru);
		}
		break;
	case US_READY:
		{
			m_bOffline = false;
			m_bShowMask = false;
			m_pIconReady->setVisible(true);
		}
		break;
	case US_LOOKON:
		break;
	case US_PLAYING:
		{
			m_bOffline = false;
			m_bShowMask = false;
			showNormal();
		}
		break;
	case US_OFFLINE:
		{
			m_bOffline = true;
			m_bShowMask = true;
			showOffline();
		}
		break;
	case US_AGAIN_GAME:
		break;
	case US_LEFT:
		{
			resetPlayer();
			m_pLabTest->setVisible(g_LabTest);
		}
		break;
	default:
		break;
	}
}

//	设置视图ID
void UserDZPK::setViewID(EDZPKPlayerViewID eViewID)
{
	m_eViewID = eViewID;
	autoPositioning();
}

//	获取视图ID
EDZPKPlayerViewID UserDZPK::getViewID() const
{
	return m_eViewID;
}

void UserDZPK::resetHandCard()
{
	m_pHandCard->resetHandCard();
	if (m_eViewID != EDZPKPlayerViewID_3)
	{
		m_pHandCard->setScale(DZPK_HAND_CARD_PRIVATE_SCALE);
	}
}

void UserDZPK::setHandCard(int nCardID, unsigned char cCardData)
{
	m_pHandCard->setHandCardImg(nCardID, cCardData);
}

void UserDZPK::onBehavior(EDZPKPlayerBehaviorType eType)
{
	//	玩家行为处理
	switch (eType)
	{
	case EDZPKPlayerBehaviorType_Check:
		break;
	case EDZPKPlayerBehaviorType_Call:
		break;
	case EDZPKPlayerBehaviorType_Raise:
		break;
	case EDZPKPlayerBehaviorType_Bet:
		break;
	case EDZPKPlayerBehaviorType_Fold:
		onFold();
		break;
	case EDZPKPlayerBehaviorType_AllIn:
		break;
	case EDZPKPlayerBehaviorType_Null:
		break;
	default:
		return;
	}
	cancelCountdown();
	//	设置行为状态
	setBehaviorState(eType);
	//	显示行为提示
	showBehaviorTips(eType);
}

void UserDZPK::showBehaviorTips(EDZPKPlayerBehaviorType eType)
{
	//	播放行为提示音效
	playBehaviorMusicEffect(eType, m_sPlayerData.cbGender);

	//	播放行为提示动画
	switch (eType)
	{
	case EDZPKPlayerBehaviorType_Check:
		m_pIconPBehavior->setSpriteFrame(texture_name::s_action_check);
		break;
	case EDZPKPlayerBehaviorType_Call:
		m_pIconPBehavior->setSpriteFrame(texture_name::s_action_call);
		break;
	case EDZPKPlayerBehaviorType_Raise:
		m_pIconPBehavior->setSpriteFrame(texture_name::s_action_raise);
		break;
	case EDZPKPlayerBehaviorType_Bet:
		m_pIconPBehavior->setSpriteFrame(texture_name::s_action_bet);
		break;
	case EDZPKPlayerBehaviorType_Fold:
		{
			m_pIconPBehavior->setSpriteFrame(texture_name::s_action_fold);
			m_pIconPBehavior->setVisible(true);
			m_pBGMask->setVisible(true);
			return;
		}
		break;
	case EDZPKPlayerBehaviorType_AllIn:
		m_pIconPBehavior->setSpriteFrame(texture_name::s_action_allin);
		break;
	default:
		return;
	}
	auto pDelay = DelayTime::create(2.0f);
	auto pFadeOut = FadeOut::create(1.0f);
	auto pCallHide = CallFunc::create([=](){
		m_pIconPBehavior->setVisible(false);
	});
	auto pSeq = Sequence::create(pDelay, pFadeOut, pCallHide, nullptr);
	m_pIconPBehavior->setVisible(true);
	m_pIconPBehavior->runAction(pSeq);
}

void UserDZPK::playBehaviorMusicEffect(EDZPKPlayerBehaviorType eType, char cGender)
{
	switch (eType)
	{
	case EDZPKPlayerBehaviorType_Check:
		if (cGender == 1)
		{
			SoundControl::sharedSoundControl()->playSoundEffect(sound_name::s_dzpk_man_guo);
		}
		else
		{
			SoundControl::sharedSoundControl()->playSoundEffect(sound_name::s_dzpk_women_guo);
		}
		break;
	case EDZPKPlayerBehaviorType_Call:
	case EDZPKPlayerBehaviorType_Raise:
		SoundControl::sharedSoundControl()->playSoundEffect(sound_name::s_dzpk_chouma);
		break;
	case EDZPKPlayerBehaviorType_Bet:
		if (cGender == 1)
		{
			SoundControl::sharedSoundControl()->playSoundEffect(sound_name::s_dzpk_man_xz);
		}
		else
		{
			SoundControl::sharedSoundControl()->playSoundEffect(sound_name::s_dzpk_women_xz);
		}
		break;
	case EDZPKPlayerBehaviorType_Fold:
		if (cGender == 1)
		{
			SoundControl::sharedSoundControl()->playSoundEffect(sound_name::s_dzpk_man_giveover);
		}
		else
		{
			SoundControl::sharedSoundControl()->playSoundEffect(sound_name::s_dzpk_women_giveover);
		}
		break;
	case EDZPKPlayerBehaviorType_AllIn:
		if (cGender == 1)
		{
			SoundControl::sharedSoundControl()->playSoundEffect(sound_name::s_dzpk_man_qx);
		}
		else
		{
			SoundControl::sharedSoundControl()->playSoundEffect(sound_name::s_dzpk_women_qx);
		}
		break;
	default:
		break;
	}
}

void UserDZPK::showBlind(EDZPKBlindType eType)
{
	switch (eType)
	{
	case EDZPKBlindType_Big:
		m_pIconBlind->setSpriteFrame(texture_name::s_big_blind);
		break;
	case EDZPKBlindType_small:
		m_pIconBlind->setSpriteFrame(texture_name::s_small_blind);
		break;
	default:
		break;
	}
	m_pIconBlind->setVisible(true);
}

void UserDZPK::hideBlind()
{
	m_pIconBlind->setVisible(false);
}

//	展示牌型标识
void UserDZPK::showCardGroupType(BYTE bCardType)
{
	if(bCardType < 1 || bCardType > 9)
	{
		return;
	}

	static const char *szArr[9]={texture_name::s_cardtype_gaopai,
		texture_name::s_cardtype_yidui,
		texture_name::s_cardtype_liangdui,
		texture_name::s_cardtype_santiao,
		texture_name::s_cardtype_shunzi,
		texture_name::s_cardtype_tonghua,
		texture_name::s_cardtype_hulu,
		texture_name::s_cardtype_sitiao,
		texture_name::s_cardtype_tonghuashun};
	m_pIconCardGroupType->setSpriteFrame(szArr[bCardType - 1]);
	m_pIconCardGroupType->setVisible(true);
}

void UserDZPK::showWaiting()
{
	m_pIconWait->setVisible(true);
	m_pLabName->setVisible(false);
	m_pLabGold->setVisible(false);
}

void UserDZPK::showDPlayer(bool bIsDPlayer)
{
	m_pIconBanker->setVisible(bIsDPlayer);
}

void UserDZPK::showNormal()
{
	m_pIconReady->setVisible(false);
	m_pBGPlayerData->setVisible(true);
	m_pLabName->setVisible(true);
	m_pLabGold->setVisible(true);
	m_pIconVIP->setVisible(true);
	m_pIconNetwork->setVisible(m_bOffline);
	m_pBGMask->setVisible(m_bShowMask);
	m_pIconPBehavior->setVisible(false);
	m_pIconWait->setVisible(false);
	m_pIconVIP->setVisible(true);
	m_pLabTest->setVisible(g_LabTest);
	m_pLabTest2->setVisible(g_LabTest2);
}

//	展示掉线状态
void UserDZPK::showOffline()
{
	m_pIconNetwork->setVisible(m_bOffline);
	m_pBGMask->setVisible(m_bShowMask);
}

//	展示准备标识
void UserDZPK::dealReady()
{
	m_pIconBanker->setVisible(false);
	m_pIconCardGroupType->setVisible(false);
	resetHandCard();
}

void UserDZPK::onFold()
{
	aniFold();
	cancelCountdown();
}

void UserDZPK::aniFold()
{
	//	弃牌动作
	float fDelayValue = 0;
	for (int i = 0; i < 2; i++)
	{
		Scene* pScene = Director::getInstance()->getRunningScene();
		DZPKCard* pAniCard = DZPKCard::create();
		this->addChild(pAniCard);
		pAniCard->setScale(0.5);
		//	玩家所在位置作为起点位置
		Vec2 vStartPos = pAniCard->convertToNodeSpace(this->getPosition());
		//	筹码池位置作为终点位置
		Vec2 vEndPos = convertToNodeSpace(m_vChipPotPosition);
		pAniCard->setPosition(vStartPos);

		//	弃牌动作
		auto pDelay = DelayTime::create(fDelayValue);
		auto pMove = MoveTo::create(0.5f, vEndPos);
		auto pRemove = RemoveSelf::create();
		auto pSeq = Sequence::create(pDelay, pMove, pRemove, nullptr);

		pAniCard->runAction(pSeq);
		fDelayValue += 0.3;
	}
	if (m_eViewID == EDZPKPlayerViewID_3)
	{
		m_pHandCard->m_pImgHandCard[EDZPKCardID_1]->showCardDark();
		m_pHandCard->m_pImgHandCard[EDZPKCardID_2]->showCardDark();
	}
	else
	{
		m_pHandCard->setCardVisible(EDZPKCardID_1, false);
		m_pHandCard->setCardVisible(EDZPKCardID_2, false);
	}
}

//	获取用户信息
tagUserData UserDZPK::getPlayerData() const
{
	return m_sPlayerData;
}

void UserDZPK::showCountdown()
{
	m_pCountdown->setVisible(true);
	CallFunc *pCallSelf = CallFunc::create([=]()
	{

		if (m_eViewID == MY_VIEW_CHAIR_ID)
		{
			CMD_C_NULL cNull;
			EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_GIVE_UP,&cNull,sizeof(cNull));
		}
		cancelCountdown();
	});
	m_pCountdown->runAction(Sequence::create(
		ProgressFromTo::create(MyConfig::Instance().m_t2, 100, 0),
		pCallSelf,
		NULL));
}

void UserDZPK::cancelCountdown()
{
	if (m_eBehaviorState == EDZPKPlayerBehaviorType_Fold)
	{
		log("%d", m_eBehaviorState);
	}
	this->setBehaviorState(EDZPKPlayerBehaviorType_Null);
	m_pCountdown->stopAllActions();
	m_pCountdown->setVisible(false);
}

