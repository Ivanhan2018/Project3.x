#include "WindowInfo.h"
#include "ScriptData.h"

FV_SINGLETON_STORAGE(WindowInfo);

WindowInfo::WindowInfo()
{
	init();
}
WindowInfo::~WindowInfo(void)
{
}

void WindowInfo::init()
{
	m_kUseSize.width = ScriptData<float>("UseWidth").Value();
	m_kUseSize.height = ScriptData<float>("UseHeight").Value();

	m_kWinSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
}
void WindowInfo::changWidthScale(cocos2d::Node* pNode)
{
	float fScale = getChangeWidthScale();
	pNode->setScaleX(fScale);
}
void WindowInfo::BackWinSizeX(cocos2d::Node* pNode,int iPosType)
{
	cocos2d::Size kUseSize = WindowInfo::Instance().getUseSize();
	cocos2d::Vec2 kWinSize = WindowInfo::Instance().getWinSize();
	float fPercent = (kUseSize.height/kWinSize.y)/(kUseSize.width/kWinSize.x);
	pNode->setScaleY(fPercent);
	if (iPosType == 1)
	{
		int iPosY = pNode->getContentSize().height *(1-fPercent)/2;
		pNode->setPosition(pNode->getPosition().x,pNode->getPosition().y + iPosY);
	}
	if (iPosType == 2)
	{
		int iPosY = pNode->getContentSize().height *(1-fPercent)/2;
		pNode->setPosition(pNode->getPosition().x,pNode->getPosition().y - iPosY);
	}
	if (abs(iPosType) > 10)
	{
		pNode->setPosition(pNode->getPosition().x,pNode->getPosition().y + iPosType*(1-fPercent));
	}
}
void WindowInfo::BackWinSizeY(cocos2d::Node* pNode,int iPosType)
{
	cocos2d::Size kUseSize = WindowInfo::Instance().getUseSize();
	cocos2d::Vec2 kWinSize = WindowInfo::Instance().getWinSize();
	float fPercent = (kUseSize.width/kWinSize.x)/(kUseSize.height/kWinSize.y);
	pNode->setScaleX(fPercent);
	if (iPosType == 1)
	{
		int iPosX = pNode->getContentSize().height *(1-fPercent)/2;
		pNode->setPosition(pNode->getPosition().x - iPosX,pNode->getPosition().y);
	}
	if (iPosType == 2)
	{
		int iPosY = pNode->getContentSize().height *(1-fPercent)/2;
		pNode->setPosition(pNode->getPosition().x,pNode->getPosition().y - iPosY);
	}
	if (abs(iPosType) > 10)
	{
		pNode->setPosition(pNode->getPosition().x,pNode->getPosition().y + iPosType*(1-fPercent));
	}
}
void WindowInfo::changWinSize(cocos2d::Node* pNode)
{
	cocos2d::Size kUseSize = WindowInfo::Instance().getUseSize();
	cocos2d::Size kWinSize = WindowInfo::Instance().getWinSize();
	pNode->setScaleX(kWinSize.width/kUseSize.width);
	pNode->setScaleY(kWinSize.height/kUseSize.height);
}
void WindowInfo::BackWinSize(cocos2d::Node* pNode)
{
	cocos2d::Size kUseSize = WindowInfo::Instance().getUseSize();
	cocos2d::Size kWinSize = WindowInfo::Instance().getWinSize();
	pNode->setScaleX(kUseSize.width/kWinSize.width);
	pNode->setScaleY(kUseSize.height/kWinSize.height);
}
float WindowInfo::getChangeWidthScale()
{
	float fScaleWin = m_kWinSize.width/m_kWinSize.height;
	float fScaleUse = m_kUseSize.width/m_kUseSize.height;
	return fScaleUse/fScaleWin;
}
cocos2d::Vec2 WindowInfo::getRealPos(cocos2d::Vec2 kPos)
{
	float fScaleX = m_kWinSize.width/m_kUseSize.width;
	float fScaleY = m_kWinSize.height/m_kUseSize.height;
	kPos.x = kPos.x * fScaleX;
	kPos.y = kPos.y * fScaleY;
	return kPos;
}
cocos2d::Size WindowInfo::getRealSize(cocos2d::Size kSize)
{
	float fScaleX = m_kWinSize.width/m_kUseSize.width;
	float fScaleY = m_kWinSize.height/m_kUseSize.height;
	kSize.width = kSize.width * fScaleX;
	kSize.height = kSize.height * fScaleY;
	return kSize;
}
float WindowInfo::getChangeHeightScale()
{
	float fScaleWin = m_kWinSize.height/m_kWinSize.width;
	float fScaleUse = m_kUseSize.height/m_kUseSize.width;
	return fScaleUse/fScaleWin;
}
cocos2d::Size WindowInfo::getUseSize()
{
	return m_kUseSize;
}
cocos2d::Size WindowInfo::getWinSize()
{
	return m_kWinSize;
}
void WindowInfo::PreLoadScence()
{
	new ScriptDataManager();
}
void WindowInfo::delScence()
{

}
cocos2d::Vec2 WindowInfo::getCenter()
{
	return cocos2d::Vec2(m_kUseSize.width/2,m_kUseSize.height/2);
}
void WindowInfo::setShowSize(cocos2d::Size kSize)
{
	m_kShowSize = kSize;
}
cocos2d::Size WindowInfo::getShowSize()
{
	return m_kShowSize;
}