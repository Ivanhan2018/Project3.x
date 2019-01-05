#include "SystemLayer.h"
#include "cocos/editor-support/cocostudio/CocoStudio.h"
#include "Sound.h"
#include "VisibleRect.h"
#include "PopupLayer.h"

USING_NS_CC;
using namespace cocostudio;
using namespace CocosDenshion;
using namespace ui;
using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ControlSliderEx : public ControlSlider
{
public:
	static ControlSliderEx* create(const char* bgFile, const char* progressFile, const char* thumbFile);
	static ControlSliderEx* create(Sprite * backgroundSprite, Sprite* pogressSprite, Sprite* thumbSprite);
	ControlSliderEx();
	virtual ~ControlSliderEx();
	virtual bool initWithSprites(Sprite * backgroundSprite, Sprite* progressSprite, Sprite* thumbSprite);
	virtual bool initWithSprites(Sprite * backgroundSprite, Sprite* progressSprite, Sprite* thumbSprite,Sprite* selectedThumbSprite);
	virtual void needsLayout();
	virtual void setMaximumValue(float val);
	virtual void setEnabled(bool enabled);
	virtual bool isTouchInside(Touch * touch);
	Vec2 locationFromTouch(Touch* touch);
	virtual void setValue(float val);
	virtual void setMinimumValue(float val);
protected:
    void sliderBegan(Vec2 location);
    void sliderMoved(Vec2 location);
    void sliderEnded(Vec2 location);

	virtual bool onTouchBegan(Touch* touch, Event* pEvent);
	virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
    float valueForLocation(Vec2 location);
    CC_SYNTHESIZE_READONLY(float, _value, Value);
    CC_SYNTHESIZE_READONLY(float, _minimumValue, MinimumValue);
    CC_SYNTHESIZE_READONLY(float, _maximumValue, MaximumValue);
    CC_SYNTHESIZE(float, _minimumAllowedValue, MinimumAllowedValue);
    CC_SYNTHESIZE(float, _maximumAllowedValue, MaximumAllowedValue);
    CC_SYNTHESIZE_RETAIN(Sprite*, _thumbSprite, ThumbSprite);
    CC_SYNTHESIZE_RETAIN(Sprite*, _selectedThumbSprite, SelectedThumbSprite);
    CC_SYNTHESIZE_RETAIN(Sprite*, _progressSprite, ProgressSprite);
    CC_SYNTHESIZE_RETAIN(Sprite*, _backgroundSprite, BackgroundSprite);
};

ControlSliderEx::ControlSliderEx()
	: _value(0.0f)
	, _minimumValue(0.0f)
	, _maximumValue(0.0f)
	, _minimumAllowedValue(0.0f)
	, _maximumAllowedValue(0.0f)
	, _thumbSprite(nullptr)
	, _selectedThumbSprite(nullptr)
	, _progressSprite(nullptr)
	, _backgroundSprite(nullptr)
{

}

ControlSliderEx::~ControlSliderEx()
{
	CC_SAFE_RELEASE(_thumbSprite);
	CC_SAFE_RELEASE(_selectedThumbSprite);
	CC_SAFE_RELEASE(_progressSprite);
	CC_SAFE_RELEASE(_backgroundSprite);
}

ControlSliderEx* ControlSliderEx::create(const char* bgFile, const char* progressFile, const char* thumbFile)
{
	Sprite *backgroundSprite      = Sprite::create(bgFile);
	Sprite *progressSprite        = Sprite::create(progressFile);
	Sprite *thumbSprite           = Sprite::create(thumbFile);
	return ControlSliderEx::create(backgroundSprite, progressSprite, thumbSprite);
}

ControlSliderEx* ControlSliderEx::create(Sprite * backgroundSprite, Sprite* pogressSprite, Sprite* thumbSprite)
{
	ControlSliderEx *pRet = new ControlSliderEx();
	pRet->initWithSprites(backgroundSprite, pogressSprite, thumbSprite);
	pRet->autorelease();
	return pRet;
}

bool ControlSliderEx::initWithSprites(Sprite * backgroundSprite, Sprite* progressSprite, Sprite* thumbSprite)
{
	Sprite* selectedThumbSprite = Sprite::createWithTexture(thumbSprite->getTexture(),
		thumbSprite->getTextureRect());
	selectedThumbSprite->setColor(Color3B::GRAY);
	return this->initWithSprites(backgroundSprite, progressSprite, thumbSprite, selectedThumbSprite);
}

bool ControlSliderEx::initWithSprites(Sprite * backgroundSprite, Sprite* progressSprite, Sprite* thumbSprite,
									Sprite* selectedThumbSprite)
{
	if (Control::init())
	{
		CCASSERT(backgroundSprite,      "Background sprite must be not nil");
		CCASSERT(progressSprite,        "Progress sprite must be not nil");
		CCASSERT(thumbSprite,           "Thumb sprite must be not nil");
		CCASSERT(selectedThumbSprite,   "Thumb sprite must be not nil");

		ignoreAnchorPointForPosition(false);

		this->setBackgroundSprite(backgroundSprite);
		this->setProgressSprite(progressSprite);
		this->setThumbSprite(thumbSprite);
		this->setSelectedThumbSprite(selectedThumbSprite);

		// Defines the content size
		Rect maxRect   = ControlUtils::RectUnion(backgroundSprite->getBoundingBox(), thumbSprite->getBoundingBox());

		setContentSize(cocos2d::Size(maxRect.size.width, maxRect.size.height));

		// Add the slider background
		_backgroundSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
		_backgroundSprite->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
		addChild(_backgroundSprite);

		// Add the progress bar
		_progressSprite->setAnchorPoint(Vec2(0.0f, 0.5f));
		_progressSprite->setPosition(Vec2(0.0f, this->getContentSize().height / 2));
		addChild(_progressSprite);

		// Add the slider thumb  
		_thumbSprite->setPosition(Vec2(0.0f, this->getContentSize().height / 2));
		addChild(_thumbSprite);

		_selectedThumbSprite->setPosition(Vec2(0.0f, this->getContentSize().height / 2));
		_selectedThumbSprite->setVisible(false);
		addChild(_selectedThumbSprite);

		// Init default values
		_minimumValue                   = 0.0f;
		_maximumValue                   = 1.0f;

		setValue(_minimumValue);
		return true;
	}
	else
	{
		return false;
	}
}


void ControlSliderEx::setEnabled(bool enabled)
{
	Control::setEnabled(enabled);
	if (_thumbSprite != nullptr) 
	{
		_thumbSprite->setOpacity((enabled) ? 255 : 128);
	}
}

void ControlSliderEx::setValue(float value)
{
	// set new value with sentinel
	if (value < _minimumValue)
	{
		value = _minimumValue;
	}

	if (value > _maximumValue) 
	{
		value = _maximumValue;
	}

	_value = value;

	this->needsLayout();

	this->sendActionsForControlEvents(Control::EventType::VALUE_CHANGED);
}

void ControlSliderEx::setMinimumValue(float minimumValue)
{
	_minimumValue=minimumValue;
	_minimumAllowedValue = minimumValue;
	if (_minimumValue >= _maximumValue)    
	{
		_maximumValue   = _minimumValue + 1.0f;
	}
	setValue(_value);
}

void ControlSliderEx::setMaximumValue(float maximumValue)
{
	_maximumValue=maximumValue;
	_maximumAllowedValue = maximumValue;
	if (_maximumValue <= _minimumValue)   
	{
		_minimumValue   = _maximumValue - 1.0f;
	}
	setValue(_value);
}

bool ControlSliderEx::isTouchInside(Touch * touch)
{
	Vec2 touchLocation   = touch->getLocation();
	touchLocation           = this->getParent()->convertToNodeSpace(touchLocation);

	Rect rect             = this->getBoundingBox();
	rect.size.width         += _thumbSprite->getContentSize().width;
	rect.origin.x           -= _thumbSprite->getContentSize().width / 2;

	return rect.containsPoint(touchLocation);
}

Vec2 ControlSliderEx::locationFromTouch(Touch* touch)
{
	Vec2 touchLocation   = touch->getLocation();                      // Get the touch position
	touchLocation           = this->convertToNodeSpace(touchLocation);                  // Convert to the node space of this class

	if (touchLocation.x < 0)
	{
		touchLocation.x     = 0;
	} else if (touchLocation.x > _backgroundSprite->getContentSize().width)
	{
		touchLocation.x     = _backgroundSprite->getContentSize().width;
	}

	return touchLocation;
}


bool ControlSliderEx::onTouchBegan(Touch* touch, Event* pEvent)
{
	if (!isTouchInside(touch) || !isEnabled() || !isVisible())
	{
		return false;
	}

	Vec2 location = locationFromTouch(touch);
	sliderBegan(location);
	return true;
}

void ControlSliderEx::onTouchMoved(Touch *pTouch, Event *pEvent)
{
	Vec2 location = locationFromTouch(pTouch);
	sliderMoved(location);
}

void ControlSliderEx::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	sliderEnded(Vec2::ZERO);
}

void ControlSliderEx::needsLayout()
{
	if (nullptr == _thumbSprite || nullptr == _selectedThumbSprite || nullptr == _backgroundSprite
		|| nullptr == _progressSprite)
	{
		return;
	}
	// Update thumb position for new value
	float percent               = (_value - _minimumValue) / (_maximumValue - _minimumValue);

	Vec2 pos                 = _thumbSprite->getPosition();
	pos.x                       = percent * _backgroundSprite->getContentSize().width;
	_thumbSprite->setPosition(pos);
	_selectedThumbSprite->setPosition(pos);

	// Stretches content proportional to newLevel
	Rect textureRect          = _progressSprite->getTextureRect();
	textureRect                 = Rect(textureRect.origin.x, textureRect.origin.y, pos.x, textureRect.size.height);
	_progressSprite->setTextureRect(textureRect, _progressSprite->isTextureRectRotated(), textureRect.size);
}

void ControlSliderEx::sliderBegan(Vec2 location)
{
	this->setSelected(true);
	_thumbSprite->setVisible(false);
	_selectedThumbSprite->setVisible(true);
	setValue(valueForLocation(location));
}

void ControlSliderEx::sliderMoved(Vec2 location)
{
	setValue(valueForLocation(location));
}

void ControlSliderEx::sliderEnded(Vec2 location)
{
	if (this->isSelected())
	{
		setValue(valueForLocation(_thumbSprite->getPosition()));
	}

	if (_value < 40)
	{
		_value = 20;
	}
	else
	{
		_value = 80;
	}
	_thumbSprite->setVisible(true);
	_selectedThumbSprite->setVisible(false);
	this->setSelected(false);
}

float ControlSliderEx::valueForLocation(Vec2 location)
{
	float percent = location.x/ _backgroundSprite->getContentSize().width;
	return MAX(MIN(_minimumValue + percent * (_maximumValue - _minimumValue), _maximumAllowedValue), _minimumAllowedValue);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SystemLayer::SystemLayer(void)
{
	m_widgetUI=nullptr;
	Panel_SystemLayer=nullptr;
	set_close=nullptr;
	CheckBox_chat=nullptr;
	CheckBox_cartoon=nullptr;
	CheckBox_Music = nullptr;
	CheckBox_Effect = nullptr;
	m_spSwitch = nullptr;
	shakeImage = nullptr;
	gameEffectImage = nullptr;
	musicImage = nullptr;
	effectImage = nullptr;
	m_TouchListener = nullptr;
}

SystemLayer::~SystemLayer(void)
{
// 	SetLayerTouchEnable(false);

}

SystemLayer* SystemLayer::create(float fScale,const char* pszFilePath)
{
	SystemLayer *ret = new SystemLayer();
	if (ret && ret->init(fScale, pszFilePath))
	{
		ret->autorelease();
		return ret;
	}
	else
	{
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
}
bool SystemLayer::init(float fScale,const char* pszFilePath)
{

	mpszFilePath = pszFilePath;

	LayerColor::initWithColor(Color4B(0,0,0,0));

	m_widgetUI = GUIReader::getInstance()->widgetFromJsonFile("PopWindow/zhongyou_system.json");
	this->addChild(m_widgetUI);

	Panel_SystemLayer = (Layout*)Helper::seekWidgetByName(m_widgetUI, "panel");

	this->setContentSize(m_widgetUI->getContentSize());
	set_close	= (Button*)Helper::seekWidgetByName(m_widgetUI,"btn_quit");
	set_close->setTag(0);
	set_close->setPressedActionEnabled(true);

	//公告标题	
	m_btnSwitchAccount	= (Button*)Helper::seekWidgetByName(m_widgetUI,"btn_switch_ac");
	m_btnSwitchAccount->addTouchEventListener(this, toucheventselector(SystemLayer::onSwitchAccount));

// 	ImageView* notice_title = (ImageView*)Helper::seekWidgetByName(m_widgetUI, "notice_title");
// 	notice_title->setVisible(false);

	//系统标题
// 	ImageView* server_title = (ImageView*)Helper::seekWidgetByName(m_widgetUI, "server_title");
// 	server_title->setVisible(false);
 
	/////////////////////////////////////////////////////    背景音乐  /////////////////////////////////////////////////////////
	musicImage = (ImageView*)Helper::seekWidgetByName(m_widgetUI, "game_music");
	auto* sliderMusic =  ControlSliderEx::create("Common/slide_2.png", "Common/slide_1.png", "Common/btn_switch.png");
	sliderMusic->setMinimumValue(0);
	sliderMusic->setMaximumValue(100);
	sliderMusic->setMinimumAllowedValue(20);
	sliderMusic->setMaximumAllowedValue(80);

	if (UserDefault::getInstance()->getBoolForKey("Music"))
	{
		sliderMusic->setValue(80);
	}
	else
	{
		sliderMusic->setValue(20);

	}
	
	musicImage->addChild(sliderMusic);
	sliderMusic->setAnchorPoint(Vec2(0, 0.5));
	sliderMusic->setPosition(Vec2(musicImage->getContentSize().width + 20, musicImage->getContentSize().height / 2));
	sliderMusic->addTargetWithActionForControlEvents(this, cccontrol_selector(SystemLayer::sliderMusicChange), Control::EventType::VALUE_CHANGED);

	/////////////////////////////////////////////////////    游戏音效  /////////////////////////////////////////////////////////
	effectImage = (ImageView*)Helper::seekWidgetByName(m_widgetUI, "game_effect");
	auto* sliderEffect =  ControlSliderEx::create("Common/slide_2.png", "Common/slide_1.png", "Common/btn_switch.png");
	sliderEffect->setMinimumValue(0);
	sliderEffect->setMaximumValue(100);
	sliderEffect->setMinimumAllowedValue(20);
	sliderEffect->setMaximumAllowedValue(80);

	if (UserDefault::getInstance()->getBoolForKey("MusicEffect"))
	{
		sliderEffect->setValue(80);
	}
	else
	{
		sliderEffect->setValue(20);
	}

	effectImage->addChild(sliderEffect);
	sliderEffect->setAnchorPoint(Vec2(0, 0.5));
	sliderEffect->setPosition(Vec2(musicImage->getContentSize().width + 20, musicImage->getContentSize().height / 2));
	sliderEffect->addTargetWithActionForControlEvents(this, cccontrol_selector(SystemLayer::sliderEffectChange), Control::EventType::VALUE_CHANGED);

#if 0
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	shakeImage = (ImageView*)Helper::seekWidgetByName(m_widgetUI, "shake_effect");
 	//创建震屏开关按钮
 	Sprite* nor = Sprite::create("Common/show_off.png");
 	Sprite* sel = Sprite::create("Common/show_on.png");
	MenuItemSprite* itemNor = MenuItemSprite::create(nor,nor);
	MenuItemSprite* itemSel = MenuItemSprite::create(sel,sel);
	MenuItemToggle* btnToggle = MenuItemToggle::createWithCallback(CC_CALLBACK_1(SystemLayer::shakeBtnCall,this),itemSel,itemNor,NULL);

 	//"震屏效果"开关按钮
 	if (!UserDefault::getInstance()->getBoolForKey("ShakeScreen"))
 	{
 		btnToggle->setSelectedIndex(1);
 	}
	else
	{
		btnToggle->setSelectedIndex(0);
	}
 		
 	Menu* btnShake = Menu::create(btnToggle,nullptr);
 	shakeImage->addChild(btnShake);
	btnShake->setAnchorPoint(Vec2(0, 0.5));
 	btnShake->setPosition(Vec2(shakeImage->getContentSize().width + 70, shakeImage->getContentSize().height / 2));
 
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	gameEffectImage = (ImageView*)Helper::seekWidgetByName(m_widgetUI, "game_special_effect");
 	//"游戏特效"开关按钮
 	Sprite* nor1 = Sprite::create("Common/show_off.png");
 	Sprite* sel1 = Sprite::create("Common/show_on.png");
 	MenuItemSprite* itemNor1 = MenuItemSprite::create(nor1,nor1);
 	MenuItemSprite* itemSel1 = MenuItemSprite::create(sel1,sel1);
 	MenuItemToggle* btnToggle1 = MenuItemToggle::createWithCallback(CC_CALLBACK_1(SystemLayer::gameEffect,this),itemSel1,itemNor1,NULL);
 	//根据音效的播放状态设置按钮的显示状态
 	if (!UserDefault::getInstance()->getBoolForKey("Cartoon"))
 	{
 		btnToggle1->setSelectedIndex(1);
 	}else
 	{
 		btnToggle1->setSelectedIndex(0);
 	}
 
 	Menu* btnGameEffect = Menu::create(btnToggle1,nullptr);
 	gameEffectImage->addChild(btnGameEffect);
	btnGameEffect->setAnchorPoint(Vec2(0, 0.5));
	btnGameEffect->setPosition(Vec2(gameEffectImage->getContentSize().width + 70, gameEffectImage->getContentSize().height / 2));

#endif
	//屏蔽底层触摸
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch*,Event*)->bool
	{
		return true;
	};
	//listener->onTouchEnded = CC_CALLBACK_2(SystemLayer::onTouchEnded, this);
	listener->setSwallowTouches(true);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
	this->setScale(0);
	this->runAction(Sequence::create(ScaleTo::create(0.3f,1.0f),CallFunc::create([&]{
		this->setOpacity(125);
	}),NULL));
	return true;
}

void SystemLayer::SetBtnCallBack(cocos2d::Ref* target,cocos2d::ui::SEL_TouchEvent selector)
{
	m_touchtarget = target;
	m_touchevent = selector;
// 	SetLayerTouchEnable(true);
	set_close->addTouchEventListener(target,selector);
}

void SystemLayer::SetSwitchAccountCallBack(cocos2d::Ref* target,cocos2d::ui::SEL_TouchEvent selector)
{
	m_Switchtarget = target;
	m_SwitchAccountEvent = selector;
	// 	SetLayerTouchEnable(true);
}


void SystemLayer::shakeBtnCall(cocos2d::Ref* pSender)
{
	bool shake = UserDefault::getInstance()->getBoolForKey("ShakeScreen");
	UserDefault::getInstance()->setBoolForKey("ShakeScreen",!shake);

	if (shake)
	{
		UserDefault::getInstance()->setBoolForKey("ShakeScreen",false);
	}
	else
	{
		UserDefault::getInstance()->setBoolForKey("ShakeScreen",true);
	}
}

void SystemLayer::gameEffect(cocos2d::Ref* pSender)
{
	bool cartoon = UserDefault::getInstance()->getBoolForKey("Cartoon");
	UserDefault::getInstance()->setBoolForKey("Cartoon", !cartoon);

	if (cartoon)
	{
		UserDefault::getInstance()->setBoolForKey("Cartoon",false);
	}
	else
	{
		UserDefault::getInstance()->setBoolForKey("Cartoon",true);
	}
}

void SystemLayer::sliderMusicChange(Ref* psender, Control::EventType event)
{
	ControlSliderEx* slider = (ControlSliderEx*)psender;

	if (slider->getValue() < 40)
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		UserDefault::getInstance()->setBoolForKey("Music",false);
	}
	else
	{
		//SimpleAudioEngine::getInstance()->rewindBackgroundMusic();
		SimpleAudioEngine::getInstance()->playBackgroundMusic(mpszFilePath,true);
		//int nRand = rand()%2;
		//if(nRand == 0)
		//{
		//	Sound::getInstance()->playBgMusic(MUSIC_TYPE_SceneBg);
		//}
		//else if(nRand == 1)
		//{
		//	Sound::getInstance()->playBgMusic(MUSIC_TYPE_BACKGROUND2);
		//}

		UserDefault::getInstance()->setBoolForKey("Music",true);
	}
}

void SystemLayer::sliderEffectChange(Ref* psender, Control::EventType event)
{
	ControlSliderEx* slider = (ControlSliderEx*)psender;

	if (slider->getValue() < 40)
	{
		UserDefault::getInstance()->setBoolForKey("MusicEffect",false);
	}
	else
	{
		UserDefault::getInstance()->setBoolForKey("MusicEffect",true);
	}
}


void SystemLayer::SetLayerTouchEnable(bool var)
{
	if (var)
	{
		if (m_TouchListener == nullptr)
		{	
			m_TouchListener = EventListenerTouchOneByOne::create();
			m_TouchListener->onTouchBegan = CC_CALLBACK_2(SystemLayer::onTouchBegan, this);
// 			m_TouchListener->onTouchMoved = CC_CALLBACK_2(SystemLayer::onTouchMoved, this);
// 			m_TouchListener->onTouchEnded = CC_CALLBACK_2(SystemLayer::onPriTouchEnded, this);
			m_TouchListener->retain();
		}

		_eventDispatcher->addEventListenerWithSceneGraphPriority(m_TouchListener, Panel_SystemLayer);
		m_TouchListener->setEnabled(true);
		m_TouchListener->setSwallowTouches(true);
	}
	else
	{
		if (m_TouchListener)
		{
			m_TouchListener->setEnabled(false);
			_eventDispatcher->removeEventListener(m_TouchListener);

		}
	}
}

bool SystemLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	Vec2 tp = touch->getLocationInView();

	if (!Panel_SystemLayer->getBoundingBox().containsPoint(tp))
	{
		if (m_touchtarget && m_touchevent)
		{
			(m_touchtarget->*m_touchevent)(set_close, TOUCH_EVENT_ENDED);
		}
	}
	return true;
}

void SystemLayer::MessageBox(const char* text,const std::function<void (cocos2d::Ref *,cocos2d::ui::TouchEventType )> &callBack, int tag1, int tag2)
{
	PopupLayer* msgBox = PopupLayer::create("msgbox_bg.png");
	msgBox->setPosition(VisibleRect::leftTop());
	msgBox->setTitle("");
	msgBox->setContentText(text, 24, 50, 150);
	msgBox->setCallBackFunc(callBack);
	msgBox->addButton("msgbox_btn_ok_nor.png","msgbox_btn_ok_sel.png",tag1);
	msgBox->addButton("msgbox_btn_cancle_nor.png","msgbox_btn_cancle_sel.png",tag2);
	this->addChild(msgBox,99);

}

void SystemLayer::onSwitchAccount(Ref* obj,TouchEventType type)
{
	if (type != TouchEventType::TOUCH_EVENT_ENDED)
	{
		return;
	}
	
}

void SystemLayer::HideSwitchAccount()
{
	m_btnSwitchAccount->setVisible(false);

	musicImage->setPositionY(musicImage->getPositionY() - 70);
	effectImage->setPositionY(effectImage->getPositionY() - 70);
}

// 
// void SystemLayer::onTouchMoved(Touch *touch, Event *unused_event)
// {
// 
// 	if (m_touchtarget && m_touchevent)
// 	{
// 		(m_touchtarget->*m_touchevent)(this, TOUCH_EVENT_MOVED);
// 	}
// 	return;
// 
// }
// 


// void SystemLayer::onPriTouchEnded(Touch *touch, Event *unused_event)
// {
// 	Vec2 tp = touch->getLocationInView();
// 
// 	if (!Panel_SystemLayer->getBoundingBox().containsPoint(tp))
// 	{
// 		if (m_touchtarget && m_touchevent)
// 		{
// 			(m_touchtarget->*m_touchevent)(set_close, TOUCH_EVENT_ENDED);
// 		}
// 	}
// 
// 
// 	return;
// }
