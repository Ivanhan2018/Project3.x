#include "PPCSetDialog.h"
#include "PPCSound.h"
#include "PPCRes.h"

USING_NS_CC;
using namespace ui;

PPCSetDialog::PPCSetDialog():
	m_btnQuit(NULL),
	m_btnSure(NULL),
	m_music(true),
	m_musicEffect(true)
{

}

PPCSetDialog::~PPCSetDialog()
{
}

bool PPCSetDialog::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(PPCSetDialog::onTouchBegan, this);	
	listener->onTouchEnded = CC_CALLBACK_2(PPCSetDialog::onTouchEnded, this);
	listener->onTouchMoved = CC_CALLBACK_2(PPCSetDialog::onTouchMoved, this);
	listener->onTouchCancelled = CC_CALLBACK_2(PPCSetDialog::onTouchCancelled, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	this->setTouchEnabled(true);

	LayerColor* color = LayerColor::create(ccc4(0, 0, 0, 120));
	color->setContentSize(cocos2d::Size(1280, 720));
	this->addChild(color);
	this->setContentSize(color->getContentSize());
	
	m_bg = ui::ImageView::create(PPCTextureName::s_ppc_setting_bg);
	
	m_bg->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(m_bg);

	m_btnQuit = ui::Button::create(PPCTextureName::s_ppc_btn_closed_normal, PPCTextureName::s_ppc_btn_closed_selected,"",Widget::TextureResType::PLIST);
	m_btnQuit->setPosition(Vec2(m_bg->getContentSize().width - m_btnQuit->getContentSize().width / 2 - 20, m_bg->getContentSize().height - m_btnQuit->getContentSize().height / 2 - 20));
	m_btnQuit->addTouchEventListener(this,SEL_TouchEvent(&PPCSetDialog::buttonQuit));
	m_bg->addChild(m_btnQuit, 100);

	m_btnSure = ui::Button::create(PPCTextureName::s_ppc_btn_ok_normal, PPCTextureName::s_ppc_btn_ok_selected, "",Widget::TextureResType::PLIST);
	m_btnSure->setPosition(Vec2(m_bg->getContentSize().width / 2, m_bg->getContentSize().height * 0.27f));
	m_btnSure->addTouchEventListener(this,SEL_TouchEvent(&PPCSetDialog::buttonSure));
	m_bg->addChild(m_btnSure, 100);

	initSettingLayer();
	return true;
}

bool PPCSetDialog::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	return true;
}
void PPCSetDialog::onTouchMoved(Touch *pTouch, Event *pEvent)
{

}
void PPCSetDialog::onTouchEnded(Touch *pTouch, Event *pEvent)
{

}
void PPCSetDialog::onTouchCancelled(Touch *pTouch, Event *pEvent)
{

}

void PPCSetDialog::buttonSure(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type)
{
	if (type != TouchEventType::TOUCH_EVENT_ENDED)
		return;
	this->removeFromParent();
}

void PPCSetDialog::buttonQuit(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type)
{
	if (type != TouchEventType::TOUCH_EVENT_ENDED)
		return;
	this->removeFromParent();
}

void PPCSetDialog::buttonMusicEffect(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type)
{
	if (type != TouchEventType::TOUCH_EVENT_ENDED)
		return;
	
	if (m_musicEffect)
	{
		m_musicEffect = false;
	}
	else
	{
		m_musicEffect = true;
	}
	std::string name2 = PPC_EFFECT_SWITCH;
	UserDefault::getInstance()->setBoolForKey(name2.c_str(), m_musicEffect);

	if (m_musicEffect)
	{
		m_btnMusicEffect->loadTextures(PPCTextureName::s_ppc_setting_switches_on,PPCTextureName::s_ppc_setting_switches_on,"");
	}
	else
	{
		m_btnMusicEffect->loadTextures(PPCTextureName::s_ppc_setting_switches_off,PPCTextureName::s_ppc_setting_switches_off,"");
	}
}

void PPCSetDialog::buttonMusicBg(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type)
{
	if (type != TouchEventType::TOUCH_EVENT_ENDED)
		return;
	if (m_music)
	{
		m_music = false;
	}
	else
	{
		m_music = true;
	}
	std::string name1 = PPC_MUSICE_SWITCH;
	UserDefault::getInstance()->setBoolForKey(name1.c_str(), m_music);
	if (m_music)
	{
		m_btnMusic->loadTextures(PPCTextureName::s_ppc_setting_switches_on,PPCTextureName::s_ppc_setting_switches_on,"");
	}
	else
	{
		m_btnMusic->loadTextures(PPCTextureName::s_ppc_setting_switches_off,PPCTextureName::s_ppc_setting_switches_off,"");
	}
	playBackgroudMusic(m_music);
}

void PPCSetDialog::initSettingLayer()
{
	std::string name1 = PPC_MUSICE_SWITCH;
	std::string name2 = PPC_EFFECT_SWITCH;
	m_musicEffect = UserDefault::getInstance()->getBoolForKey(name2.c_str(),true);
	m_music = UserDefault::getInstance()->getBoolForKey(name1.c_str(),true);

	auto musicTxt = Sprite::create(PPCTextureName::s_ppc_setting_text_music);
	musicTxt->setPosition(Vec2(m_bg->getContentSize().width * 0.13f, m_bg->getContentSize().height * 0.55f));
	m_bg->addChild(musicTxt);

	m_btnMusic = ui::Button::create();
	if (m_music)
	{
		m_btnMusic->loadTextures(PPCTextureName::s_ppc_setting_switches_on,PPCTextureName::s_ppc_setting_switches_on,"");
	}
	else
	{
		m_btnMusic->loadTextures(PPCTextureName::s_ppc_setting_switches_off,PPCTextureName::s_ppc_setting_switches_off,"");
	}
	m_btnMusic->setPosition(Vec2(m_bg->getContentSize().width * 0.33f, m_bg->getContentSize().height * 0.55f));
	m_btnMusic->addTouchEventListener(this,SEL_TouchEvent(&PPCSetDialog::buttonMusicBg));
	m_bg->addChild(m_btnMusic);

	auto musicEffectTxt = Sprite::create(PPCTextureName::s_ppc_setting_text_music_effect);
	musicEffectTxt->setPosition(Vec2(m_bg->getContentSize().width * 0.6f, m_bg->getContentSize().height * 0.55f));
	m_bg->addChild(musicEffectTxt);

	m_btnMusicEffect = ui::Button::create();
	if (m_musicEffect)
	{
		m_btnMusicEffect->loadTextures(PPCTextureName::s_ppc_setting_switches_on,PPCTextureName::s_ppc_setting_switches_on,"");
	}
	else
	{
		m_btnMusicEffect->loadTextures(PPCTextureName::s_ppc_setting_switches_off,PPCTextureName::s_ppc_setting_switches_off,"");
	}
	m_btnMusicEffect->setPosition(Vec2(m_bg->getContentSize().width * 0.8f, m_bg->getContentSize().height * 0.55f));
	m_btnMusicEffect->addTouchEventListener(this,SEL_TouchEvent(&PPCSetDialog::buttonMusicEffect));
	m_bg->addChild(m_btnMusicEffect);

}



void PPCSetDialog::playBackgroudMusic(bool isPlay)
{
	if (isPlay)
	{
		PPCSound::getInstance()->playMusicByState();
	}
	else
	{
		PPCSound::getInstance()->StopMusic();
	}
}

void PPCSetDialog::playMusicEffect(bool isPlay)
{
	if (!isPlay)
	{
		PPCSound::getInstance()->StopEffect();
	}
}
