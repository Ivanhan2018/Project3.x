#include "BRNNGameLayer.h"
#include "BRNNGameScene.h"
#include "BRNNDialogQuit.h"
#include "BRNNProtocol.h"
#include "EntityMgr.h"
#include "BRNNTool.h"
#include "BRNNGameLogic.h"
#include "ToolKit.h"

BRNNGameLayer::BRNNGameLayer():
	m_eCurrChipType(E_BRNNChip_NULL),
	m_eApplyState(E_BRNN_Apply_State_Null)
{
}

BRNNGameLayer::~BRNNGameLayer()
{
}

void BRNNGameLayer::resetGameLayer()
{
	m_eCurrChipType = E_BRNNChip_NULL;
	stopAllActions();

	for (int i = E_BRNNBetArea_E; i < E_BRNNBetArea_NULL; i++)
	{
		m_pBetArea[i]->resetTableArea();
	}

	for (int i = E_BRNNChip_1; i < E_BRNNChip_NULL; i++)
	{
		m_pChipHighLight[i]->setVisible(false);
	}

	for (int i = E_BRNN_PokerPos_B; i < E_BRNN_PokerPos_NULL; i++)
	{
		m_pHandCard[i]->resetHandsCard();
	}
}

float BRNNGameLayer::doAniSendCard(const int nIdx, const BYTE sCardData[][5])
{
	int nBeginIdx = nIdx;
	float fDelay = 0.0f;

	//	发牌

	for (int i = 0; i < BRNN_HANDS_CARD_COUNT; i++)
	{
		for (int j = E_BRNN_PokerPos_B; j < E_BRNN_PokerPos_NULL; j++)
		{
			m_pHandCard[nBeginIdx]->initHandsCard(i, sCardData[nBeginIdx][i]);
			auto pDelayTime = DelayTime::create(fDelay);
			auto pSendCard = CallFunc::create([=]()
			{
				m_pHandCard[nBeginIdx]->m_pPokerCard[i]->setVisible(true);
				if (i != 3)
				{
					m_pHandCard[nBeginIdx]->m_pPokerCard[i]->showCardObverse(false);
				}
			});
			auto pSeq = Sequence::create(pDelayTime, pSendCard, nullptr);
			runAction(pSeq);

			fDelay += 0.3f;
			if (++nBeginIdx >= E_BRNN_PokerPos_NULL)
			{
				nBeginIdx = 0;
			}
		}
	}

	//	翻牌

	for (int i = 0, j = E_BRNN_PokerPos_E; i < E_BRNN_PokerPos_NULL; i++)
	{
		auto pDelayTime = DelayTime::create(fDelay);
		auto pCallfunc = CallFunc::create([=]()
		{
			BYTE bShowCardData[BRNN_HANDS_CARD_COUNT];
			memset(bShowCardData, 0, sizeof(bShowCardData));
			BYTE bTmpShowCardData[BRNN_HANDS_CARD_COUNT];
			memset(bTmpShowCardData, 0, sizeof(bTmpShowCardData));
			E_BRNN_CardType eCardType = BRNNGameLogic::GetCardType(m_pHandCard[j]->m_cbCardData, BRNN_HANDS_CARD_COUNT, bTmpShowCardData);
			if (eCardType == E_BRNN_CardType_Error)
			{
				return;
			}
			m_pHandCard[j]->setCardType(eCardType);

			if (eCardType >= E_BRNN_CardType_Niu1)
			{
				bShowCardData[0] = bTmpShowCardData[3];
				bShowCardData[1] = bTmpShowCardData[4];
				bShowCardData[2] = bTmpShowCardData[0];
				bShowCardData[3] = bTmpShowCardData[1];
				bShowCardData[4] = bTmpShowCardData[2];				
			}
			else
			{
				memcpy(bShowCardData, bTmpShowCardData, sizeof(bTmpShowCardData));
			}

			m_pHandCard[j]->m_pPokerCard[3]->showCardObverse(true);
			m_pHandCard[j]->initHandsCard(bShowCardData);
			auto pDelayShowType = DelayTime::create(0.3f);
			auto pShowCardType = CallFunc::create([=]()
			{
				if (eCardType >= E_BRNN_CardType_Niu1)
				{
					m_pHandCard[j]->showCardMarkedness();
				}
				m_pHandCard[j]->showCardType(eCardType);
			});
			auto pSeq = Sequence::create(pDelayShowType, pShowCardType, nullptr);
			m_pHandCard[j]->runAction(pSeq);
		});
		auto pSeq = Sequence::create(pDelayTime, pCallfunc, nullptr);
		runAction(pSeq);

		fDelay += 0.5f;
		if (++j >= E_BRNN_PokerPos_NULL)
		{
			j = E_BRNN_PokerPos_B;
		}
	}

	return fDelay;
}

void BRNNGameLayer::bet(E_BRNNBetAreaType eAreaType, SCORE sValue)
{
	E_BRNNChipType eChipType = BRNNTool::convertToChipType(sValue);
	m_pBetArea[eAreaType]->addTableChip(eChipType);
	LONGLONG llSum = m_pBetArea[eAreaType]->getChipValueSum() + (LONGLONG)sValue;
	m_pBetArea[eAreaType]->setChipValueSum(llSum);
	m_pBetArea[eAreaType]->setLabChipValueSum(llSum);
}

BRNNBetArea* BRNNGameLayer::getTableArea(E_BRNNBetAreaType eType)
{
	return m_pBetArea[eType];
}

//	分配筹码
void BRNNGameLayer::allocationChip(E_BRNNBetAreaType eAreaType, SCORE sValue)
{
	LONGLONG allScore = sValue;
	if (allScore <= 0)
	{
		return;
	}
	for (int j = E_BRNNChip_1k; j >= E_BRNNChip_1; j--)
	{
		if (allScore == 0)
		{
			break;
		}
		if ( j == E_BRNNChip_1)
		{
			for (int i = 0; i < allScore; i++)
			{
				bet(eAreaType, 1);
			}
			break;
		}
		int tempBet = BRNNTool::convertToChipValue((E_BRNNChipType)j);
		int tempCount = allScore / tempBet;
		if (tempCount <= 0)
		{
			continue;
		}
		for (int z = 0; z < tempCount; z++)
		{
			bet(eAreaType, (SCORE)tempBet);
		}
		allScore = allScore % tempBet;
	}
}

void BRNNGameLayer::showAniChangeBanker(int nChairID)
{
	BRNNGameScene* pScene = BRNNGameScene::getBRNNGameScene();
	if (!pScene)
	{
		return;
	}
	cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();

	float fMoveTime = 1.0f;
	float fShowTime = 2.0f;

	int nSelfChairID = EntityMgr::instance()->roomFrame()->getMeUserDate()->wChairID;
	Sprite* pImgTips = nullptr;
	if (nChairID == INVALID_CHAIR)
	{
		pImgTips = Sprite::createWithSpriteFrameName(BRNNTextureName::s_brnn_tips_none_banker);
	}
	else if (nChairID == nSelfChairID)
	{
		pImgTips = Sprite::createWithSpriteFrameName(BRNNTextureName::s_brnn_tips_officeholding_banker);
	}
	else
	{
		pImgTips = Sprite::createWithSpriteFrameName(BRNNTextureName::s_brnn_tips_turn_banker);
	}
	pScene->addChild(pImgTips);
	pImgTips->setPosition(Vec2(sVSize.width / 2, sVSize.height + pImgTips->getContentSize().height / 2));
	auto pMoveTo = MoveTo::create(fMoveTime, Vec2(sVSize / 2));
	auto pFadeIn = FadeIn::create(fShowTime);
	auto pRemoveSelf = RemoveSelf::create();
	auto pSeq = Sequence::create(pMoveTo, pFadeIn, pRemoveSelf, nullptr);
	pImgTips->runAction(pSeq);
}

void BRNNGameLayer::setBtnChipEnable(E_BRNNChipType eType, bool bEnabled)
{
	m_pBTNChip[eType]->setEnabled(bEnabled);
}

void BRNNGameLayer::setAllBtnChipEnable(bool bEnabled)
{
	for(int i = E_BRNNChip_1; i < E_BRNNChip_NULL; i++)
	{
		m_pBTNChip[i]->setEnabled(bEnabled);
	}
}

void BRNNGameLayer::checkoutEnabledBtnChip(SCORE sValue)
{
	int nMaxMultiple = 5;
	SCORE sLimitChip1 = 1 * nMaxMultiple;
	SCORE sLimitChip5 = 5 * nMaxMultiple;
	SCORE sLimitChip10 = 10 * nMaxMultiple;
	SCORE sLimitChip50 = 50 * nMaxMultiple;
	SCORE sLimitChip100 = 100 * nMaxMultiple;
	SCORE sLimitChip200 = 200 * nMaxMultiple;
	SCORE sLimitChip500 = 500 * nMaxMultiple;
	SCORE sLimitChip1000 = 1000 * nMaxMultiple;
	if (sValue >= sLimitChip1000)
	{
		setAllBtnChipEnable(true);
	}
	else if (sValue >= sLimitChip500 && sValue < sLimitChip1000)
	{
		setBtnChipEnable(E_BRNNChip_1, true);
		setBtnChipEnable(E_BRNNChip_5, true);
		setBtnChipEnable(E_BRNNChip_10, true);
		setBtnChipEnable(E_BRNNChip_50, true);
		setBtnChipEnable(E_BRNNChip_100, true);
		setBtnChipEnable(E_BRNNChip_200, true);
		setBtnChipEnable(E_BRNNChip_500, true);
		setBtnChipEnable(E_BRNNChip_1k, false);
	}
	else if (sValue >= sLimitChip200 && sValue < sLimitChip500)
	{
		setBtnChipEnable(E_BRNNChip_1, true);
		setBtnChipEnable(E_BRNNChip_5, true);
		setBtnChipEnable(E_BRNNChip_10, true);
		setBtnChipEnable(E_BRNNChip_50, true);
		setBtnChipEnable(E_BRNNChip_100, true);
		setBtnChipEnable(E_BRNNChip_200, true);
		setBtnChipEnable(E_BRNNChip_500, false);
		setBtnChipEnable(E_BRNNChip_1k, false);
	}
	else if (sValue >= sLimitChip100 && sValue < sLimitChip200)
	{
		setBtnChipEnable(E_BRNNChip_1, true);
		setBtnChipEnable(E_BRNNChip_5, true);
		setBtnChipEnable(E_BRNNChip_10, true);
		setBtnChipEnable(E_BRNNChip_50, true);
		setBtnChipEnable(E_BRNNChip_100, true);
		setBtnChipEnable(E_BRNNChip_200, false);
		setBtnChipEnable(E_BRNNChip_500, false);
		setBtnChipEnable(E_BRNNChip_1k, false);
	}
	else if (sValue >= sLimitChip50 && sValue < sLimitChip100)
	{
		setBtnChipEnable(E_BRNNChip_1, true);
		setBtnChipEnable(E_BRNNChip_5, true);
		setBtnChipEnable(E_BRNNChip_10, true);
		setBtnChipEnable(E_BRNNChip_50, true);
		setBtnChipEnable(E_BRNNChip_100, false);
		setBtnChipEnable(E_BRNNChip_200, false);
		setBtnChipEnable(E_BRNNChip_500, false);
		setBtnChipEnable(E_BRNNChip_1k, false);
	}
	else if (sValue >= sLimitChip10 && sValue < sLimitChip100)
	{
		setBtnChipEnable(E_BRNNChip_1, true);
		setBtnChipEnable(E_BRNNChip_5, true);
		setBtnChipEnable(E_BRNNChip_10, true);
		setBtnChipEnable(E_BRNNChip_50, false);
		setBtnChipEnable(E_BRNNChip_100, false);
		setBtnChipEnable(E_BRNNChip_200, false);
		setBtnChipEnable(E_BRNNChip_500, false);
		setBtnChipEnable(E_BRNNChip_1k, false);
	}
	else if (sValue >= sLimitChip5 && sValue < sLimitChip10)
	{
		setBtnChipEnable(E_BRNNChip_1, true);
		setBtnChipEnable(E_BRNNChip_5, true);
		setBtnChipEnable(E_BRNNChip_10, false);
		setBtnChipEnable(E_BRNNChip_50, false);
		setBtnChipEnable(E_BRNNChip_100, false);
		setBtnChipEnable(E_BRNNChip_200, false);
		setBtnChipEnable(E_BRNNChip_500, false);
		setBtnChipEnable(E_BRNNChip_1k, false);
	}
	else if (sValue >= sLimitChip1 && sValue < sLimitChip5)
	{
		setBtnChipEnable(E_BRNNChip_1, true);
		setBtnChipEnable(E_BRNNChip_5, false);
		setBtnChipEnable(E_BRNNChip_10, false);
		setBtnChipEnable(E_BRNNChip_50, false);
		setBtnChipEnable(E_BRNNChip_100, false);
		setBtnChipEnable(E_BRNNChip_200, false);
		setBtnChipEnable(E_BRNNChip_500, false);
		setBtnChipEnable(E_BRNNChip_1k, false);
	}
	else
	{
		setAllBtnChipEnable(false);
	}
}

//	设置坐庄状态
void BRNNGameLayer::setApplyState(E_BRNNApplyState eState)
{
	m_eApplyState = eState;
	switch (eState)
	{
	case E_BRNN_EPPC_Apply_State_Apply:
		m_pBTNApply->setVisible(false);
		m_pBTNCancelApply->setVisible(true);
		m_pBTNDown->setVisible(false);
		break;
	case E_BRNN_EPPC_Apply_State_Banker:
		m_pBTNApply->setVisible(false);
		m_pBTNCancelApply->setVisible(false);
		m_pBTNDown->setVisible(true);
		break;
	case E_BRNN_Apply_State_Null:
		m_pBTNApply->setVisible(true);
		m_pBTNCancelApply->setVisible(false);
		m_pBTNDown->setVisible(false);
		break;
	default:
		break;
	}
}

//	获取当前坐庄状态
E_BRNNApplyState BRNNGameLayer::getApplyState() const
{
	return m_eApplyState;
}

bool BRNNGameLayer::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Layer::init());

		initUI();
		setApplyState(E_BRNN_Apply_State_Null);

		//	添加触摸事件
		auto pListener = EventListenerTouchOneByOne::create();
		pListener->onTouchBegan = CC_CALLBACK_2(BRNNGameLayer::onTouchBegan, this);
		pListener->onTouchMoved = CC_CALLBACK_2(BRNNGameLayer::onTouchMoved, this);
		pListener->onTouchCancelled = CC_CALLBACK_2(BRNNGameLayer::onTouchCancelled, this);
		pListener->onTouchEnded = CC_CALLBACK_2(BRNNGameLayer::onTouchEnded, this);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pListener, this);

		bRet = true;
	} while (0);

	return bRet;
}

void BRNNGameLayer::initUI()
{
	cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();

	m_pBG = Sprite::create(BRNNTextureName::s_brnn_table_bg);
	addChild(m_pBG);
	m_pBG->setPosition(Vec2(sVSize / 2));
	cocos2d::Size sBGSize = m_pBG->getContentSize();

	m_pBTNQuit = Button::create(BRNNTextureName::s_brnn_quit_normal,
		BRNNTextureName::s_brnn_quit_selected,
		"", Widget::TextureResType::PLIST);
	addChild(m_pBTNQuit);
	m_pBTNQuit->setPosition(Vec2(sVSize.width * 0.95, sVSize.height * 0.92));
	m_pBTNQuit->addClickEventListener(CC_CALLBACK_0(BRNNGameLayer::onClickBTNQuitEvent, this));

	m_pBTNSetting = Button::create(BRNNTextureName::s_brnn_setting_normal,
		BRNNTextureName::s_brnn_setting_selected,
		"", Widget::TextureResType::PLIST);
	addChild(m_pBTNSetting);
	m_pBTNSetting->setPosition(Vec2(sVSize.width * 0.87, sVSize.height * 0.92));
	m_pBTNSetting->addClickEventListener(CC_CALLBACK_0(BRNNGameLayer::onClickBTNSettingEvent, this));

	m_pBTNTravel = Button::create(BRNNTextureName::s_brnn_btn_travel_normal,
		BRNNTextureName::s_brnn_btn_travel_selected,
		"", Widget::TextureResType::PLIST);
	addChild(m_pBTNTravel);
	m_pBTNTravel->setPosition(Vec2(sVSize.width * 0.79, sVSize.height * 0.92));
	m_pBTNTravel->addClickEventListener(CC_CALLBACK_0(BRNNGameLayer::onClickBTNTravelEvent, this));

	m_pBTNApplyList = Button::create(BRNNTextureName::s_brnn_apply_lsit_normal,
		BRNNTextureName::s_brnn_apply_lsit_pressed,
		"", Widget::TextureResType::PLIST);
	addChild(m_pBTNApplyList);
	m_pBTNApplyList->setPosition(Vec2(sVSize.width * 0.71, sVSize.height * 0.92));
	m_pBTNApplyList->addClickEventListener(CC_CALLBACK_0(BRNNGameLayer::onClickBTNApplyListEvent, this));

	m_pBTNApply = Button::create(BRNNTextureName::s_brnn_apply_normal,
		BRNNTextureName::s_brnn_apply_selected,
		"", Widget::TextureResType::PLIST);
	addChild(m_pBTNApply);
	m_pBTNApply->setPosition(Vec2(sVSize.width * 0.27, sVSize.height * 0.92));
	m_pBTNApply->addClickEventListener(CC_CALLBACK_0(BRNNGameLayer::onClickBTNApplyEvent, this));

	m_pBTNCancelApply = Button::create(BRNNTextureName::s_brnn_cancel_normal,
		BRNNTextureName::s_brnn_cancel_selected,
		"", Widget::TextureResType::PLIST);
	addChild(m_pBTNCancelApply);
	m_pBTNCancelApply->setPosition(Vec2(sVSize.width * 0.27, sVSize.height * 0.92));
	m_pBTNCancelApply->addClickEventListener(CC_CALLBACK_0(BRNNGameLayer::onClickBTNCancelApplyEvent, this));

	m_pBTNDown = Button::create(BRNNTextureName::s_brnn_down_normal,
		BRNNTextureName::s_brnn_down_selected,
		"", Widget::TextureResType::PLIST);
	addChild(m_pBTNDown);
	m_pBTNDown->setPosition(Vec2(sVSize.width * 0.27, sVSize.height * 0.92));
	m_pBTNDown->addClickEventListener(CC_CALLBACK_0(BRNNGameLayer::onClickBTNDownEvent, this));

	//	初始化下注区域

	Vec2 vBetAreaPos[E_BRNNBetArea_NULL] =
	{
		Vec2(sBGSize.width * 0.15, sBGSize.height * 0.5),
		Vec2(sBGSize.width * 0.3833, sBGSize.height * 0.5),
		Vec2(sBGSize.width * 0.6167, sBGSize.height * 0.5),
		Vec2(sBGSize.width * 0.85, sBGSize.height * 0.5)
	};

	for (int i = E_BRNNBetArea_E; i < E_BRNNBetArea_NULL; i++)
	{
		m_pBetArea[i] = BRNNBetArea::create();
		m_pBG->addChild(m_pBetArea[i]);
		m_pBetArea[i]->setPosition(vBetAreaPos[i]);
		m_pBetArea[i]->initArea((E_BRNNBetAreaType)i);
	}

	//	初始化筹码按钮

	string sBufChipNormal[E_BRNNChip_NULL] =
	{
		BRNNTextureName::s_brnn_chip_normal_1,
		BRNNTextureName::s_brnn_chip_normal_5,
		BRNNTextureName::s_brnn_chip_normal_10,
		BRNNTextureName::s_brnn_chip_normal_50,
		BRNNTextureName::s_brnn_chip_normal_100,
		BRNNTextureName::s_brnn_chip_normal_200,
		BRNNTextureName::s_brnn_chip_normal_500,
		BRNNTextureName::s_brnn_chip_normal_1k
	};

	string sBufChipDisable[E_BRNNChip_NULL] =
	{
		BRNNTextureName::s_brnn_chip_disable_1,
		BRNNTextureName::s_brnn_chip_disable_5,
		BRNNTextureName::s_brnn_chip_disable_10,
		BRNNTextureName::s_brnn_chip_disable_50,
		BRNNTextureName::s_brnn_chip_disable_100,
		BRNNTextureName::s_brnn_chip_disable_200,
		BRNNTextureName::s_brnn_chip_disable_500,
		BRNNTextureName::s_brnn_chip_disable_1k
	};

	Vec2 vPosList[E_BRNNChip_NULL] =
	{
		Vec2(sBGSize.width * 0.3, sBGSize.height * 0.1),
		Vec2(sBGSize.width * 0.3914, sBGSize.height * 0.1),
		Vec2(sBGSize.width * 0.4828, sBGSize.height * 0.1),
		Vec2(sBGSize.width * 0.5742, sBGSize.height * 0.1),
		Vec2(sBGSize.width * 0.6657, sBGSize.height * 0.1),
		Vec2(sBGSize.width * 0.7571, sBGSize.height * 0.1),
		Vec2(sBGSize.width * 0.8485, sBGSize.height * 0.1),
		Vec2(sBGSize.width * 0.94, sBGSize.height * 0.1)
	};

	for (int i = E_BRNNChip_1; i < E_BRNNChip_NULL; i++)
	{
		m_pBTNChip[i] = Button::create(sBufChipNormal[i], sBufChipNormal[i], sBufChipDisable[i], TextureResType::PLIST);
		addChild(m_pBTNChip[i]);
		m_pBTNChip[i]->setPosition(vPosList[i]);
		m_pBTNChip[i]->addClickEventListener(CC_CALLBACK_1(BRNNGameLayer::onClickBTNChipEvent, this));

		m_pChipHighLight[i] = Sprite::createWithSpriteFrameName(BRNNTextureName::s_brnn_chip_high_light);
		m_pBTNChip[i]->addChild(m_pChipHighLight[i]);
		m_pChipHighLight[i]->setPosition(Vec2(m_pBTNChip[i]->getContentSize() / 2));
		m_pChipHighLight[i]->setVisible(false);
	}

	//	初始化扑克牌

	Vec2 vHandCardPos[E_BRNN_PokerPos_NULL] =
	{
		Vec2(sBGSize.width * 0.5, sBGSize.height * 0.84),		//	庄
		Vec2(sBGSize.width * 0.15, sBGSize.height * 0.45),		//	东
		Vec2(sBGSize.width * 0.3833, sBGSize.height * 0.45),	//	南
		Vec2(sBGSize.width * 0.6167, sBGSize.height * 0.45),	//	西
		Vec2(sBGSize.width * 0.85, sBGSize.height * 0.45)		//	北
	};

	for (int i = E_BRNN_PokerPos_B; i < E_BRNN_PokerPos_NULL; i++)
	{
		m_pHandCard[i] = BRNNHandsCard::create();
		addChild(m_pHandCard[i]);
		m_pHandCard[i]->setPosition(vHandCardPos[i]);
	}
}

bool BRNNGameLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	BRNNGameScene* pScene = BRNNGameScene::getBRNNGameScene();
	if (!pScene)
	{
		return false;
	}
	if (pScene->m_eGameStatue != E_BRNN_Bet)
	{
		return false;
	}

	for (int i = E_BRNNBetArea_E; i < E_BRNNBetArea_NULL; i++)
	{
		Vec2 vTPos = touch->getLocation();
		if (m_pBetArea[i]->isContainsPoint(vTPos))
		{
			return true;
		}	
	}
	return false;
}

void BRNNGameLayer::onTouchMoved(Touch *touch, Event *unused_event)
{

}

void BRNNGameLayer::onTouchCancelled(Touch *touch, Event *unused_event)
{

}

void BRNNGameLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
	for (int i = E_BRNNBetArea_E; i < E_BRNNBetArea_NULL; i++)
	{
		Vec2 vTPos = touch->getLocation();
		if (m_pBetArea[i]->isContainsPoint(vTPos))
		{
			if (m_eCurrChipType >= E_BRNNChip_1 && m_eCurrChipType <= E_BRNNChip_1k)
			{
				BRNN_CMD_C_PlaceJetton pData;
				pData.cbJettonArea = BRNNTool::convertToServerArea((E_BRNNBetAreaType)i);
				pData.lJettonScore = BRNNTool::convertToChipValue(m_eCurrChipType);
				EntityMgr::instance()->getDispatch()->sendSocketDate(BRNN_SUB_C_PLACE_JETTON, &pData, sizeof(pData));

				log("pData->cbJettonArea = %d, pData->lJettonScore = %d", (int)pData.cbJettonArea, (int)pData.lJettonScore);
				break;
			}
		}	
	}
}

//	点击筹码事件
void BRNNGameLayer::onClickBTNChipEvent(Ref* pSender)
{
	Button* pChip = dynamic_cast<Button*>(pSender);
	for (int i = E_BRNNChip_1; i < E_BRNNChip_NULL; i++)
	{
		if (pChip && pChip == m_pBTNChip[i])
		{
			m_pChipHighLight[i]->setVisible(true);
			m_eCurrChipType = (E_BRNNChipType)i;
			continue;
		}
		m_pChipHighLight[i]->setVisible(false);
	}
}

//	点击退出游戏事件
void BRNNGameLayer::onClickBTNQuitEvent()
{
	BRNNGameScene* pScene = BRNNGameScene::getBRNNGameScene();
	if (!pScene)
	{
		return;
	}
	BRNNDialogQuit* pQuit = BRNNDialogQuit::create();
	if (pQuit)
	{
		pScene->addChild(pQuit);
	}
}

//	点击设置按钮
void BRNNGameLayer::onClickBTNSettingEvent()
{
	BRNNGameScene* pScene = BRNNGameScene::getBRNNGameScene();
	if (!pScene)
	{
		return;
	}
	pScene->m_pPopupSetting->setPopupSettingVisible(true);
}

//	点击路单按钮
void BRNNGameLayer::onClickBTNTravelEvent()
{
	BRNNGameScene* pScene = BRNNGameScene::getBRNNGameScene();
	if (!pScene)
	{
		return;
	}
	pScene->m_pPopupTravel->setPopupVisible(true);
}

//	点击上庄列表事件
void BRNNGameLayer::onClickBTNApplyListEvent()
{
	EntityMgr::instance()->getDispatch()->sendSocketDate(BRNN_SUB_C_GET_PLAYLIST);
}

//	点击申请上庄事件
void BRNNGameLayer::onClickBTNApplyEvent()
{
	DWORD dSelfChairID = EntityMgr::instance()->roomFrame()->getMeUserDate()->wChairID;
	BRNN_CMD_S_ApplyBanker pData;
	pData.wApplyUser = dSelfChairID;
	EntityMgr::instance()->getDispatch()->sendSocketDate(BRNN_SUB_C_APPLY_BANKER, &pData, sizeof(pData));
}

//	点击取消申请事件
void BRNNGameLayer::onClickBTNCancelApplyEvent()
{
	tagUserData* pUserData = EntityMgr::instance()->roomFrame()->getMeUserDate();
	BRNN_CMD_S_CancelBanker pData;
	memset(pData.szCancelUser, 0, sizeof(pData.szCancelUser));
	memcpy(pData.szCancelUser, pUserData->szNickName, sizeof(pUserData->szNickName));
	EntityMgr::instance()->getDispatch()->sendSocketDate(BRNN_SUB_C_CANCEL_BANKER, &pData, sizeof(pData));
}

//	点击我要下庄事件
void BRNNGameLayer::onClickBTNDownEvent()
{
	tagUserData* pUserData = EntityMgr::instance()->roomFrame()->getMeUserDate();
	BRNN_CMD_S_CancelBanker pData;
	memset(pData.szCancelUser, 0, sizeof(pData.szCancelUser));
	memcpy(pData.szCancelUser, pUserData->szNickName, sizeof(pUserData->szNickName));
	EntityMgr::instance()->getDispatch()->sendSocketDate(BRNN_SUB_C_CANCEL_BANKER, &pData, sizeof(pData));
}


