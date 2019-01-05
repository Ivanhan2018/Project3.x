#include "PPCControl.h"
#include "PPCProtocol.h"
#include "EntityMgr.h"
#include "PPCTool.h"
#include "ToolKit.h"
#include "PPCGameScene.h"

PPCControl::PPCControl()
{
}

PPCControl::~PPCControl()
{
}

void PPCControl::betAll(EPPCCarLogoType eAreaType, SCORE sValue)
{
	LONGLONG allScore = sValue;
	if (allScore <= 0)
	{
		return;
	}
	for (int j = EPPCChip_1k; j >= EPPCChip_1; j--)
	{
		if (allScore == 0)
		{
			break;
		}
		if ( j == EPPCChip_1)
		{
			for (int i = 0; i < allScore; i++)
			{
				bet(eAreaType, 1);
			}
			break;
		}
		int tempBet = PPCTool::convertToChipValue((EPPCChipType)j);
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
	PPCGameScene* pGameScene = dynamic_cast<PPCGameScene*>(Director::getInstance()->getRunningScene());
	if (!pGameScene)
	{
		return;
	}
	pGameScene->m_pPPCGameLayer->resetCarLogo();
}

void PPCControl::setBtnChipEnable(EPPCChipType eType, bool bEnabled)
{
	m_pBtnChip[eType]->setEnabled(bEnabled);
}

void PPCControl::setAllBtnChipEnable(bool bEnabled)
{
	for(int i = EPPCChip_1; i < EPPCChip_Null; i++)
	{
		m_pBtnChip[i]->setEnabled(bEnabled);
	}
}

void PPCControl::checkoutEnabledBtnChip(SCORE sValue)
{
	if (sValue >= 1000)
	{
		setAllBtnChipEnable(true);
	}
	else if (sValue >= 500 && sValue < 1000)
	{
		setBtnChipEnable(EPPCChip_1, true);
		setBtnChipEnable(EPPCChip_5, true);
		setBtnChipEnable(EPPCChip_10, true);
		setBtnChipEnable(EPPCChip_100, true);
		setBtnChipEnable(EPPCChip_500, true);
		setBtnChipEnable(EPPCChip_1k, false);
	}
	else if (sValue >= 100 && sValue < 500)
	{
		setBtnChipEnable(EPPCChip_1, true);
		setBtnChipEnable(EPPCChip_5, true);
		setBtnChipEnable(EPPCChip_10, true);
		setBtnChipEnable(EPPCChip_100, true);
		setBtnChipEnable(EPPCChip_500, false);
		setBtnChipEnable(EPPCChip_1k, false);
	}
	else if (sValue >= 10 && sValue < 100)
	{
		setBtnChipEnable(EPPCChip_1, true);
		setBtnChipEnable(EPPCChip_5, true);
		setBtnChipEnable(EPPCChip_10, true);
		setBtnChipEnable(EPPCChip_100, false);
		setBtnChipEnable(EPPCChip_500, false);
		setBtnChipEnable(EPPCChip_1k, false);
	}
	else if (sValue >= 1 && sValue < 10)
	{
		setBtnChipEnable(EPPCChip_1, true);
		setBtnChipEnable(EPPCChip_5, false);
		setBtnChipEnable(EPPCChip_10, false);
		setBtnChipEnable(EPPCChip_100, false);
		setBtnChipEnable(EPPCChip_500, false);
		setBtnChipEnable(EPPCChip_1k, false);
	}
	else
	{
		setAllBtnChipEnable(false);
	}
}

bool PPCControl::isWinTheLotterys(EPPCCarLogoType eLotteryType)
{
	for (int i = 0; i < EPPCCarLogo_Null; i++)
	{
		LONGLONG llValue = m_pTableArea[i]->getChipValueSelf();
		if (llValue > 0 && i == eLotteryType)
		{
			return true;
		}
	}

	return false;
}

void PPCControl::resetPPCControl()
{
	m_eCurrChipType = EPPCChip_Null;

	for (int i = EPPCChip_1; i < EPPCChip_Null; i++)
	{
		m_pChipHighLight[i]->setVisible(false);
	}

	for (int i = EPPCCarLogo_Porsche_Big; i < EPPCCarLogo_Null; i++)
	{
		m_pTableArea[i]->resetTableArea();
	}
}

void PPCControl::bet(EPPCCarLogoType eAreaType, SCORE sValue)
{
	EPPCChipType eChipType = PPCTool::convertToChipType(sValue);
	m_pTableArea[eAreaType]->addTableChip(eChipType);
	LONGLONG llSum = m_pTableArea[eAreaType]->getChipValueSum() + (LONGLONG)sValue;
	m_pTableArea[eAreaType]->setChipValueSum(llSum);
	m_pTableArea[eAreaType]->setLabChipValueSum(llSum);
	showAniBet(eAreaType);
}

void PPCControl::showAniBet(EPPCCarLogoType eType)
{
	int nRandomArray[4];
	int nStart;
	for (int i = 0; i < 4; i++)
	{
		switch (eType)
		{
		case EPPCCarLogo_Porsche_Big:
			nStart = 0;
			break;
		case EPPCCarLogo_Porsche_Small:
			nStart = 1;
			break;
		case EPPCCarLogo_BMW_Big:
			nStart = 2;
			break;
		case EPPCCarLogo_BMW_Small:
			nStart = 3;
			break;
		case EPPCCarLogo_Benz_Big:
			nStart = 4;
			break;
		case EPPCCarLogo_Benz_Small:
			nStart = 5;
			break;
		case EPPCCarLogo_VW_Big:
			nStart = 6;
			break;
		case EPPCCarLogo_VW_Small:
			nStart = 7;
			break;
		default:
			break;
		}
		nRandomArray[i] = nStart + i * 8;
	}
	int idx = random(0, 3);
	PPCGameScene* pGameScene = dynamic_cast<PPCGameScene*>(Director::getInstance()->getRunningScene());
	if (!pGameScene)
	{
		return;
	}
	pGameScene->m_pPPCGameLayer->showSingleCarLogoHighLight(nRandomArray[idx]);
}

void PPCControl::showAreaHighLight(EPPCCarLogoType eCarLogoType)
{
	m_pTableArea[eCarLogoType]->setAreaHighLight(true);
}

PPCTableArea* PPCControl::getTableArea(EPPCCarLogoType eType)
{
	return m_pTableArea[eType];
}

bool PPCControl::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Layer::init());

		initUI();
		initData();

		bRet = true;
	} while (0);

	return bRet;
}

void PPCControl::initData()
{
	m_eCurrChipType = EPPCChip_Null;
	//	Ìí¼Ó´¥ÃþÊÂ¼þ
	auto pListener = EventListenerTouchOneByOne::create();
	pListener->onTouchBegan = CC_CALLBACK_2(PPCControl::onTouchBegan, this);
	pListener->onTouchMoved = CC_CALLBACK_2(PPCControl::onTouchMoved, this);
	pListener->onTouchCancelled = CC_CALLBACK_2(PPCControl::onTouchCancelled, this);
	pListener->onTouchEnded = CC_CALLBACK_2(PPCControl::onTouchEnded, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pListener, this);
}

void PPCControl::initUI()
{
	cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();

	//	ÏÂ×¢ÇøÓò¿é

	Vec2 vAreaPosList[EPPCCarLogo_Null] = 
	{
		Vec2(sVSize.width * 0.216, sVSize.height * 0.703),
		Vec2(sVSize.width * 0.216, sVSize.height * 0.530),
		Vec2(sVSize.width * 0.403, sVSize.height * 0.703),
		Vec2(sVSize.width * 0.404, sVSize.height * 0.530),
		Vec2(sVSize.width * 0.591, sVSize.height * 0.704),
		Vec2(sVSize.width * 0.591, sVSize.height * 0.530),
		Vec2(sVSize.width * 0.779, sVSize.height * 0.704),
		Vec2(sVSize.width * 0.779, sVSize.height * 0.530),
	};

	vector<Node*> vAreaList;
	for (int i = EPPCCarLogo_Porsche_Big; i < EPPCCarLogo_Null; i++)
	{
		m_pTableArea[i] = PPCTableArea::create();
		addChild(m_pTableArea[i]);
		m_pTableArea[i]->setPosition(vAreaPosList[i]);
		m_pTableArea[i]->initArea((EPPCCarLogoType)i);
		vAreaList.push_back(m_pTableArea[i]);
	}
// 	ToolKit::studioUIMulti(this, vAreaList);


	//	³ïÂë

	string sBufChipNormal[EPPCChip_Null] = 
	{
		PPCTextureName::s_ppc_chip_normal_1,
		PPCTextureName::s_ppc_chip_normal_5,
		PPCTextureName::s_ppc_chip_normal_10,
		PPCTextureName::s_ppc_chip_normal_100,
		PPCTextureName::s_ppc_chip_normal_500,
		PPCTextureName::s_ppc_chip_normal_1k,
	};

	string sBufChipDisable[EPPCChip_Null] = 
	{
		PPCTextureName::s_ppc_chip_disable_1,
		PPCTextureName::s_ppc_chip_disable_5,
		PPCTextureName::s_ppc_chip_disable_10,
		PPCTextureName::s_ppc_chip_disable_100,
		PPCTextureName::s_ppc_chip_disable_500,
		PPCTextureName::s_ppc_chip_disable_1k,
	};

	Vec2 vPosList[EPPCChip_Null] = 
	{
		Vec2(sVSize.width * 0.273, sVSize.height * 0.195),
		Vec2(sVSize.width * 0.3632, sVSize.height * 0.195),
		Vec2(sVSize.width * 0.4544, sVSize.height * 0.195),
		Vec2(sVSize.width * 0.5456, sVSize.height * 0.195),
		Vec2(sVSize.width * 0.6368, sVSize.height * 0.195),
		Vec2(sVSize.width * 0.728, sVSize.height * 0.195),
	};
	vector<Node*> vChipList;
	for (int i = EPPCChip_1; i < EPPCChip_Null; i++)
	{
		m_pBtnChip[i] = Button::create(sBufChipNormal[i], sBufChipNormal[i], sBufChipDisable[i], TextureResType::PLIST);
		addChild(m_pBtnChip[i]);
		m_pBtnChip[i]->setPosition(vPosList[i]);
		m_pBtnChip[i]->addClickEventListener(CC_CALLBACK_1(PPCControl::onClickChip, this));

		m_pChipHighLight[i] = Sprite::createWithSpriteFrameName(PPCTextureName::s_ppc_chip_high_light);
		m_pBtnChip[i]->addChild(m_pChipHighLight[i]);
		m_pChipHighLight[i]->setPosition(Vec2(m_pBtnChip[i]->getContentSize() / 2));
		m_pChipHighLight[i]->setVisible(false);

// 		m_pBtnChip[i]->setEnabled(false);
		vChipList.push_back(m_pBtnChip[i]);
	}
// 	ToolKit::studioUIMulti(this, vChipList);

}

void PPCControl::onClickChip(Ref* pSender)
{
	Button* pChip = dynamic_cast<Button*>(pSender);
	for (int i = EPPCChip_1; i < EPPCChip_Null; i++)
	{
		if (pChip && pChip == m_pBtnChip[i])
		{
			m_pChipHighLight[i]->setVisible(true);
			m_eCurrChipType = (EPPCChipType)i;
			continue;
		}
		m_pChipHighLight[i]->setVisible(false);
	}
}


bool PPCControl::onTouchBegan(Touch *touch, Event *unused_event)
{
	for (int i = EPPCCarLogo_Porsche_Big; i < EPPCCarLogo_Null; i++)
	{
		Vec2 vTPos = touch->getLocation();
		if (m_pTableArea[i]->isContainsPoint(vTPos))
		{
			return true;
		}	
	}
	return false;
}

void PPCControl::onTouchMoved(Touch *touch, Event *unused_event)
{

}

void PPCControl::onTouchCancelled(Touch *touch, Event *unused_event)
{

}

void PPCControl::onTouchEnded(Touch *touch, Event *unused_event)
{
	for (int i = EPPCCarLogo_Porsche_Big; i < EPPCCarLogo_Null; i++)
	{
		Vec2 vTPos = touch->getLocation();
		if (m_pTableArea[i]->isContainsPoint(vTPos))
		{
			if (m_eCurrChipType >= EPPCChip_1 && m_eCurrChipType <= EPPCChip_1k)
			{
				PPCGameScene* pGameScene = dynamic_cast<PPCGameScene*>(Director::getInstance()->getRunningScene());
				if (!pGameScene)
				{
					return;
				}
				if (pGameScene->m_GameStatue != EPPC_Bet)
				{
					return;
				}
				ppcCMD_C_PlaceJetton pData;
				pData.cbJettonArea = PPCTool::convertToServerArea((EPPCCarLogoType)i);
				pData.lJettonScore = PPCTool::convertToChipValue(m_eCurrChipType);
				EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_PLACE_JETTON, &pData, sizeof(pData));

				log("pData->cbJettonArea = %d, pData->lJettonScore = %d", (int)pData.cbJettonArea, (int)pData.lJettonScore);
				break;
			}
		}	
	}
}

