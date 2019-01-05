#ifndef __LOTTERY__LotteryCartScrollView__
#define __LOTTERY__LotteryCartScrollView__
#include "CocosUnits.h"
#include "ConfigMgr.h"
#include "AnsString.h"
#include <iostream>
#include "cocos2d.h"

# ifndef DELTA
# define DELTA 10
# endif
#define LOTTERY_KIND_SPRITE_TAG			0
#define GAME_KIND_SPRITE_TAG			100
#define INVALID_TAG					-100

class LotteryCartScrollView : public Layer
{

public:
	LotteryCartScrollView();
	~LotteryCartScrollView();
	virtual bool init();
	static Scene* scene();
	void pressKindButton(int tag);
	CREATE_FUNC(LotteryCartScrollView);
public:
	void onEnter();
	void onExit();
	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchMoved(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);
	void onTouchCancelled(Touch *pTouch, Event *pEvent);
private:
	Node *m_node;
	float m_start;
	float m_end;
	Vec2 m_endPos;
	Vec2 startPoint;
	Vec2 endPoint;
	int selectedSpriteTag;
	Array *lotteryKindData;
	Array *gameKindData;
	int lotteryCount;
	int gameCount;
};
#endif  //__Game__HelpScrollView__