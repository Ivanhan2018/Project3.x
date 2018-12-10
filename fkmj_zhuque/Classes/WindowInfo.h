#pragma once
#include "cocos2d.h"
#include "FvSingleton.h"

class WindowInfo
	:public FvSingleton<WindowInfo> 
{
public:
	WindowInfo();
	~WindowInfo();
public:
	void init();
public:
	float getChangeWidthScale();
	float getChangeHeightScale();
	void PreLoadScence();
	void delScence();
	void changWinSize(cocos2d::Node* pNode);
	void BackWinSize(cocos2d::Node* pNode);
	void changWidthScale(cocos2d::Node* pNode);
	void BackWinSizeX(cocos2d::Node* pNode,int iPosType);
	void BackWinSizeY(cocos2d::Node* pNode,int iPosType);
	cocos2d::Size getUseSize();
	cocos2d::Size getWinSize();
	cocos2d::Vec2 getRealPos(cocos2d::Vec2 kPos);
	cocos2d::Size getRealSize(cocos2d::Size kSize);
	cocos2d::Vec2 getCenter();
	void setShowSize(cocos2d::Size kSize);
	cocos2d::Size getShowSize();
private:

	cocos2d::Size m_kUseSize;
	cocos2d::Size m_kShowSize;
	cocos2d::Size m_kWinSize; 
};

