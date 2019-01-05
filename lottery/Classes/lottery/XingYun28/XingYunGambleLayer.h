#ifndef __XINGYUN_GAMBLELAYER_H__
#define __XINGYUN_GAMBLELAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>
#include "XingYunWuFen.h"
#include "comm.h"
#include "Define.h"
#include "GameRule.h"
#include "LotteryGlobal.h"
#include "BaseLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class XingYunGambleLayer : public BaseLayer, public EditBoxDelegate
{
public:
	XingYunGambleLayer();
	~XingYunGambleLayer();
	CREATE_FUNC(XingYunGambleLayer);
	virtual bool init();
	static Scene* scene();
	virtual void onEnter();
	virtual void onExit();
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);	
	void setGameKind(CaiZhong kind);
	CaiZhong getGameKind(){ return gameKind; };

	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchMoved(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);

	//倍数
	virtual void editBoxEditingDidBegin(EditBox* editBox) {};
	virtual void editBoxEditingDidEnd(EditBox* editBox);
	virtual void editBoxTextChanged(EditBox* editBox, const std::string& text){};
	virtual void editBoxReturn(EditBox* editBox){};

	//更新
	void refreshLayer();
private:
	void setWanFaKind(XingYun28Kind kind);
	void initData();
	void initView();
	void touZhuRet(Object *obj);
	//top menu
	void back(Object *obj);
	void setting(Object* obj);
	void jumpResult(Object* obj);
	
	//bottom menu
	void toGamble(Object* obj);
	void toCustom(Object* obj);
	void toConfirm(Object* obj);
	void toCancel(Object* obj);
	//
	void changeKind(Object *obj);
	//返回继续投注	
	void getSystemTimeObject(Object *obj);

	//update period
	void updatePeriod(float dt);
	void updateLotteryId();
	void updateCurrentTime();

	void calculateCQSSCQiShu();
	
	void GetLastYueRet(Object *obj);
	
	void getSystemTime(float dt);
	void myUpdate(float dt);
	cocos2d::Size winSize;
	CGameRule *gameRule;
	//当前期数
	LONGLONG currentLotteryId;
	//封单时间
	string currentTime;

	XingYunBetArea *betArea;
	
	XingYun28Kind wanFaKind;
	int selectedItemTag;
	float TouZhuJinE;
	float ZhangHuYuE;
	//float TouZhuJinEJubu;
	CaiZhong gameKind;
	//注数
	int betZhuShuTouZhuJinE; 
	//float  fWinPrize;					//可赢金额
	Integer *wKindID;
	String *wPeriod;
	String *szLotNum;
	String *szShijian;
	Sprite *gambleTypeBk;
	LabelTTF *gameTitle;
	//LabelTTF *gambleTypeTitle;
	LabelTTF *lotteryIdLabel;
	LabelTTF *currentTimeLabel;
	LabelTTF* TouZhuJinELabel;

	bool kindLayerIsShow;	

    WORD wYear;
    WORD wMonth;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;

	//新增
	EditBox *beilvInput;
	LabelTTF* Cardnumttf;

	Integer *n_t_kindid;
	String *f_t_bonus;
	String *f_t_bonusPercent;
	String *f_t_fandian;
	float beilvInputMidX;
	int beiShu;

	Vec2 startPoint;
	Vec2 endPoint;

	bool isReceiveFanDian;

	//统计次数
	//投了多少条记录
	int touZhuCount;
	//该显示的提示
	std::string strNeedDisplay;

	String * GetBonus(){ return f_t_bonus;};
	
	void updateMoneyLabel();

	//跨年返回
	void kuaNianExpect(Object *obj);
};

#endif // __HELLOWORLD_SCENE_H__