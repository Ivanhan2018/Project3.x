#include "SliderMenu.h"
#include "utility.h"

USING_NS_CC;


SliderMenu::SliderMenu()
	:m_fMaxPosY(1)
	,m_fMaxValue(1)
	,m_pControlItem(NULL)
	,m_pListener(NULL)
	,m_pfnSelector(NULL)
	,m_fActValue(0.0f)
	,m_fMaxPosX(1)
	,m_bIsMoveByX(false)
{

}
SliderMenu* SliderMenu::create(MenuItem* item)
{
	if (!item)
	{
		return NULL;
	}
	Vector<MenuItem*> kVector;
	kVector.pushBack(item);
	return SliderMenu::createWithArray(kVector);
}
SliderMenu* SliderMenu::createWithArray(Vector<MenuItem*> pArrayOfItems)
{
	SliderMenu *pRet = new SliderMenu();
	if (pRet && pRet->initWithArray(pArrayOfItems))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}
bool SliderMenu::onTouchBegan(Touch* touch, Event* event)
{

	m_fToucPosX = convertToNodeSpace(touch->getLocation()).x;
	m_fToucPosX -=m_pControlItem->getPositionX();

	m_fToucPosY = convertToNodeSpace(touch->getLocation()).y;
	m_fToucPosY -= m_pControlItem->getPositionY();
	return Menu::onTouchBegan(touch,event);
}
void SliderMenu::onTouchMoved(Touch* touch, Event* event)
{
	cocos2d::Vec2 kTouch = convertToNodeSpace(touch->getLocation());
	if (m_bIsMoveByX)
	{
		int x = (kTouch.x-m_fToucPosX);
		SetButtonPosX(x);
	}
	else
	{
		int y = (kTouch.y - m_fToucPosY);
		SetButtonPosY(y);
	}
	if (m_pListener && m_pfnSelector)
	{
		(m_pListener->*m_pfnSelector)(this);
	}
	return Menu::onTouchMoved(touch,event);
}

void SliderMenu::setTarget(Ref *rec, SEL_MenuHandler selector)
{
	m_pListener = rec;
	m_pfnSelector = selector;
}
void SliderMenu::setontrolItem(MenuItem* pItem)
{
	m_pControlItem = pItem;
}
void SliderMenu::AddPosY(float fPosY)
{
	if (fPosY == 0)
	{
		return;
	}
	float fY = m_pControlItem->getPosition().y + fPosY;
	SetButtonPosY(fY);
}
void SliderMenu::SetButtonPosY(float fPosY)
{
	fPosY = utility::fMin(fPosY,m_fMaxPosY);
	fPosY = utility::fMax(fPosY,0);
	m_pControlItem->setPosition(ccp(0,fPosY));

	m_fActValue = fPosY/m_fMaxPosY*m_fMaxValue;
	if (m_pListener && m_pfnSelector)
	{
		(m_pListener->*m_pfnSelector)(this);
	}
}
void SliderMenu::setActValue(float fValue)
{
	if (m_bIsMoveByX)
	{
		float fPosX = fValue/m_fMaxValue*m_fMaxPosX;
		SetButtonPosX(fPosX);
	}
	else
	{
		float fPosY = fValue/m_fMaxValue*m_fMaxPosY;
		SetButtonPosY(fPosY);
	}
	
}

float SliderMenu::getValue()
{
	return m_fActValue;
}
void SliderMenu::setMaxValue(float fValue)
{
	CCAssert(fValue > 0,"");
	m_fMaxValue = fValue;
}
float SliderMenu::getMaxValue()
{
	return m_fMaxValue;
}
void SliderMenu::setMaxPosY(float fValue)
{
	CCAssert(fValue > 0,"");
	m_fMaxPosY = fValue;
	m_fMaxPosX = fValue;
}

void cocos2d::SliderMenu::SetButtonPosX( float fPosX )
{
	fPosX = utility::fMin(fPosX,m_fMaxPosX);
	fPosX = utility::fMax(fPosX,0);
	m_pControlItem->setPosition(ccp(fPosX,0));

	m_fActValue = fPosX/m_fMaxPosX*m_fMaxValue;
}

void cocos2d::SliderMenu::setMoveByX( bool bMoveByX )
{
	m_bIsMoveByX = bMoveByX;
}
