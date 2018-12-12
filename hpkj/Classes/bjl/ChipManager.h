#ifndef __GAME_CHIPMANAGER_H__
#define __GAME_CHIPMANAGER_H__
#pragma once

/***************************************
 筹码管理类
 游戏界面
 xxxx 2013.1.10
 ***************************************/

#include "cocos2d.h"
#include "Define.h"
USING_NS_CC;

class ChipManager : public cocos2d::Layer
{
public:
	ChipManager();
	~ChipManager();
	//创建函数
	static ChipManager *create();
	//初始化底注
	bool initTotalChipValue();
	
	//设置底注
	void showGetChipManager(BYTE cbBetArea,LONGLONG lBetScore);

	//设置筹码精灵
	void showChipManagerSprite(BYTE cbBetArea, LONGLONG lBetScore);

	//底注初始化为0
	void initGetChipManager();

	//显示底注
	void showTotalChipValue();
	//隐藏
	void dissRemoveChuoValue();

private:
	cocos2d::Size WinSize;
	Sprite*   m_pSpriteNunber[8];     //总注
	CCLabelAtlas* m_pTotalChipValue[8];  	//底注数额
	Array *ChipManagerArray;
	Sprite* m_pTotalChipsBG;
};

#endif