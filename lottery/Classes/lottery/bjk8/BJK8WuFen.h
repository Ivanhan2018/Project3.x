#ifndef __BJK8__Game__ChongQingWuFen__
#define __BJK8__Game__ChongQingWuFen__
#include "BJK8BetArea.h"
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

class BJK8WuFen : public BJK8BetArea
{

public:
	BJK8WuFen();
	~BJK8WuFen();
	virtual bool init();
	static Scene* scene();
	void pressKindButton(int tag);
	CREATE_FUNC(BJK8WuFen);

	void onEnter();
	void onExit();
	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchMoved(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);
	void onTouchCancelled(Touch *pTouch, Event *pEvent);
	virtual void onAcceleration(Acceleration* acc, Event* unused_event);

	virtual String *getBetNumber();
	virtual void createRandomNumbers();
	virtual void clearNumbers();

	virtual void setBetKind(BeiJingKuai8Kind kind);
	virtual void cancelAllMenuSelect();
private:
	int areaNum;
	Node *m_node;
	float m_start;
	float m_end;
	//Vec2 m_endPos;
	Vec2 startPoint;
	Vec2 endPoint;
	Vec2 pt;
	float areaTitleXDelta;
	int m_num;
	int m_repeat;	
	std::string m_betString;
	std::string m_inputString;

	//数组
	std::vector<Vec2>  m_vecPot;
	bool m_bDrag;
private:
	void initBetArea(const char *titleXmlIndex);			//	
	void initOtherBetArea(const int start,const int count); //start从0开始

	void initBalls(Vec2 startPos, int startTag);
	void initMenu(Vec2 menuPos, int startTag);

	void pressBig(Object *obj);
	void pressLittle(Object *obj);
	void pressAll(Object *obj);
	void pressSingle(Object *obj);
	void pressDouble(Object *obj);
	void pressClear(Object *obj);
	void cancelMenuSelect(int itemTag);

	//获取投注号码 num为位数个数
	String *getZhixuanBetNumber(int num);

	//获取随机号码；机选号码
	void createZhixuanRandomNumbers(int num);

	int ballStatus[80];
	int menuStatus[6]; 
};
#endif  //__Game__HelpScrollView__