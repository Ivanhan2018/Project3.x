#include "Button.h"

Button::Button(void)
{
    
}

Button::~Button(void)
{
    
}

Button *Button::create(const char *fileName)
{
    Button *button = new Button();
    if (button && button->initWithFile(fileName))
    {
        button->autorelease();
        return button;
    }
   
    CC_SAFE_DELETE(button);
    return NULL;
}

Button *Button::createWithSpriteFrameName(const char *fileName)
{
    Button *button = new Button();
    if (button && button->initWithSpriteFrameName(fileName))
    {
        button->autorelease();
        return button;
    }
    CC_SAFE_DELETE(button);
    return NULL;
}

bool Button::init()
{
    m_pfnCallFunc = NULL;
    m_pobj = NULL;
	status = 0;
    
    return true;
}

void Button::onEnter()
{
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -2, false);
    CCSprite::onEnter();
}

void Button::onExit()
{
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCSprite::onExit();
}

bool Button::isTouchInside(CCTouch* touch)
{
    CCPoint touchPoint = touch->getLocation();
    CCPoint localPos = this->getParent()->convertToNodeSpace(touchPoint);
    CCRect rc = this->boundingBox();
    
    bool isTouched = rc.containsPoint(localPos);
    
    return isTouched;
}

bool Button::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    beginPoint = touch->getLocation();
 
    if (!isTouchInside(touch))
    {
        return false;
	}
    /*
    if (!m_rect.containsPoint(touchPoint))
    {
        CCLog("out of boundbox[%f][%f]", touchPoint.x, touchPoint.y);
        return false;
    }*/
    
    status = 1;
    this->setColor(ccc3(100, 100, 100));
    
    return true;
}

void Button::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    if (!isTouchInside(touch))
    {
        if (status == 1) {
            status = 0;
            this->setColor(ccc3(255, 255, 255));
        }
    }
}

void Button::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
  
    if (status == 1) {
        status = 0;
        this->setColor(ccc3(255, 255, 255));
    }

    CCPoint touchPoint = touch->getLocation();
    if (abs((int)(beginPoint.x - touchPoint.x)) > 20 || abs((int)(beginPoint.y - touchPoint.y)) > 20)
        return;

    if (!isTouchInside(touch))
        return;
   /*
    if (!m_rect.containsPoint(touchPoint))
    {
        return;
    }
    */
    if (!isVisible()){
        return;
    }

    if (m_pobj && m_pfnCallFunc)
    {
        (m_pobj->*m_pfnCallFunc)(this->getTag());
    }
}

void Button::setSelector(CCObject* obj, SEL_COMMON selector)
{
    m_pfnCallFunc = selector;
    m_pobj = obj;
}

void Button::setDisplayRect(CCRect rect)
{
    m_rect = rect;
}
