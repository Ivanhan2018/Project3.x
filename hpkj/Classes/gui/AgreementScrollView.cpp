#include "AgreementLayer.h"
#include "RoomLayer.h"
#include "AgreementScrollView.h"

using namespace cocos2d;
AgreementScrollView::AgreementScrollView()
{

}
AgreementScrollView::~AgreementScrollView()
{
}
Scene* AgreementScrollView::scene()
{
	Scene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = Scene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		AgreementScrollView *layer = AgreementScrollView::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}
bool AgreementScrollView::init()
{
	bool bRet = false;
	do 
	{
		cocos2d::Size winSize = Director::getInstance()->getWinSize();

		Sprite *pPlayerInfoBJLeft1 = Sprite::createWithSpriteFrame(spriteFrame("dt_email8.jpg"));
		Sprite *pPlayerInfoBJRight1 =Sprite::createWithSpriteFrame(spriteFrame("dt_email8.jpg"));
		pPlayerInfoBJRight1->setRotationY(180);
		pPlayerInfoBJLeft1->setPosition(Vec2(winSize.width*0.25+5,winSize.height*0.0-18));
		pPlayerInfoBJRight1->setPosition(Vec2(winSize.width*0.75-5,winSize.height*0.0-18));
		pPlayerInfoBJLeft1->setScaleY(1.2f);
		pPlayerInfoBJRight1->setScaleY(1.2f);
		pPlayerInfoBJLeft1->setAnchorPoint(Vec2(0.5,0));
		pPlayerInfoBJRight1->setAnchorPoint(Vec2(0.5,0));
		this->addChild(pPlayerInfoBJLeft1,1);
		this->addChild(pPlayerInfoBJRight1,1);

		
		str1 = LabelTTF::create(pConfigMgr->text("t241"),"",18,cocos2d::Size(770,0),kCCTextAlignmentLeft);
		str1->setColor(ccc3(189,154,97));
		str1->setAnchorPoint(Vec2(0,1));
		str1->setPosition(Vec2(61.0f,378.0f));
		this->addChild(str1,2);

		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = CC_CALLBACK_2(AgreementScrollView::onTouchBegan, this);
		listener->onTouchCancelled = CC_CALLBACK_2(AgreementScrollView::onTouchCancelled, this);
		listener->onTouchEnded = CC_CALLBACK_2(AgreementScrollView::onTouchEnded, this);
		listener->onTouchMoved = CC_CALLBACK_2(AgreementScrollView::onTouchMoved, this);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
		

		bRet = true;
	} while (0);

	return bRet;
}

//void AgreementScrollView::visit()
//{
//	glEnable(GL_SCISSOR_TEST);
//	glScissor(22.0f,8.0f, 802.0f, 378.0f);//x, y, w, h
//	Layer::visit();//调用自己的类方法
//	glDisable(GL_SCISSOR_TEST);
//
//}

void AgreementScrollView::onEnter()
{
	Layer::onEnter();
	//Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,1,true);	
}

void AgreementScrollView::onExit()
{
	
	Layer::onExit();
}
bool AgreementScrollView::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	Vec2 touchPoint = Director::getInstance()->convertToGL(pTouch->getLocationInView());
	m_start = touchPoint.y;
	return true;
}
void AgreementScrollView::onTouchMoved(Touch *pTouch, Event *pEvent)
{
	Vec2 touchPoint = Director::getInstance()->convertToGL(pTouch->getLocationInView());
	m_end = touchPoint.y;
	if(str1->getPositionY()<378){
      str1->setPositionY(378);
	  
	}
	str1->setPosition(Vec2(str1->getPositionX(),str1->getPositionY()+m_end-m_start));

	m_start = m_end;
}
void AgreementScrollView::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	Vec2 touchPoint = Director::getInstance()->convertToGL(pTouch->getLocationInView());
}
void AgreementScrollView::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
	Vec2 touchPoint = Director::getInstance()->convertToGL(pTouch->getLocationInView());
}

