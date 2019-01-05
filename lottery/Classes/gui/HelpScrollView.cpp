#include "HelpLayer.h"
#include "RoomLayer.h"
#include "HelpScrollView.h"

using namespace cocos2d;
HelpScrollView::HelpScrollView()
{

}
HelpScrollView::~HelpScrollView()
{
}
Scene* HelpScrollView::scene()
{
	Scene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = Scene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		HelpScrollView *layer = HelpScrollView::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}
bool HelpScrollView::init()
{
	bool bRet = false;
	do 
	{

		//Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,1,true);	
		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = CC_CALLBACK_2(HelpScrollView::onTouchBegan, this);
		listener->onTouchCancelled = CC_CALLBACK_2(HelpScrollView::onTouchCancelled, this);
		listener->onTouchEnded = CC_CALLBACK_2(HelpScrollView::onTouchEnded, this);
		listener->onTouchMoved = CC_CALLBACK_2(HelpScrollView::onTouchMoved, this);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

		cocos2d::Size winSize = Director::getInstance()->getWinSize();

		/*Sprite *pPlayerInfoBJLeft1 = Sprite::createWithSpriteFrame(spriteFrame("dt_email8.jpg"));
		Sprite *pPlayerInfoBJRight1 = Sprite::createWithSpriteFrame(spriteFrame("dt_email8.jpg"));
		pPlayerInfoBJRight1->setRotationY(180);
		pPlayerInfoBJLeft1->setPosition(Vec2(winSize.width*0.25+5,winSize.height*0.0-18));
		pPlayerInfoBJRight1->setPosition(Vec2(winSize.width*0.75-5,winSize.height*0.0-18));
		pPlayerInfoBJLeft1->setScaleY(1.2f);
		pPlayerInfoBJRight1->setScaleY(1.2f);
		pPlayerInfoBJLeft1->setAnchorPoint(Vec2(0.5,0));
		pPlayerInfoBJRight1->setAnchorPoint(Vec2(0.5,0));
		this->addChild(pPlayerInfoBJLeft1,1);
		this->addChild(pPlayerInfoBJRight1,1);*/

		m_node = Node::create();
		this->addChild(m_node,2);


// 		LabelTTF *str1 = LabelTTF::create(ConfigMgr::instance()->text("t214"),"",18,cocos2d::Size(750,300),kCCTextAlignmentLeft);
// 		str1->setColor(ccc3(189,154,97));
// 		str1->setAnchorPoint(Vec2(0,0.5));
// 		str1->setPosition(Vec2(50.0f,175.0f));
// 		m_node->addChild(str1,3);
		Vec2 pt=Vec2(50.0f,175.0f+150.f+40);
		for (int i=0;i<32;i++)
		{
			const char *str =String::createWithFormat("t%d",209+i)->getCString();

			Rect rect=addString(pt,ConfigMgr::instance()->text(str),cocos2d::Size(0,0));
			pt.y-=rect.size.height+20;
		}
		m_endPos=Vec2(pt.x,-pt.y+50);

		//setTouchEnabled(true);
		bRet = true;
	} while (0);

	return bRet;
}



void HelpScrollView::onEnter()
{
	Layer::onEnter();

}

void HelpScrollView::onExit()
{
	
	Layer::onExit();
}
bool HelpScrollView::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	Vec2 touchPoint = Director::getInstance()->convertToGL(pTouch->getLocationInView());
	m_start = touchPoint.y;
	return true;
}
void HelpScrollView::onTouchMoved(Touch *pTouch, Event *pEvent)
{
	Vec2 touchPoint = Director::getInstance()->convertToGL(pTouch->getLocationInView());
	m_end = touchPoint.y;
	CCLOG("%d",m_node->getPositionY());
	if(m_node->getPositionY()<0){
		m_node->setPositionY(0);
	}
	if (m_node->getPositionY()>m_endPos.y)
	{
		m_node->setPositionY(m_endPos.y);
	}
	
	m_node->setPosition(Vec2(m_node->getPositionX(),m_node->getPositionY()+m_end-m_start));
	
	m_start = m_end;
}
void HelpScrollView::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	Vec2 touchPoint = Director::getInstance()->convertToGL(pTouch->getLocationInView());
}
void HelpScrollView::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
	Vec2 touchPoint = Director::getInstance()->convertToGL(pTouch->getLocationInView());
}

Rect HelpScrollView::addString( Vec2 pt,const char* buff,cocos2d::Size size )
{

	LabelTTF *str1 = LabelTTF::create(buff,"",18,cocos2d::Size(750,0),kCCTextAlignmentLeft);
	str1->setColor(ccc3(189,154,97));
	str1->setAnchorPoint(Vec2(0,1.0));
	str1->setPosition(pt);
	m_node->addChild(str1,3);
	return str1->boundingBox();
}
