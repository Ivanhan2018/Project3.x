#include "SGLYPopupSetting.h"
#include "SGLYRes.h"
#include "ToolKit.h"
#include "SGLYManageAudio.h"

SGLYPopupSetting::SGLYPopupSetting()
{
}

SGLYPopupSetting::~SGLYPopupSetting()
{
}


bool SGLYPopupSetting::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!BaseDialog::init());

		m_bOnMusic = SGLYManageAudio::getInstance()->getOnBGMusic();
		m_bOnSoundEffect = SGLYManageAudio::getInstance()->getOnSoundEffect();

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void SGLYPopupSetting::initUI()
{
	cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();

	m_pBG = Sprite::create(SGLYTextureName::sSGLY_PopupSettingBG);
	addChild(m_pBG);
	m_pBG->setPosition(Vec2(sVSize / 2));
	cocos2d::Size sBGSize = m_pBG->getContentSize();

	m_pTextSetting = Sprite::createWithSpriteFrameName(SGLYTextureName::sSGLY_PopupSettingTextSetting);
	m_pBG->addChild(m_pTextSetting);
	m_pTextSetting->setPosition(Vec2(sBGSize.width * 0.5, sBGSize.height * 0.85));

	m_pTextMusic = Sprite::createWithSpriteFrameName(SGLYTextureName::sSGLY_PopupSettingTextMusic);
	m_pBG->addChild(m_pTextMusic);
	m_pTextMusic->setPosition(Vec2(sBGSize.width * 0.33, sBGSize.height * 0.55));

	m_pTextSoundEffect = Sprite::createWithSpriteFrameName(SGLYTextureName::sSGLY_PopupSettingTextSoundEffect);
	m_pBG->addChild(m_pTextSoundEffect);
	m_pTextSoundEffect->setPosition(Vec2(sBGSize.width * 0.83, sBGSize.height * 0.55));

	m_pCheckBoxMusic = CheckBox::create(
		SGLYTextureName::sSGLY_PopupSettingCheckBoxNormal,
		SGLYTextureName::sSGLY_PopupSettingCheckBoxNormal,
		SGLYTextureName::sSGLY_PopupSettingCheckBoxActive,
		SGLYTextureName::sSGLY_PopupSettingCheckBoxNormal,
		SGLYTextureName::sSGLY_PopupSettingCheckBoxNormal,
		TextureResType::PLIST);
	m_pBG->addChild(m_pCheckBoxMusic);
	m_pCheckBoxMusic->setPosition(Vec2(sBGSize.width * 0.18, sBGSize.height * 0.55));
	m_pCheckBoxMusic->addEventListener(CC_CALLBACK_2(SGLYPopupSetting::onCBMusicEvent, this));
	m_pCheckBoxMusic->setSelectedState(m_bOnMusic);

	m_pCheckBoxSoundEffect = CheckBox::create(
		SGLYTextureName::sSGLY_PopupSettingCheckBoxNormal,
		SGLYTextureName::sSGLY_PopupSettingCheckBoxNormal,
		SGLYTextureName::sSGLY_PopupSettingCheckBoxActive,
		SGLYTextureName::sSGLY_PopupSettingCheckBoxNormal,
		SGLYTextureName::sSGLY_PopupSettingCheckBoxNormal,
		TextureResType::PLIST);
	m_pBG->addChild(m_pCheckBoxSoundEffect);
	m_pCheckBoxSoundEffect->setPosition(Vec2(sBGSize.width * 0.68, sBGSize.height * 0.55));
	m_pCheckBoxSoundEffect->addEventListener(CC_CALLBACK_2(SGLYPopupSetting::onCBSoundEffectEvent, this));
	m_pCheckBoxSoundEffect->setSelectedState(m_bOnSoundEffect);

	m_pBtnOK = Button::create(
		SGLYTextureName::sSGLY_PPopupSettingBtnOKNormal,
		SGLYTextureName::sSGLY_PPopupSettingBtnOKPressed,
		"",
		TextureResType::PLIST);
	m_pBG->addChild(m_pBtnOK);
	m_pBtnOK->setPosition(Vec2(sBGSize.width * 0.3, sBGSize.height * 0.2));
	m_pBtnOK->addClickEventListener(CC_CALLBACK_0(SGLYPopupSetting::onClickBtnOKEvent, this));

	m_pBtnNO = Button::create(
		SGLYTextureName::sSGLY_PPopupSettingBtnNONormal,
		SGLYTextureName::sSGLY_PPopupSettingBtnNOPressed,
		"",
		TextureResType::PLIST);
	m_pBG->addChild(m_pBtnNO);
	m_pBtnNO->setPosition(Vec2(sBGSize.width * 0.7, sBGSize.height * 0.2));
	m_pBtnNO->addClickEventListener(CC_CALLBACK_0(SGLYPopupSetting::onClickBtnNOEvent, this));

#ifdef VER_9YI
	m_pTextSetting->setPosition(Vec2(sBGSize.width * 0.5, sBGSize.height * 0.85));
#endif // VER_9YI

#ifdef VER_QIANBO
	m_pTextSetting->setPosition(Vec2(sBGSize.width * 0.5, sBGSize.height * 0.9));
#endif // VER_QIANBO

#ifdef VER_369
	m_pTextSetting->setPosition(Vec2(sBGSize.width * 0.5, sBGSize.height * 0.9));
#endif // VER_QIANBO

}

//	点击音乐复选框事件
void SGLYPopupSetting::onCBMusicEvent(Ref* pSender,CheckBox::EventType type)
{
	switch (type)
	{
	case CheckBox::EventType::SELECTED:
		m_bOnMusic = true;
		break;
	case CheckBox::EventType::UNSELECTED:
		m_bOnMusic = false;
		break;
	default:
		break;
	}
}

//	点击音效复选框事件
void SGLYPopupSetting::onCBSoundEffectEvent(Ref* pSender,CheckBox::EventType type)
{
	switch (type)
	{
	case CheckBox::EventType::SELECTED:
		m_bOnSoundEffect = true;
		break;
	case CheckBox::EventType::UNSELECTED:
		m_bOnSoundEffect = false;
		break;
	default:
		break;
	}
}

//	点击确定按钮事件
void SGLYPopupSetting::onClickBtnOKEvent()
{
	SGLYManageAudio::getInstance()->setOnBGMusic(m_bOnMusic);
	SGLYManageAudio::getInstance()->setOnSoundEffect(m_bOnSoundEffect);
	closed();
}

//	点击取消按钮事件
void SGLYPopupSetting::onClickBtnNOEvent()
{
	closed();
}

