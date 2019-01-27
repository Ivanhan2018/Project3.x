#include "DZPKDialog.h"
#include "SoundControl.h"
#include "BJLRes.h"
#include "CMD_DZPK.h"
#include "DZPKLayer.h"

using namespace ui;
/**************************DialogBase	Begin**********************************************/

DialogBase::DialogBase()
{
}

DialogBase::~DialogBase()
{
}

bool DialogBase::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Layer::init());

		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = CC_CALLBACK_2(DialogBase::onTouchBegan, this);
		listener->onTouchMoved = CC_CALLBACK_2(DialogBase::onTouchMoved, this);
		listener->onTouchCancelled = CC_CALLBACK_2(DialogBase::onTouchCancelled, this);
		listener->onTouchEnded = CC_CALLBACK_2(DialogBase::onTouchEnded, this);
		listener->setSwallowTouches(true);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
		
		setZOrder(EDZPKLayerZOrder_Dialog);
		m_sVSize = Director::getInstance()->getVisibleSize();

		bRet = true;
	} while (0);

	return bRet;
}

bool DialogBase::onTouchBegan(Touch *touch, Event *unused_event)
{
	log("DialogBase::onTouchBegan");
	return true;
}

void DialogBase::onTouchMoved(Touch *touch, Event *unused_event)
{
	log("DialogBase::onTouchMoved");
}

void DialogBase::onTouchCancelled(Touch *touch, Event *unused_event)
{
	log("DialogBase::onTouchCancelled");
}

void DialogBase::onTouchEnded(Touch *touch, Event *unused_event)
{
	log("DialogBase::onTouchEnded");
}

void DialogBase::closed()
{
	removeFromParent();
}

/**************************DialogBase	End**********************************************/


/**************************DialogEnding	Begin**********************************************/

DialogEnding::DialogEnding()
{
}

DialogEnding::~DialogEnding()
{
}

DialogEnding* DialogEnding::create(EndingType eType)
{
	DialogEnding* pLayer = new DialogEnding();
	if (pLayer && pLayer->init(eType))
	{
		pLayer->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pLayer);
	}
	
	return pLayer;
}


bool DialogEnding::init(EndingType eType)
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!DialogBase::init());
		initUI(eType);

		bRet = true;
	} while (0);

	return bRet;
}

void DialogEnding::initUI(EndingType eType)
{
	Sprite* pBG = nullptr;
	switch (eType)
	{
	case DialogEnding::EndingType_Win:
		{
			SoundControl::sharedSoundControl()->playSoundEffect(sound_name::s_dzpk_win);
			pBG = Sprite::createWithSpriteFrameName(texture_name::s_win);
		}
		break;
	case DialogEnding::EndingType_Lose:
		{
			SoundControl::sharedSoundControl()->playSoundEffect(sound_name::s_dzpk_lose);
			pBG = Sprite::createWithSpriteFrameName(texture_name::s_lose);
		}
		break;
	case DialogEnding::EndingType_Draw:
		{
			SoundControl::sharedSoundControl()->playSoundEffect(sound_name::s_dzpk_lose);
			pBG = Sprite::createWithSpriteFrameName(texture_name::s_draw);
		}
		break;
	default:
		break;
	}
	if (!pBG)
	{
		return;
	}
	pBG->setPosition(Vec2(m_sVSize / 2));
	addChild(pBG);

	auto pDelayTime = DelayTime::create(3.0f);
	auto pClosed = CallFunc::create(CC_CALLBACK_0(DialogEnding::closed, this));
	pBG->runAction(Sequence::create(pDelayTime, pClosed, nullptr));
}

/**************************DialogEnding	End**********************************************/


/**************************DialogQuit	Begin**********************************************/

DialogQuit::DialogQuit()
{
}

DialogQuit::~DialogQuit()
{
}

bool DialogQuit::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!DialogBase::init());
		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void DialogQuit::initUI()
{

}
/**************************DialogQuit	End**********************************************/


/**************************DialogTimeOutQuit	Begin**********************************************/

DialogTimeOutQuit::DialogTimeOutQuit()
{
}

DialogTimeOutQuit::~DialogTimeOutQuit()
{
}

bool DialogTimeOutQuit::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!DialogBase::init());

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void DialogTimeOutQuit::initUI()
{
	m_pBG = Sprite::createWithSpriteFrameName(texture_name::s_ol_bg_bccm);
	addChild(m_pBG);
#if 0
	Vec2 offset=Vec2(-400,300);//µ˜ ‘”√<by hxh>
	m_pBG->setPosition(Vec2(m_sVSize / 2)+offset);
#else
	m_pBG->setPosition(Vec2(m_sVSize / 2));
#endif
	cocos2d::Size sBGSize = m_pBG->getContentSize();

	m_pBtnOK = Button::create(texture_name::s_strGame_button1, texture_name::s_strGame_button2, "", TextureResType::PLIST);
	m_pBG->addChild(m_pBtnOK);
	m_pBtnOK->setPosition(Vec2(sBGSize.width / 2, sBGSize.height * 0.2));
	m_pBtnOK->addClickEventListener(CC_CALLBACK_1(DialogTimeOutQuit::onClickBtnOK, this));
}

void DialogTimeOutQuit::onClickBtnOK(Ref* sender)
{
	closed();
	DZPKLayer::ExitDZPK();
}
/**************************DialogTimeOutQuit	End**********************************************/
