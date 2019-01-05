#ifndef __XingYun__Game__ChongQingWuFen__
#define __XingYun__Game__ChongQingWuFen__
#include "XingYunBetArea.h"
#include "cocos2d.h"
#include "cocos-ext.h"

#define W_AREA_BALL_TAG			0
#define Q_AREA_BALL_TAG			10
#define B_AREA_BALL_TAG			20
#define S_AREA_BALL_TAG			30
#define G_AREA_BALL_TAG			40
#define HEZHI_AREA_BALL_TAG			50
#define W_AREA_MENU_TAG			90
#define Q_AREA_MENU_TAG			100
#define B_AREA_MENU_TAG			110
#define S_AREA_MENU_TAG			120
#define G_AREA_MENU_TAG			130
#define HEZHI_AREA_MENU_TAG			140
#define INVALID_TAG				-100
#define AREA_BALL_GAP				Q_AREA_BALL_TAG-W_AREA_BALL_TAG

USING_NS_CC;
USING_NS_CC_EXT;

class XingYunWuFen : public XingYunBetArea
{

public:
	XingYunWuFen();
	~XingYunWuFen();
	virtual bool init();
	static Scene* scene();
	void pressKindButton(int tag);
	CREATE_FUNC(XingYunWuFen);

	void onEnter();
	void onExit();
	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchMoved(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);
	void onTouchCancelled(Touch *pTouch, Event *pEvent);

	virtual String *getBetNumber();
private:
	int areaNum;
	Node *m_node;
	float m_start;
	float m_end;
	//Vec2 m_endPos;
	Vec2 startPoint;
	Vec2 endPoint;
	Vec2 pt;

	//สýื้
	std::vector<Vec2>  m_vecPot;
	bool m_bDrag;

private:

	void initBetArea();	
	void initHeBalls(Vec2 startPos, int startTag);
	
	int ballStatus[80];
};
#endif  //