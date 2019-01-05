#ifndef __GAME__CARDSPRITE__
#define __GAME__CARDSPRITE__
#pragma once

//牌精灵类 .h
//Created by xxxx on 14-1-6.

#include "cocos2d.h"
USING_NS_CC;

class CardSprite :public Sprite
{
public:
	//初始化牌
	static CardSprite * CardSpriteWith(unsigned char ucCardInfo);
	//更改牌面
	void changeCardWith(unsigned char ucCardInfo);
private:
	//初始化函数
	bool initCardSprite(unsigned char ucCardInfo);
	//存储所有牌面的Sprite
	Sprite* tmp;
	//存储所有牌面的CCTexture2D
	CCTexture2D* tmpTexture;
};
#endif
