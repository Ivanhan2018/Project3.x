#ifndef __Game__GuangDong11Xuan5WuFen__
#define __Game__GuangDong11Xuan5WuFen__
#include "GuangDong11Xuan5BetArea.h"
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
#define BALL_NUM				11

USING_NS_CC;
USING_NS_CC_EXT;

class GuangDong11Xuan5WuFen : public GuangDong11Xuan5BetArea, public EditBoxDelegate
{

public:
	GuangDong11Xuan5WuFen();
	~GuangDong11Xuan5WuFen();
	virtual bool init();
	static Scene* scene();
	void pressKindButton(int tag);
	CREATE_FUNC(GuangDong11Xuan5WuFen);

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
	virtual void setBetKind(tagEnumIIXuan5 kind);

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

	bool m_isDanTuo;
private:
	void initDanshiArea();
	void initArea(const char *titleXmlIndex);
	void initWArea(const char *titleXmlIndex);
	void initQArea(const char *titleXmlIndex);
	void initBArea(const char *titleXmlIndex);
	void initSArea(const char *titleXmlIndex);
	void initGWArea(const char *titleXmlIndex);
	void initBalls(Vec2 startPos, int startTag);
	void initMenu(Vec2 menuPos, int startTag);
	void pressBig(Object *obj);
	void pressLittle(Object *obj);
	void pressAll(Object *obj);
	void pressSingle(Object *obj);
	void pressDouble(Object *obj);
	void pressClear(Object *obj);
	void cancelMenuSelect(int itemTag);

	void createRenXuanRandomNumbers();
	void createRenXuanDanTuoRandomNumbers();
	void createQianEr_ZuXuanRandomNumbers();
	void createQianSan_ZuXuanRandomNumbers();
	 virtual void onAcceleration(Acceleration* acc, Event* unused_event);

    int ballStatus[5][11];
    int menuStatus[5][6]; 
    int areaStatus[5];
public:

	//获取投注号码
	String* getIIRenXuan1BetNumber();
	String* getIIRenXuan2BetNumber();
	String* getIIRenXuan3BetNumber();
	String* getIIRenXuan4BetNumber();
	String* getIIRenXuan5BetNumber();
	String* getIIRenXuan6BetNumber();
	String* getIIRenXuan7BetNumber();
	String* getIIRenXuan8BetNumber();
	String* getIIRenXuanDanTuoBetNumber(const int count);
	String* getQianYiBetNumber();
	String* getQianEr_ZuXuanBetNumber();
	String* getQianEr_ZhiXuanBetNumber();
	String* getQianSan_ZhiXuanBetNumber();
	String* getQianSan_ZuXuanBetNumber();
	//增加
	String *getDanShiBetNumber();	
	void setDanShiCount();
	//单式随机
	void createDanShiRandomNumber();
};
#endif  //__Game__HelpScrollView__