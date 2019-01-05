#ifndef __Game__ChongQingWuFen__
#define __Game__ChongQingWuFen__
#include "ChongQingBetArea.h"
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

class ChongQingWuFen : public ChongQingBetArea, public EditBoxDelegate
{

public:
	ChongQingWuFen();
	~ChongQingWuFen();
	virtual bool init();
	static Scene* scene();
	void pressKindButton(int tag);
	CREATE_FUNC(ChongQingWuFen);

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

	virtual void setBetKind(SSCWanFaKind kind);
	void cancelAllMenuSelect();

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
	void initLongHuHe();
	void initDanshiArea();
	void initWArea(const char *titleXmlIndex);			//万位
	void initQArea(const char *titleXmlIndex);			//千位
	void initBArea(const char *titleXmlIndex);			//百位
	void initSArea(const char *titleXmlIndex);			//十位
	void initGWArea(const char *titleXmlIndex);			//各位
	void initHeadArea(const char *titleXmlIndex);		//个十百千万的头
	void initHeZhiArea(const char *titleXmlIndex);
	void initDXDS(Vec2 startPos, int startTag);		//初始化大小单双
	void initBalls(Vec2 startPos, int startTag);		//初始化小球
	void initHeZhiBalls(Vec2 startPos, int startTag);
	void initMenu(Vec2 menuPos, int startTag);
	void pressBig(Object *obj);
	void pressLittle(Object *obj);
	void pressAll(Object *obj);
	void pressSingle(Object *obj);
	void pressDouble(Object *obj);
	void pressClear(Object *obj);
	void cancelMenuSelect(int itemTag);

	//获取投注号码
	String *getZhixuanBetNumber();
	String *getBuDingWeiBetNumber();

	String *getWuxingZhixuanBetNumber();
	String *getZuxuan120BetNumber();
	String *getZuxuan60BetNumber();
	String *getZuxuan30BetNumber();
	String *getZuxuan20BetNumber();
	String *getZuxuan10BetNumber();
	String *getZuxuan5BetNumber();
	//前四
	String *getZuxuanQS24BetNumber();
	String *getZuxuanQS12BetNumber();
	String *getZuxuanQS6BetNumber();
	String *getZuxuanQS4BetNumber();
	//后四
	String *getZuxuanHS24BetNumber();
	String *getZuxuanHS12BetNumber();
	String *getZuxuanHS6BetNumber();
	String *getZuxuanHS4BetNumber();

	String *getWuxingQianSiBetNumber();
	String *getWuxingHousiBetNumber();
	String *getQianSanZhiXuan_FuShiBetNumber();
	//String *getQianSanZhiXuan_DanShiBetNumber();
	String *getQianSanZuXuan_ZuSanFuShiBetNumber();
	String *getQianSanZuXuan_ZuLiuFuShiBetNumber();
	String *getQianSanZhiXuanHeZhiBetNumber();
	String *getZhongSanZhiXuan_FuShiBetNumber();
	String *getZhongSanZuXuan_ZuSanFuShiBetNumber();
	String *getZhongSanZuXuan_ZuLiuFuShiBetNumber();
	String *getZhongSanZhiXuanHeZhiBetNumber();
	String *getHouSanZhiXuan_FuShiBetNumber();
	String *getHouSanZuXuan_ZuSanFuShiBetNumber();
	String *getHouSanZuXuan_ZuLiuFuShiBetNumber();
	String *getHouSanZhiXuanHeZhiBetNumber();
	String *getWuXingHouEr_FuShiBetNumber();
	String *getWuXingQianEr_FuShiBetNumber();
	String *getHouErZuXuan_FuShiBetNumber();
	String *getQianErZuXuan_FuShiBetNumber();
	String *getDingWeiDanBetNumber();
	String *GetDaXiaoDanShuang_DW(bool isRepeat = false,int num = 5);
	String *getBuDingWei_HouSanBetNumber();
	String *getBuDingWei_QianSanBetNumber();
	String *getBuDingWei_HouErBetNumber();
	String *getBuDingWei_QianErBetNumber();
	String *getQW_YiFanFengShunBetNumber();
	String *getQW_HaoShiChengShuangBetNumber();
	String *getQW_SanXingBaoXiBetNumber();
	String *getQW_SiJiFaCaiBetNumber();
	String *getSSCLongHuBetNumber();
	//增加
	String *getDanShiBetNumber();	
	void setDanShiCount();
	//任选
	String *getRen4BetNumber();
	String *getRen3BetNumber();
	String *getRen3Zu3BetNumber();
	String *getRen3Zu6BetNumber();
	String *getRen2BetNumber();
	String *getRen2ZuXuanBetNumber();

	//获取随机号码；机选号码
	void createDXDSRandomNumbers(int nArea);			//大小单双机选
	void createZuXuanRandomNumbers(int area, int num);
	void createWuxingZhixuanRandomNumbers();
	void createRenxuanRandomNumber(int num); //任选num 个数  总共是5个
	void createZuxuan120RandomNumbers();
	void createZuxuan60RandomNumbers();
	void createZuxuan30RandomNumbers();
	void createZuxuan20RandomNumbers();
	void createZuxuan10RandomNumbers();
	void createZuxuan5RandomNumbers();
	void createWuxingQianSiRandomNumbers();
	void createWuxingHousiRandomNumbers();
	//前四
	void createZuxuanQS24RandomNumber();
	void createZuxuanQS12RandomNumber();
	void createZuxuanQS6RandomNumber();
	void createZuxuanQS4RandomNumber();
	//后四
	void createZuxuanHS24RandomNumber();
	void createZuxuanHS12RandomNumber();
	void createZuxuanHS6RandomNumber();
	void createZuxuanHS4RandomNumber();

	void createQianSanZhiXuan_FuShiRandomNumbers();
	void createQianSanZuXuan_ZuSanFuShiRandomNumbers();
	void createQianSanZuXuan_ZuLiuFuShiRandomNumbers();
	void createQianSanZhiXuanHeZhiRandomNumbers();
	void createZhongSanZhiXuan_FuShiRandomNumbers();
	void createZhongSanZuXuan_ZuSanFuShiRandomNumbers();
	void createZhongSanZuXuan_ZuLiuFuShiRandomNumbers();
	void createZhongSanZhiXuanHeZhiRandomNumbers();
	void createHouSanZhiXuan_FuShiRandomNumbers();
	void createHouSanZuXuan_ZuSanFuShiRandomNumbers();
	void createHouSanZuXuan_ZuLiuFuShiRandomNumbers();
	void createHouSanZhiXuanHeZhiRandomNumbers();
	void createWuXingHouEr_FuShiRandomNumbers();
	void createWuXingQianEr_FuShiRandomNumbers();
	void createHouErZuXuan_FuShiRandomNumbers();
	void createQianErZuXuan_FuShiRandomNumbers();
	void createDingWeiDanRandomNumbers();
	void createBuDingWei_HouSanRandomNumbers();
	void createBuDingWei_QianSanRandomNumbers();
	void createBuDingWei_HouErRandomNumbers();
	void createBuDingWei_QianErRandomNumbers();
	void createQW_YiFanFengShunRandomNumbers();
	void createQW_HaoShiChengShuangRandomNumbers();
	void createQW_SanXingBaoXiRandomNumbers();
	void createQW_SiJiFaCaiRandomNumbers();
	void createLongHuHeRandomNumbers();
	//单式随机
	void createDanShiRandomNumber();
	//任选
	void createRen4RandomNumbers();
	void createRen3RandomNumbers();
	void createRen3Zu3RandomNumbers();
	void createRen3Zu6RandomNumbers();
	void createRen2RandomNumbers();
	void createRen2ZuXuanRandomNumbers();

	//添加方法
	void updateRenXuanHeadTips();
	//计算注数和字符串
	void countTouZhuParams();

	//

	int ballStatus[6][30];
	int menuStatus[6][6]; 
	int areaStatus[6];

	int isWanWeiSelected;
	int isQianWeiSelected;
	int isBaiWeiSelected;
	int isShiWeiSelected;
	int isGeWeiSelected;
};
#endif  //__Game__HelpScrollView__