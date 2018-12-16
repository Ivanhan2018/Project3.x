#include "ViewMenuLayer.h"

ViewMenuLayer::ViewMenuLayer()
{

}

ViewMenuLayer::~ViewMenuLayer()
{
}
bool ViewMenuLayer::init()
{
	cocos2d::Size size = WINSIZE;
	m_isVisibleStart = false;
	m_menuPositionY = 200.0f;		//按钮坐标Y位置
	if(!Layer::init())
	{
		return false;
	}
	do 
	{
		this->setKeypadEnabled(true);
		m_isAutoHints = true;	
		//计时器
		m_timer = MyTimer::create();
		m_timer->setPosition(ccp(size.width*0.04,size.height*0.2+25));
		this->addChild(m_timer);
		m_timer->setVisible(false);

	} while (0);
	return true;
}


void ViewMenuLayer::addMenuBarItem(const char * normal,const char * selected,const char * disable, Object * context,SEL_MenuHandler selector,Vec2 point, Object * pUserObject, int tag,bool visible)
{
	SpriteFrame * pFrameNormal = SpriteFrameCache::getInstance()->spriteFrameByName(normal);
	SpriteFrame * pFrameSelected = SpriteFrameCache::getInstance()->spriteFrameByName(normal);
	SpriteFrame * pFrameDisable = SpriteFrameCache::getInstance()->spriteFrameByName(normal);
	Sprite * normalSprite = Sprite::createWithSpriteFrame(pFrameNormal);
	Sprite * selectedSprite = Sprite::createWithSpriteFrame(pFrameSelected);
	selectedSprite->setColor(ccc3(100,100,100));
	Sprite * disableSprite = Sprite::createWithSpriteFrame(pFrameDisable);
	MenuItemSprite * menuItemSprite = MenuItemSprite::create(normalSprite,selectedSprite,disableSprite,context,selector);
	menuItemSprite->setPosition(point);
	menuItemSprite->setUserObject(pUserObject);
	menuItemSprite->setTag(tag);
	menuItemSprite->setVisible(visible);
	m_menuBar->addChild(menuItemSprite);
}

void ViewMenuLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if(keyCode == EventKeyboard::KeyCode::KEY_BACK)
	{
		//NotificationCenter::getInstance()->postNotification(MESSAGE_MENUCLICKED,Integer::create(menu_menuBarBackItem));
	}
}


void ViewMenuLayer::setTimer(int userId,Game_State gameState,int t,bool visible,int key)
{
	if(!visible)
	{
		m_timer->setVisible(false);
		return;
	}
	cocos2d::Size size = WINSIZE;
	m_timer->unscheduleAllSelectors();
	if (!m_timer->isVisible())
	{
		m_timer->setVisible(true);
	}
	m_timer->setPosition(ccp(size.width*0.04,size.height*0.2+25));
	m_timer->start(userId,t,gameState,key);
	
}
void ViewMenuLayer::closeTimer()
{
	m_timer->unscheduleAllSelectors();
	m_timer->setVisible(false);
}

Sprite* ViewMenuLayer::addMenuBarTips( const char * name,float delTime,Vec2 pt )
{
	Sprite* sptBack=Sprite::createWithSpriteFrameName(texture_name::s_bubble);
	sptBack->setPosition(pt);
	Sprite* spt=Sprite::createWithSpriteFrameName(name);
	spt->setPosition(ccp(sptBack->getContentSize().width*0.5,sptBack->getContentSize().height*0.5));
	spt->setAnchorPoint(ccp(0.5f,0.5f));
	spt->setOpacity(0);
	spt->runAction(Sequence::create(DelayTime::create(delTime),CCFadeIn::create(0.2f),NULL));
	sptBack->setOpacity(0);
	sptBack->runAction(Sequence::create(DelayTime::create(delTime),CCFadeIn::create(0.2f),CCJumpBy::create(0.6f,ccp(0.0f,0.0f),5.0f,1),
		CCFadeOut::create(0.2f),CallFuncN::create(this,callfuncN_selector(ViewMenuLayer::removeSelfCall)),NULL));
	sptBack->addChild(spt);
	return sptBack;
}

void ViewMenuLayer::removeSelfCall(Node* _node)
{
	if(_node)_node->removeFromParent();
}

void ViewMenuLayer::setMenuNameTipsView( bool isVisible )
{
#define Up_posY  42.0f
	m_menuBarTips->removeAllChildren();
	if(isVisible)
	{
		m_menuBarTips->addChild(addMenuBarTips(texture_name::s_font_chat,1.0f,ccp(-140.0f,-26.0f-Up_posY)),166,166);
		m_menuBarTips->addChild(addMenuBarTips(texture_name::s_font_robot,3.0f,ccp(-70.0f,-26.0f-Up_posY)),166,166);
		m_menuBarTips->addChild(addMenuBarTips(texture_name::s_font_task,5.0f,ccp(0,-26.0f-Up_posY)),166,166);
		m_menuBarTips->addChild(addMenuBarTips(texture_name::s_font_setup,7.0f,ccp(70.0f,-26.0f-Up_posY)),166,166);
		m_menuBarTips->addChild(addMenuBarTips(texture_name::s_font_back,9.0f,ccp(140.0f,-26.0f-Up_posY)),166,166);
	}
}

void ViewMenuLayer::SetTimeSprite()
{
}

