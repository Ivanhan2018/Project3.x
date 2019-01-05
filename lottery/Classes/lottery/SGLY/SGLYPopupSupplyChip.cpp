#include "SGLYPopupSupplyChip.h"
#include "SGLYRes.h"
#include "SGLYDefine.h"
#include "SGLYGameScene.h"
#include "EntityMgr.h"
#include "ToolKit.h"
#include "SGLYProtocol.h"
#include "SGLYTool.h"

SGLYPopupSupplyChip::SGLYPopupSupplyChip()
{
}

SGLYPopupSupplyChip::~SGLYPopupSupplyChip()
{
}

//	设置是否显示弹窗
void SGLYPopupSupplyChip::setPopupVisible(bool bIsVisible)
{
	this->setVisible(bIsVisible);
	m_pListener->setSwallowTouches(bIsVisible);
	auto vList = m_pBG->getChildren();
	for (auto k : vList)
	{
		Button* pNode = dynamic_cast<Button*>(k);
		if (!pNode)
		{
			continue;
		}
		pNode->setEnabled(bIsVisible);
	}
}

bool SGLYPopupSupplyChip::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!BaseDialog::init());

		initUI();
		initData();
		scheduleUpdate();

		bRet = true;
	} while (0);

	return bRet;
}

void SGLYPopupSupplyChip::initUI()
{
	cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();

	m_pBG = Sprite::create(SGLYTextureName::sSGLY_PopupSupplyBG);
	addChild(m_pBG);
	m_pBG->setPosition(Vec2(sVSize / 2));
	cocos2d::Size sBGSize = m_pBG->getContentSize();

	vector<Node*> vList;

	m_pBtnChip100 = Button::create(
		SGLYTextureName::sSGLY_PopupSupplyBtnChip100Normal,
		SGLYTextureName::sSGLY_PopupSupplyBtnChip100Pressed,
		"",
		TextureResType::PLIST
		);
	m_pBG->addChild(m_pBtnChip100);
	m_pBtnChip100->setPosition(Vec2(sBGSize.width * 0.115, sBGSize.height * 0.67));
	m_pBtnChip100->addClickEventListener(CC_CALLBACK_1(SGLYPopupSupplyChip::onClickBtnChip100Event, this));

	m_pBtnChip1000 = Button::create(
		SGLYTextureName::sSGLY_PopupSupplyBtnChip1000Normal,
		SGLYTextureName::sSGLY_PopupSupplyBtnChip1000Pressed,
		"",
		TextureResType::PLIST
		);
	m_pBG->addChild(m_pBtnChip1000);
	m_pBtnChip1000->setPosition(Vec2(sBGSize.width * 0.305, sBGSize.height * 0.67));
	m_pBtnChip1000->addClickEventListener(CC_CALLBACK_1(SGLYPopupSupplyChip::onClickBtnChip1000Event, this));

	m_pBtnChip10000 = Button::create(
		SGLYTextureName::sSGLY_PopupSupplyBtnChip10000Normal,
		SGLYTextureName::sSGLY_PopupSupplyBtnChip10000Pressed,
		"",
		TextureResType::PLIST
		);
	m_pBG->addChild(m_pBtnChip10000);
	m_pBtnChip10000->setPosition(Vec2(sBGSize.width * 0.495, sBGSize.height * 0.67));
	m_pBtnChip10000->addClickEventListener(CC_CALLBACK_1(SGLYPopupSupplyChip::onClickBtnChip10000Event, this));

	m_pBtnChipMax = Button::create(
		SGLYTextureName::sSGLY_PopupSupplyBtnChipNormalMax,
		SGLYTextureName::sSGLY_PopupSupplyBtnChipPressedMax,
		"",
		TextureResType::PLIST
		);
	m_pBG->addChild(m_pBtnChipMax);
	m_pBtnChipMax->setPosition(Vec2(sBGSize.width * 0.685, sBGSize.height * 0.67));
	m_pBtnChipMax->addClickEventListener(CC_CALLBACK_1(SGLYPopupSupplyChip::onClickBtnChipMaxEvent, this));

	m_pBtnClear = Button::create(
		SGLYTextureName::sSGLY_PopupSupplyBtnClearNormal,
		SGLYTextureName::sSGLY_PopupSupplyBtnClearPressed,
		"",
		TextureResType::PLIST
		);
	m_pBG->addChild(m_pBtnClear);
	m_pBtnClear->setPosition(Vec2(sBGSize.width * 0.875, sBGSize.height * 0.67));
	m_pBtnClear->addClickEventListener(CC_CALLBACK_1(SGLYPopupSupplyChip::onClickBtnClearEvent, this));

	m_pBtnOK = Button::create(
		SGLYTextureName::sSGLY_PopupSupplyBtnOKNormal,
		SGLYTextureName::sSGLY_PopupSupplyBtnOKPressed,
		"",
		TextureResType::PLIST
		);
	m_pBG->addChild(m_pBtnOK);
	m_pBtnOK->setPosition(Vec2(sBGSize.width * 0.35, sBGSize.height * 0.16));
	m_pBtnOK->addClickEventListener(CC_CALLBACK_1(SGLYPopupSupplyChip::onClickBtnOKEvent, this));

	m_pBtnNO = Button::create(
		SGLYTextureName::sSGLY_PopupSupplyBtnNONormal,
		SGLYTextureName::sSGLY_PopupSupplyBtnNOPressed,
		"",
		TextureResType::PLIST
		);
	m_pBG->addChild(m_pBtnNO);
	m_pBtnNO->setPosition(Vec2(sBGSize.width * 0.65, sBGSize.height * 0.16));
	m_pBtnNO->addClickEventListener(CC_CALLBACK_1(SGLYPopupSupplyChip::onClickBtnNOEvent, this));

	m_pLabDeclaration = Label::create();
	m_pBG->addChild(m_pLabDeclaration);
	m_pLabDeclaration->setPosition(Vec2(sBGSize.width * 0.5, sBGSize.height * 0.88));
	m_pLabDeclaration->setSystemFontSize(23);
	m_pLabDeclaration->setColor(Color3B::WHITE);

	m_pLabExchangeRate = Label::create();
	m_pBG->addChild(m_pLabExchangeRate);
	m_pLabExchangeRate->setPosition(Vec2(sBGSize.width * 0.5, sBGSize.height * 0.52));
	m_pLabExchangeRate->setSystemFontSize(23);
	m_pLabExchangeRate->setColor(Color3B::WHITE);

	m_pLabTipsExchangeChip = Label::create();
	m_pBG->addChild(m_pLabTipsExchangeChip);
	m_pLabTipsExchangeChip->setPosition(Vec2(sBGSize.width * 0.5, sBGSize.height * 0.42));
	m_pLabTipsExchangeChip->setSystemFontSize(25);
	m_pLabTipsExchangeChip->setColor(Color3B::WHITE);

	m_pLabSurplusGold = Label::create();
	m_pBG->addChild(m_pLabSurplusGold);
	m_pLabSurplusGold->setPosition(Vec2(sBGSize.width * 0.5, sBGSize.height * 0.32));
	m_pLabSurplusGold->setSystemFontSize(25);
	m_pLabSurplusGold->setColor(Color3B::WHITE);
}

void SGLYPopupSupplyChip::initData()
{
	Dictionary * pDictionary = Dictionary::createWithContentsOfFile(SGLY_XML_FILE_NAME);
	m_sDeclaration = ((CCString*)pDictionary->objectForKey(SGLY_XML_KEY_SUPPLY_DECLARATION))->getCString();
	m_sExchangeRateFormat = ((CCString*)pDictionary->objectForKey(SGLY_XML_KEY_SUPPLY_EXCHANGE_RATE_FORMAT))->getCString();
	m_sTipsExchangeChipFormat = ((CCString*)pDictionary->objectForKey(SGLY_XML_KEY_SUPPLY_TIPS_EXCHANGE_CHIP_FORMAT))->getCString();
	m_sTipsSurplusGoldFormat = ((CCString*)pDictionary->objectForKey(SGLY_XML_KEY_SUPPLY_TIPS_SURPLUS_GOLD_FORMAT))->getCString();
	m_pLabDeclaration->setString(m_sDeclaration);
	m_llPlayerChipValue = 0;
}

void SGLYPopupSupplyChip::update(float dt)
{
	bool bIsGameStart = EntityMgr::instance()->roomFrame()->getGameIsStart();
	if (!bIsGameStart)
	{
		return;
	}
	SGLYGameScene* pScene = SGLYGameScene::getGameScene();
	if (!pScene)
	{
		return;
	}
	char cExchangeRate[128];
	sprintf(cExchangeRate, m_sExchangeRateFormat.c_str(), 1, SGLYTool::m_dwExchangeRate);
	m_pLabExchangeRate->setString(cExchangeRate);
	m_llPlayerGold = EntityMgr::instance()->roomFrame()->getMeUserDate()->lScore;
	showExchangeTips(0);

	unscheduleUpdate();
}

//	显示兑换提示
void SGLYPopupSupplyChip::showExchangeTips(const LONGLONG llGoldValue)
{
	LONGLONG llChipValue = SGLYTool::exchangeForChip(llGoldValue);
	LONGLONG llSurplusGold = m_llPlayerGold - llGoldValue;


	char cTipsExchangeChip[128];
	char cTipsSurplusGold[128];

	sprintf(cTipsExchangeChip, m_sTipsExchangeChipFormat.c_str(), llChipValue, llGoldValue);
	sprintf(cTipsSurplusGold, m_sTipsSurplusGoldFormat.c_str(), llSurplusGold);

	m_pLabTipsExchangeChip->setString(cTipsExchangeChip);
	m_pLabSurplusGold->setString(cTipsSurplusGold);
}

//	点击筹码100按钮事件
void SGLYPopupSupplyChip::onClickBtnChip100Event(Ref* pSender)
{
	int nChipValue = 100;
	LONGLONG llTmp = m_llPlayerChipValue + SGLYTool::exchangeForGold(nChipValue);
	bool bIsEnough = m_llPlayerGold - llTmp >= 0 ? true : false;
	if (bIsEnough == false)
	{
		return;
	}
	m_llPlayerChipValue = llTmp;
	showExchangeTips(m_llPlayerChipValue);
}

//	点击筹码1000按钮事件
void SGLYPopupSupplyChip::onClickBtnChip1000Event(Ref* pSender)
{
	int nChipValue = 1000;
	LONGLONG llTmp = m_llPlayerChipValue + SGLYTool::exchangeForGold(nChipValue);
	bool bIsEnough = m_llPlayerGold - llTmp >= 0 ? true : false;
	if (bIsEnough == false)
	{
		return;
	}
	m_llPlayerChipValue = llTmp;
	showExchangeTips(m_llPlayerChipValue);
}

//	点击筹码10000按钮事件
void SGLYPopupSupplyChip::onClickBtnChip10000Event(Ref* pSender)
{
	int nChipValue = 10000;
	LONGLONG llTmp = m_llPlayerChipValue + SGLYTool::exchangeForGold(nChipValue);
	bool bIsEnough = m_llPlayerGold - llTmp >= 0 ? true : false;
	if (bIsEnough == false)
	{
		return;
	}
	m_llPlayerChipValue = llTmp;
	showExchangeTips(m_llPlayerChipValue);
}

//	点击筹码最大按钮事件
void SGLYPopupSupplyChip::onClickBtnChipMaxEvent(Ref* pSender)
{
	LONGLONG llMaxChipValue = SGLYTool::exchangeForChip(m_llPlayerGold);
	m_llPlayerChipValue = SGLYTool::exchangeForGold(llMaxChipValue);
	showExchangeTips(m_llPlayerChipValue);
}

//	点击清空按钮事件
void SGLYPopupSupplyChip::onClickBtnClearEvent(Ref* pSender)
{
	m_llPlayerChipValue = 0;
	showExchangeTips(m_llPlayerChipValue);
}

//	点击确定按钮事件
void SGLYPopupSupplyChip::onClickBtnOKEvent(Ref* pSender)
{
	if (m_llPlayerChipValue <= 0)
	{
		return;
	}
	m_llPlayerGold -= m_llPlayerChipValue;
	SGLY_CMD_C_Exchange pMessage;
	pMessage.lExchangeScore = m_llPlayerChipValue;
	EntityMgr::instance()->getDispatch()->sendSocketDate(SGLY_SUB_C_EXCHANGE, &pMessage, sizeof(pMessage));
	onClickBtnNOEvent(nullptr);
}

//	点击取消按钮事件
void SGLYPopupSupplyChip::onClickBtnNOEvent(Ref* pSender)
{
	onClickBtnClearEvent(nullptr);
	setPopupVisible(false);
}

