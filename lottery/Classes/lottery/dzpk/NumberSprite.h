#ifndef __NUMBER__SPRITE__
#define __NUMBER__SPRITE__
#pragma once
#include "cocos2d.h"

using namespace cocos2d;

//单个数字精灵
class NumberSprite :public cocos2d::CCSprite
{
public:
	NumberSprite(CCTexture2D *pTexture, CCSize size);
	~NumberSprite(void);
	void setValue(int v, bool isShow = true);

private:
	CCSize size;
	CCRect rect;
};
#endif
