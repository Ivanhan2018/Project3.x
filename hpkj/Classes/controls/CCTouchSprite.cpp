#include "CCTouchSprite.h"
USING_NS_CC;

CCTouchSprite* CCTouchSprite::createWithPic(const char *fileName, int row, int cell)
{
	return CCTouchSprite::createWithPic(fileName, row, cell, NULL, NULL);
}

CCTouchSprite* CCTouchSprite::createWithPic(const char *fileName, int row, int cell, Object* rec, SEL_CallFuncN selector)
{
	/************************************************************************/
	/* 	const char *fileName 声明一个指向字符常量的指针（表示值不可以变）指向常量的指针 fileName is a point, pointer char const
	    char const *fileName 同上
	    char* const fileName 声明一个指向可变字符的指针常量（表示指针地址不可以变 地址可以变）常量指针 fileName is a point, pointer char const point
        const char* const fileName=&d   char const* const fileName=&d  指针和值都不可以变了
   */
	/************************************************************************/
	CCTouchSprite* pMySprite = new CCTouchSprite();
	if (pMySprite && pMySprite->initWithFile(fileName))
	{
		pMySprite->setRowIndex(row);
		pMySprite->setCellIndex(cell);
		pMySprite->c_pfnSelector = selector;
		pMySprite->m_pListener = rec;
		pMySprite->autorelease();

		if (selector)
		{
			//设置事件优先级高于tableview
			//Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);	

			auto listener = EventListenerTouchOneByOne::create();
			listener->setSwallowTouches(true);
			listener->onTouchBegan = CC_CALLBACK_2(CCTouchSprite::onTouchBegan, pMySprite);
			listener->onTouchCancelled = CC_CALLBACK_2(CCTouchSprite::onTouchCancelled, pMySprite);
			listener->onTouchEnded = CC_CALLBACK_2(CCTouchSprite::onTouchEnded, pMySprite);
			listener->onTouchMoved = CC_CALLBACK_2(CCTouchSprite::onTouchMoved, pMySprite);
			Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, pMySprite);
		}

		return pMySprite;
	}
	CC_SAFE_DELETE(pMySprite);
	return NULL;
}

CCTouchSprite* CCTouchSprite::createWithSpriteFrame(SpriteFrame *pSpriteFrame,int row, int cell, Object* rec, SEL_CallFuncN selector)
{
	CCTouchSprite* pMySprite = new CCTouchSprite();
	if (pMySprite && pMySprite->initWithSpriteFrame(pSpriteFrame))
	{
		pMySprite->setRowIndex(row);
		pMySprite->setCellIndex(cell);
		pMySprite->c_pfnSelector = selector;
		pMySprite->m_pListener = rec;
		pMySprite->autorelease();

		if (selector)
		{
			//设置事件优先级高于tableview
			//Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);	

			auto listener = EventListenerTouchOneByOne::create();
			listener->setSwallowTouches(true);
			listener->onTouchBegan = CC_CALLBACK_2(CCTouchSprite::onTouchBegan, pMySprite);
			listener->onTouchCancelled = CC_CALLBACK_2(CCTouchSprite::onTouchCancelled, pMySprite);
			listener->onTouchEnded = CC_CALLBACK_2(CCTouchSprite::onTouchEnded, pMySprite);
			listener->onTouchMoved = CC_CALLBACK_2(CCTouchSprite::onTouchMoved, pMySprite);
			Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, pMySprite);
		}

		return pMySprite;
	}
	CC_SAFE_DELETE(pMySprite);
	return NULL;
}

//获取系统毫秒数（双击一般时间间隔是250ms～300ms）
static inline long millisecondNow()
{
	timeval tms;	
	gettimeofday(&tms, NULL);	
	return (tms.tv_sec * 1000 + tms.tv_usec / 1000);
}


static inline bool isDoubleTouch()
{
	static long lastTouchTime = 0;
	long newTouchTime = millisecondNow();
	if (abs(newTouchTime-lastTouchTime) < 250)
	{
		lastTouchTime = 0;
		return true;
	}
	else
	{
		lastTouchTime = millisecondNow();
		return false;
	}
}

//void CCTouchSprite::myInit(){}

bool CCTouchSprite::onTouchBegan(Touch* pTouch, Event* pEvent)
{
	m_bMovedSprite = false;
	if (this->isClickSprite(pTouch))
	{
		if (isDoubleTouch())
		{
			m_bDoubleClick = true;
			return true;
		}
		m_bDoubleClick = false;

		return true;
	}
	return false;
}

void CCTouchSprite::onTouchMoved(Touch* pTouch, Event* pEvent)
{
	Vec2 deltaPoint=pTouch->getDelta();

	if(fabs(deltaPoint.x)>1||fabs(deltaPoint.y)>1)
	{
		m_bMovedSprite = true;
	}
}

void CCTouchSprite::onTouchEnded(Touch *pTouch, Event *pEvent) 
{
	if (!m_bMovedSprite)
	{
		m_bMovedSprite = false;
		if (m_pListener && c_pfnSelector)
		{
			(m_pListener->*c_pfnSelector)(this);
		}
	}
}

void CCTouchSprite::onTouchCancelled(Touch *pTouch, Event *pEvent)
{

}

void CCTouchSprite::onEnter()
{
	Sprite::onEnter();
}

void CCTouchSprite::onExit()
{
	Sprite::onExit();
}

bool CCTouchSprite::isClickSprite(Touch* pTouch)
{
	//返回当前触摸点的opengl坐标
	Vec2 touchPoint = pTouch->getLocation();

	//将世界坐标转换为当前view本地坐标
	//Vec2 reallyPoint = this->getParent()->convertToNodeSpace(touchPoint);

	Vec2 reallyPoint = this->getParent()->convertTouchToNodeSpace(pTouch);
	//获取当前基于父类的坐标系
	Rect rect = this->boundingBox();
	//CCLOG("x = %f,y=%f",touchPoint.x,touchPoint.y);
	//CCnode->convertToNodeSpace 或者  convertToWorldSpace 是基于当前Node的  与当前Node相关
	if (rect.containsPoint(reallyPoint))
	{
		return true;
	}
	return false;
}

CCTouchSprite::CCTouchSprite()
{
	setRowIndex(-1);
	setCellIndex(-1);
	m_bMovedSprite = false;
	c_pfnSelector = NULL;
	m_pListener = NULL;
	m_bDoubleClick = false;
}
CCTouchSprite::~CCTouchSprite()
{
	m_pListener = NULL;
	c_pfnSelector = NULL;
}