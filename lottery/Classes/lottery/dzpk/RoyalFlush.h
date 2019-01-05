#pragma once
#include "cocos2d.h"
using namespace cocos2d;
class RoyalFlush : public cocos2d::Layer
{
public:
	RoyalFlush(void);
	~RoyalFlush(void);
	virtual bool init();  
	static RoyalFlush *create();

	void RoyalStart();


private:

	Sprite * m_pCardSprA;
	Sprite * m_pCardSprK;
	Sprite * m_pCardSprQ;
	Sprite * m_pCardSprJ;
	Sprite * m_pCardSpr10;
	Sprite * m_pBadgeSpr;
	Sprite * m_pZiSpr0;
	Sprite * m_pZiSpr1;
	Sprite * m_pBackLightSpr;
	Sprite * m_pBackLightSpr1;


private:
	void CardA(float f);
	void CardK(float f);
	void CardQ(float f);
	void CardJ(float f);
	void Card10(float f);
	void CardAni(float f);
	void BackLight(float f);
	void End();


};




