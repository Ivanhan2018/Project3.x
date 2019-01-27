#include "FishSettingLayer.h"
#include "Sound.h"
#include "EntityMgr.h"
#include "MyNSString.h"
#include "RoomLayer.h"
#include "CMD_Fish.h"

USING_NS_CC;
using namespace ui;


FishSettingLayer::FishSettingLayer():
	m_btnQuit(NULL),
	m_btnSure(NULL),
	m_type(0),
	m_music(true),
	m_musicEffect(true)
{

}

FishSettingLayer::~FishSettingLayer()
{
}

bool FishSettingLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(FishSettingLayer::onTouchBegan, this);	
	listener->onTouchEnded = CC_CALLBACK_2(FishSettingLayer::onTouchEnded, this);
	listener->onTouchMoved = CC_CALLBACK_2(FishSettingLayer::onTouchMoved, this);
	listener->onTouchCancelled = CC_CALLBACK_2(FishSettingLayer::onTouchCancelled, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	this->setTouchEnabled(true);

	LayerColor* color = LayerColor::create(ccc4(0, 0, 0, 120));
	color->setContentSize(cocos2d::Size(1280, 720));
	this->addChild(color);
	this->setContentSize(color->getContentSize());
	
	m_bg = ui::ImageView::create("709/FishSettingLayer/fish_setting_bg.png");
	
	m_bg->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(m_bg);

	m_btnQuit = ui::Button::create("709/Introduce/btn_quit.png","","");
	m_btnQuit->setPosition(Vec2(m_bg->getContentSize().width - m_btnQuit->getContentSize().width / 2 - 20, m_bg->getContentSize().height - m_btnQuit->getContentSize().height / 2 - 20));
	m_btnQuit->addTouchEventListener(this,SEL_TouchEvent(&FishSettingLayer::buttonQuit));
	m_bg->addChild(m_btnQuit, 100);

	m_btnSure = ui::Button::create("709/FishSettingLayer/fish_sure_nomal.png","709/FishSettingLayer/fish_sure_press.png","");
	m_btnSure->setPosition(Vec2(m_bg->getContentSize().width / 2, m_bg->getContentSize().height * 0.25f));
	m_btnSure->addTouchEventListener(this,SEL_TouchEvent(&FishSettingLayer::buttonSure));
	m_bg->addChild(m_btnSure, 100);

	//m_btnCancle
	m_btnCancle = ui::Button::create("709/FishSettingLayer/fish_cancle_nomal.png","709/FishSettingLayer/fish_cancle_press.png","");
	m_btnCancle->setPosition(Vec2(m_bg->getContentSize().width * 0.75f, m_bg->getContentSize().height * 0.25f));
	m_btnCancle->addTouchEventListener(this,SEL_TouchEvent(&FishSettingLayer::buttonQuit));
	m_bg->addChild(m_btnCancle, 100);

	return true;
}

bool FishSettingLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	return true;
}
void FishSettingLayer::onTouchMoved(Touch *pTouch, Event *pEvent)
{

}
void FishSettingLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{

}
void FishSettingLayer::onTouchCancelled(Touch *pTouch, Event *pEvent)
{

}
void FishSettingLayer::buttonQuit(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type)
{
	if (type != TouchEventType::TOUCH_EVENT_ENDED)
		return;
	this->removeFromParent();
	switch (m_type)
	{
	case TAG_SETTING:
		break;
	case TAG_LEAVE:
		break;
	case TAG_TIP:
		break;
	case TAG_DISCONNECT:
		{
			leaveGame();
		}
		break;
	default:
		break;
	}
}

void FishSettingLayer::buttonSure(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type)
{
	if (type != TouchEventType::TOUCH_EVENT_ENDED)
		return;
	
	switch (m_type)
	{
	case TAG_SETTING:
		{
			//save music
			this->removeFromParent();
		}
		break;
	case TAG_IN_BACKGROUND:
	case TAG_DISCONNECT:
	case TAG_LEAVE:
		{
			//leave game;
			leaveGame();
		}
		break;
	case TAG_TIP:
		{
			//just tip;
			//leaveGame();
		}
		break;
	default:
		break;
	}
	
}

void FishSettingLayer::buttonMusicEffect(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type)
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
	UserDefault::getInstance()->setBoolForKey("MusicEffect", m_musicEffect);

	if (m_musicEffect)
	{
		m_btnMusicEffect->loadTextures("709/FishSettingLayer/fish_music_on.png","709/FishSettingLayer/fish_music_on.png","");
	}
	else
	{
		m_btnMusicEffect->loadTextures("709/FishSettingLayer/fish_music_off.png","709/FishSettingLayer/fish_music_off.png","");
	}
}

void FishSettingLayer::buttonMusicBg(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type)
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

	UserDefault::getInstance()->setBoolForKey("Music", m_music);
	if (m_music)
	{
		m_btnMusic->loadTextures("709/FishSettingLayer/fish_music_on.png","709/FishSettingLayer/fish_music_on.png","");
	}
	else
	{
		m_btnMusic->loadTextures("709/FishSettingLayer/fish_music_off.png","709/FishSettingLayer/fish_music_off.png","");
	}
	playBackgroudMusic(m_music);
}

void FishSettingLayer::initSettingLayer()
{
	m_btnCancle->setVisible(false);
	m_musicEffect = UserDefault::getInstance()->getBoolForKey("MusicEffect",true);
	m_music = UserDefault::getInstance()->getBoolForKey("Music",true);

	auto title = Sprite::create("709/FishSettingLayer/fish_title_setting.png");
	title->setPosition(Vec2(m_bg->getContentSize().width / 2, m_bg->getContentSize().height * 0.89f));
	m_bg->addChild(title);

	auto musicTxt = Sprite::create("709/FishSettingLayer/fish_setting_music.png");
	musicTxt->setPosition(Vec2(m_bg->getContentSize().width * 0.13f, m_bg->getContentSize().height * 0.6f));
	m_bg->addChild(musicTxt);

	m_btnMusic = ui::Button::create();
	if (m_music)
	{
		m_btnMusic->loadTextures("709/FishSettingLayer/fish_music_on.png","709/FishSettingLayer/fish_music_on.png","");
	}
	else
	{
		m_btnMusic->loadTextures("709/FishSettingLayer/fish_music_off.png","709/FishSettingLayer/fish_music_off.png","");
	}
	m_btnMusic->setPosition(Vec2(m_bg->getContentSize().width * 0.33f, m_bg->getContentSize().height * 0.6f));
	m_btnMusic->addTouchEventListener(this,SEL_TouchEvent(&FishSettingLayer::buttonMusicBg));
	m_bg->addChild(m_btnMusic);

	auto musicEffectTxt = Sprite::create("709/FishSettingLayer/fish_setting_effect.png");
	musicEffectTxt->setPosition(Vec2(m_bg->getContentSize().width * 0.6f, m_bg->getContentSize().height * 0.6f));
	m_bg->addChild(musicEffectTxt);

	m_btnMusicEffect = ui::Button::create();
	if (m_musicEffect)
	{
		m_btnMusicEffect->loadTextures("709/FishSettingLayer/fish_music_on.png","709/FishSettingLayer/fish_music_on.png","");
	}
	else
	{
		m_btnMusicEffect->loadTextures("709/FishSettingLayer/fish_music_off.png","709/FishSettingLayer/fish_music_off.png","");
	}
	m_btnMusicEffect->setPosition(Vec2(m_bg->getContentSize().width * 0.8f, m_bg->getContentSize().height * 0.6f));
	m_btnMusicEffect->addTouchEventListener(this,SEL_TouchEvent(&FishSettingLayer::buttonMusicEffect));
	m_bg->addChild(m_btnMusicEffect);

}

void FishSettingLayer::initTipLayer()
{
	m_btnCancle->setVisible(false);
	auto title = Sprite::create("709/FishSettingLayer/fish_title_text.png");
	title->setPosition(Vec2(m_bg->getContentSize().width / 2, m_bg->getContentSize().height * 0.89f));
	m_bg->addChild(title);

	auto text = LabelTTF::create("text tip tip...","", 26);
	text->setPosition(Vec2(m_bg->getContentSize().width / 2, m_bg->getContentSize().height * 0.65f));
	m_bg->addChild(text);
}

void FishSettingLayer::initLeaveLayer()
{
	auto title = Sprite::create("709/FishSettingLayer/fish_title_text.png");

	m_btnCancle->setPosition(Vec2(m_bg->getContentSize().width * 0.7f, m_bg->getContentSize().height * 0.25f));
	m_btnSure->setPosition(Vec2(m_bg->getContentSize().width * 0.3f, m_bg->getContentSize().height * 0.25f));
	
	title->setPosition(Vec2(m_bg->getContentSize().width / 2, m_bg->getContentSize().height * 0.89f));
	m_bg->addChild(title);

	Dictionary * strings = Dictionary::createWithContentsOfFile("fish_strings.xml");

	string tip = ((CCString*)strings->objectForKey("dialog_leave_game_tip"))->getCString();

	auto text = LabelTTF::create(tip,"", 26);
	text->setPosition(Vec2(m_bg->getContentSize().width / 2, m_bg->getContentSize().height * 0.6f));
	m_bg->addChild(text);
}

void FishSettingLayer::initDisconnectLayer()
{
	m_btnCancle->setVisible(false);
	m_btnQuit->setVisible(false);
	auto title = Sprite::create("709/FishSettingLayer/fish_title_text.png");
	title->setPosition(Vec2(m_bg->getContentSize().width / 2, m_bg->getContentSize().height * 0.89f));
	m_bg->addChild(title);
	Dictionary * strings = Dictionary::createWithContentsOfFile("fish_strings.xml");

	string tip = ((CCString*)strings->objectForKey("dialog_disconnect"))->getCString();
	auto text = LabelTTF::create(tip,"", 26);
	text->setPosition(Vec2(m_bg->getContentSize().width / 2, m_bg->getContentSize().height * 0.6f));
	m_bg->addChild(text);
}

void FishSettingLayer::initInBackgroundLayer()
{
	m_btnCancle->setVisible(false);
	m_btnQuit->setVisible(false);
	auto title = Sprite::create("709/FishSettingLayer/fish_title_text.png");
	title->setPosition(Vec2(m_bg->getContentSize().width / 2, m_bg->getContentSize().height * 0.89f));
	m_bg->addChild(title);
	Dictionary * strings = Dictionary::createWithContentsOfFile("fish_strings.xml");

	string tip = ((CCString*)strings->objectForKey("dialog_long_time_in_background"))->getCString();
	auto text = LabelTTF::create(tip,"", 26);
	text->setPosition(Vec2(m_bg->getContentSize().width / 2, m_bg->getContentSize().height * 0.6f));
	m_bg->addChild(text);
}

void FishSettingLayer::setLayerType(int type)
{
	m_type = type;
	switch (m_type)
	{
	case TAG_SETTING:
		initSettingLayer();
		break;
	case TAG_LEAVE:
		initLeaveLayer();
		break;
	case TAG_DISCONNECT:
		initDisconnectLayer();
		break;
	case TAG_TIP:
		initTipLayer();
		break;
	case TAG_IN_BACKGROUND:
		initInBackgroundLayer();
		break;
	default:
		break;
	}
}

void FishSettingLayer::leaveGame()
{
	Sound::getInstance()->StopMusic();
	
	if (EntityMgr::instance()->getDispatch()->isGameLinkOK())
	{
		//发送下分消息CMD_C_ExchangeFishScore exchangeFishScore;
		NS_lrbyFish::CMD_C_ExchangeFishScore exchangeFishScore;
		memset(&exchangeFishScore, 0, sizeof(NS_lrbyFish::CMD_C_ExchangeFishScore));
		exchangeFishScore.increase = 0;
		EntityMgr::instance()->getDispatch()->sendSocketDate(NS_lrbyFish::SUB_C_EXCHANGE_FISHSCORE, &exchangeFishScore, sizeof(NS_lrbyFish::CMD_C_ExchangeFishScore));
		EntityMgr::instance()->getDispatch()->SendPacketWithPerformStandup();

		this->schedule([=](float dt){
			this->unschedule("FishLeaveGame");
			EntityMgr::instance()->getDispatch()->closeGameSocket();

			EntityMgr::instance()->roomFrame()->reset();
			ConfigMgr::instance()->m_Config.m_isGameRun=false;

			MyNSString::toPortrait();
			Scene* s=Scene::create();

			RoomLayer* layer = RoomLayer::create();
			s->addChild(layer);

			Director::getInstance()->replaceScene(s);
		}, 0.1f, "FishLeaveGame");
		return;

		//EntityMgr::instance()->getDispatch()->SendPacketWithPerformStandup();
		//EntityMgr::instance()->getDispatch()->closeGameSocket();
	}
	
	EntityMgr::instance()->roomFrame()->reset();
	ConfigMgr::instance()->m_Config.m_isGameRun=false;

	MyNSString::toPortrait();
	Scene* s=Scene::create();

	RoomLayer* layer = RoomLayer::create();
	s->addChild(layer);

	Director::getInstance()->replaceScene(s);
}

void FishSettingLayer::playBackgroudMusic(bool isPlay)
{
	if (isPlay)
	{
		//Sound
		int nRand = rand()%4;
		if(nRand == 0)
		{
			Sound::getInstance()->playBgMusic(MUSIC_TYPE_BACKGROUND2);
		}
		else if(nRand == 1)
		{
			Sound::getInstance()->playBgMusic(MUSIC_TYPE_BACKGROUND2);
		}
		else if(nRand == 2)
		{
			Sound::getInstance()->playBgMusic(MUSIC_TYPE_BACKGROUND3);
		}
		else if(nRand == 3)
		{
			Sound::getInstance()->playBgMusic(MUSIC_TYPE_BACKGROUND4);
		}
	}
	else
	{
		Sound::getInstance()->StopMusic();
	}
}

void FishSettingLayer::playMusicEffect(bool isPlay)
{
	if (!isPlay)
	{
		Sound::getInstance()->StopEffect();
	}
}
