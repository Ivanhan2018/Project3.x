#ifndef __ChongQingGroupSingle__
#define __ChongQingGroupSingle__
#include "ChongQingBetArea.h"

#define W_AREA_BALL_TAG			0
#define Q_AREA_BALL_TAG			10
#define B_AREA_BALL_TAG			20
#define S_AREA_BALL_TAG			30
#define G_AREA_BALL_TAG			40
#define W_AREA_MENU_TAG			50
#define Q_AREA_MENU_TAG			60
#define B_AREA_MENU_TAG			70
#define S_AREA_MENU_TAG			80
#define G_AREA_MENU_TAG			90
#define INVALID_TAG				-100

class ChongQingGroupSingle : public ChongQingBetArea
{

public:
	ChongQingGroupSingle();
	~ChongQingGroupSingle();
	virtual bool init();
	static Scene* scene();
	void pressKindButton(int tag);
	CREATE_FUNC(ChongQingGroupSingle);

	void onEnter();
	void onExit();
	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchMoved(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);
	void onTouchCancelled(Touch *pTouch, Event *pEvent);
	virtual String *getBetNumber();
	virtual void createRandomNumbers();
	virtual void clearNumbers();
	void clearNumbers(int area);
private:
	Node *m_node;
	float m_start;
	float m_end;
	Vec2 m_endPos;
	Vec2 startPoint;
	Vec2 endPoint;
	Vec2 pt;
	float areaTitleXDelta;
private:
	void initWArea();			//万位
	void initQArea();			//千位
	void initBArea();			//百位
	void initSArea();			//十位
	void initGWArea();			//个位
	void initDXDS(Vec2 startPos, int startTag);		//初始化UI:大小单双
	void initBalls(Vec2 startPos, int startTag);		//初始化UI:数字0-9
	void initMenu(Vec2 menuPos, int startTag);		//初始化UI:菜单
	void pressBig(Object *obj);
	void pressLittle(Object *obj);
	void pressAll(Object *obj);
	void pressSingle(Object *obj);
	void pressDouble(Object *obj);
	void pressClear(Object *obj);
	void cancelMenuSelect(int itemTag);
};
#endif  //__Game__HelpScrollView__