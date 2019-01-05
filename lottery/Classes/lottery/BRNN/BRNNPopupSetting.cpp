#include "BRNNPopupSetting.h"
#include "BRNNGameScene.h"
#include "BRNNRes.h"
#include "BRNNManageAudio.h"

USING_NS_CC_EXT;

BRNNPopupSetting::BRNNPopupSetting()
{
}

BRNNPopupSetting::~BRNNPopupSetting()
{
}

void BRNNPopupSetting::setPopupSettingVisible(bool bIsVisible)
{
	if (bIsVisible)
	{
		this->setVisible(true);
		m_pControlMusic->setEnabled(true);
		m_pControlSoundEffect->setEnabled(true);
	}
	else
	{
		this->setVisible(false);
		m_pControlMusic->setEnabled(false);
		m_pControlSoundEffect->setEnabled(false);

		bool bIsOnBGM = BRNNManageAudio::getInstance()->getOnBGMusic();
		bool bIsOnSoundEffect = BRNNManageAudio::getInstance()->getOnSoundEffect();

		UserDefault::getInstance()->setBoolForKey(BRNN_DATA_KEY_AUDIO_ON_BGM, bIsOnBGM);
		UserDefault::getInstance()->setBoolForKey(BRNN_DATA_KEY_AUDIO_ON_SOUND_EFFECT, bIsOnSoundEffect);
	}
}

bool BRNNPopupSetting::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Layer::init());

		initUI();

		//	添加触摸事件
		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = CC_CALLBACK_2(BRNNPopupSetting::onTouchBegan, this);
		listener->onTouchMoved = CC_CALLBACK_2(BRNNPopupSetting::onTouchMoved, this);
		listener->onTouchCancelled = CC_CALLBACK_2(BRNNPopupSetting::onTouchCancelled, this);
		listener->onTouchEnded = CC_CALLBACK_2(BRNNPopupSetting::onTouchEnded, this);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

		bRet = true;
	} while (0);

	return bRet;
}

void BRNNPopupSetting::initUI()
{
	cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();

	m_pBG = Sprite::create(BRNNTextureName::s_brnn_setting_bg);
	addChild(m_pBG);
	m_pBG->setPosition(Vec2(sVSize / 2));
	cocos2d::Size sBGSize = m_pBG->getContentSize();

	Sprite* pMusicBG = Sprite::create("brnn_setting_switch_bg.png");
	m_pBG->addChild(pMusicBG);
	pMusicBG->setPosition(Vec2(sBGSize.width * 0.65, sBGSize.height * 0.55));

	Sprite* pSoundEffectBG = Sprite::create("brnn_setting_switch_bg.png");
	m_pBG->addChild(pSoundEffectBG);
	pSoundEffectBG->setPosition(Vec2(sBGSize.width * 0.65, sBGSize.height * 0.3));

	bool bOnBGM = BRNNManageAudio::getInstance()->getOnBGMusic();
	bool bOnSoundEffect = BRNNManageAudio::getInstance()->getOnSoundEffect();

	m_pControlMusic = extension::ControlSwitch::create(
		Sprite::create(BRNNTextureName::s_brnn_setting_switch_mask),
		Sprite::create(BRNNTextureName::s_brnn_setting_switch_on),
		Sprite::create(BRNNTextureName::s_brnn_setting_switch_off),
		Sprite::create(BRNNTextureName::s_brnn_setting_switch_thumb),
		Label::createWithSystemFont("On", "Arial-BoldMT", 16),
		Label::createWithSystemFont("Off", "Arial-BoldMT", 16)
		);
	pMusicBG->addChild(m_pControlMusic);
	m_pControlMusic->setPosition(Vec2(pMusicBG->getContentSize() / 2));
	m_pControlMusic->setOn(bOnBGM);
	m_pControlMusic->addTargetWithActionForControlEvents(this, cccontrol_selector(BRNNPopupSetting::onControlMusicChange), extension::Control::EventType::VALUE_CHANGED);

	m_pControlSoundEffect = extension::ControlSwitch::create(
		Sprite::create(BRNNTextureName::s_brnn_setting_switch_mask),
		Sprite::create(BRNNTextureName::s_brnn_setting_switch_on),
		Sprite::create(BRNNTextureName::s_brnn_setting_switch_off),
		Sprite::create(BRNNTextureName::s_brnn_setting_switch_thumb),
		Label::createWithSystemFont("On", "Arial-BoldMT", 16),
		Label::createWithSystemFont("Off", "Arial-BoldMT", 16)
		);
	pSoundEffectBG->addChild(m_pControlSoundEffect);
	m_pControlSoundEffect->setPosition(Vec2(pSoundEffectBG->getContentSize() / 2));
	m_pControlSoundEffect->setOn(bOnSoundEffect);
	m_pControlSoundEffect->addTargetWithActionForControlEvents(this, cccontrol_selector(BRNNPopupSetting::onControlSoundEffectChange), extension::Control::EventType::VALUE_CHANGED);
}

//	音乐开关按钮事件
void BRNNPopupSetting::onControlMusicChange(Ref* sender, extension::Control::EventType controlEvent)
{
	BRNNGameScene* pScene = BRNNGameScene::getBRNNGameScene();
	if (!pScene)
	{
		return;
	}
	auto pManageAudio = BRNNManageAudio::getInstance();
	if (m_pControlMusic->isOn())
	{
		pManageAudio->setOnBGMusic(true);
		if (pScene->m_eGameStatue == E_BRNN_Bet)
		{
			pManageAudio->playBGMusic();
		}
	}
	else
	{
		pManageAudio->setOnBGMusic(false);
		pManageAudio->stopBGMusic();
	}
}

//	音效开关按钮事件
void BRNNPopupSetting::onControlSoundEffectChange(Ref* sender, extension::Control::EventType controlEvent)
{
	auto pManageAudio = BRNNManageAudio::getInstance();
	if (m_pControlSoundEffect->isOn())
	{
		pManageAudio->setOnSoundEffect(true);
	}
	else
	{
		pManageAudio->setOnSoundEffect(false);
	}
}

bool BRNNPopupSetting::onTouchBegan(Touch *touch, Event *unused_event)
{
	if (this->isVisible())
	{
		return true;
	}

	return false;
}

void BRNNPopupSetting::onTouchMoved(Touch *touch, Event *unused_event)
{

}

void BRNNPopupSetting::onTouchCancelled(Touch *touch, Event *unused_event)
{

}

void BRNNPopupSetting::onTouchEnded(Touch *touch, Event *unused_event)
{
	log("BRNNPopupSetting::onTouchEnded");

	Vec2 vTouchPos = touch->getLocation();
	if (m_pBG->getBoundingBox().containsPoint(vTouchPos))
	{
		return;
	}
	setPopupSettingVisible(false);
}