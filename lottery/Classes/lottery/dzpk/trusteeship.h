#pragma once
#include "cocos2d.h"
#include "DZPKTools.h"

using namespace cocos2d;
class Trusteeship : public cocos2d::Layer
{
public:
	Trusteeship(void);
	~Trusteeship(void);
	virtual bool init();  
	static Trusteeship *create();
	void Countdown(float f);
	void trusteeStart();
	void trustee(float f);
	void trusteeEnd();
	void DrawCircle(float f);
	void CircleEnd();
	bool m_isTrusteeship;
private:
	//Director *director;
	unsigned short m_wCoutDown;
	Label *CountdownITF;
	Label *_label;
	Sprite * m_pLockSpr;
	Sprite * m_pCircleSpr;
	Sprite * m_pArrowlSpr[3];
	Sprite * m_pArrowrSpr[3];

	//点击事件的监听
	bool onTouchBegan(Touch *pTouch, Event *pEvent);  
	void onTouchEnded(Touch *pTouch, Event *pEvent); 

};


