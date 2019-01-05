//
//  FreeCoins.h
//  Game
//
//  Created by wangjun on 15-1-13.
//
//

#ifndef __Game__LuckyDraw__
#define __Game__LuckyDraw__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Define.h"
#include "GBEvent.h"
USING_NS_CC;
USING_NS_CC_EXT;

//enum FreeCoinsTab
//{
//	FreeCoinsTabAlms,
//	FreeCoinsTabRecommend,
//	FreeCoinsTabRecommender
//};

class LuckyDraw : public Layer
{
private:
    int lastLuckyNum;
    SCORE moneyEarned;
	Sprite *pinSprite;
	Menu *drawMenu;
	bool canDraw;
public:
	CREATE_FUNC(LuckyDraw);
	virtual bool init();

	LuckyDraw();
	~LuckyDraw();
	void back(Object* obj);
	void pressDraw( Object* obj);
	void onEnter();
	void onExit();
	void pinMove(int tag);
	void pinMoveEnd(Node *obj);
	void addRandSeed();
    void turnTableRet(Object *pObject);
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);					//�ֻ����ذ�ť

};

#endif /* defined(__Game__RegistScene__) */
