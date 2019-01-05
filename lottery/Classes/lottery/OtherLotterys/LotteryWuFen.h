#ifndef __LOTTERY__Game__ChongQingWuFen__
#define __LOTTERY__Game__ChongQingWuFen__
#include "LotteryBetArea.h"
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

class LotteryWuFen : public LotteryBetArea
{

public:
	LotteryWuFen();
	~LotteryWuFen();
	virtual bool init();
	static Scene* scene();
	void pressKindButton(int tag);
	CREATE_FUNC(LotteryWuFen);

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
	void clearNumbers(int area);

	virtual void setBetKind(QiXingCaiKind kind);
	void cancelAllMenuSelect();

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
	void initWArea(const char *titleXmlIndex);			//万位
	void initQArea(const char *titleXmlIndex);			//千位
	void initBArea(const char *titleXmlIndex);			//百位
	void initSArea(const char *titleXmlIndex);			//十位
	void initGWArea(const char *titleXmlIndex);			//各位

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
	String *getBuDingWeiBetNumber(int num);	

	//获取随机号码；机选号码
	void createZhixuanRandomNumbers(int num);
	void createRenxuanRandomNumber(int num); //任选num 个数  总共是5个
	void createZuxuanRandomNumbers(int num);
	
	int ballStatus[6][30];
	int menuStatus[6][6]; 
	int areaStatus[6];
};
#endif  //__Game__HelpScrollView__