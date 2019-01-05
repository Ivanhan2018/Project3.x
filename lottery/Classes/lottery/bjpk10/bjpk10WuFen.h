#ifndef __GAME_BJPK10_H__
#define __GAME_BJPK10_H__
#include "bjpk10BetArea.h"
#include "cocos2d.h"
#include "cocos-ext.h"

#define AREA_BALL_TAG1			0
#define AREA_BALL_TAG2			20
#define AREA_BALL_TAG3			40
#define AREA_BALL_TAG4			60
#define AREA_BALL_TAG5			80
#define AREA_MENU_TAG1			200
#define AREA_MENU_TAG2			220
#define AREA_MENU_TAG3			240
#define AREA_MENU_TAG4			260
#define AREA_MENU_TAG5			280
#define AREA_GAP				AREA_BALL_TAG2-AREA_BALL_TAG1
#define INVALID_TAG				-100
#define BALL_NUM				10

#define MAX_NUMBER				10

USING_NS_CC;
USING_NS_CC_EXT;

class bjpk10WuFen : public bjpk10BetArea, public EditBoxDelegate
{

public:
	bjpk10WuFen();
	~bjpk10WuFen();
	virtual bool init();
	static Scene* scene();
	void pressKindButton(int tag);
	CREATE_FUNC(bjpk10WuFen);

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
	void cancelAllMenuSelect();
	virtual void setBetKind(PK10GameKind kind);

	virtual void editBoxEditingDidBegin(EditBox* editBox);
	virtual void editBoxEditingDidEnd(EditBox* editBox);
	virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
	virtual void editBoxReturn(EditBox* editBox){};
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
	void initDanshiArea();
	void initFushiArea(const int max_count);

	void initWArea(const char *titleXmlIndex);
	void initQArea(const char *titleXmlIndex);
	void initBArea(const char *titleXmlIndex);
	void initSArea(const char *titleXmlIndex);
	void initGWArea(const char *titleXmlIndex);
	void initBalls(Vec2 startPos, int startTag);
	void initDXDS(Vec2 startPos, int startTag);		//初始化大小单双
	void initHeZhiBalls(Vec2 startPos, int startTag);
	void initMenu(Vec2 menuPos, int startTag);
	void pressBig(Object *obj);
	void pressLittle(Object *obj);
	void pressAll(Object *obj);
	void pressSingle(Object *obj);
	void pressDouble(Object *obj);
	void pressClear(Object *obj);
	void cancelMenuSelect(int itemTag);

	void createRenXuanRandomNumbers();
	void createQianEr_ZuXuanRandomNumbers();
	void createQianSan_ZuXuanRandomNumbers();
	 virtual void onAcceleration(Acceleration* acc, Event* unused_event);

    int ballStatus[MAX_NUMBER][19];
    int menuStatus[MAX_NUMBER][6]; 
    int areaStatus[MAX_NUMBER];
public:

	//获取投注号码
	String* getGuanJunBetNumber();
	String* getGuanYaJunBetNumber();
	String* getQianSanBetNumber();
	String* getQianSiBetNumber();
	String* getQianWuBetNumber();
	String* getDingWeiDanBetNumber();
	String* getDaXiaoDanShuangBetNumber();
	String* getGuanYaHeZhiBetNumber();
	String* getQianEr_ZuXuanBetNumber();
	String* getQianSan_ZuXuanBetNumber();

	//增加
	String *getDanShiBetNumber();	
	void setDanShiCount();
	//单式随机
	void createDanShiRandomNumber();
};
#endif  //__Game__HelpScrollView__