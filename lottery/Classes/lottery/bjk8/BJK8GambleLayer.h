#ifndef __BJK8_GAMBLELAYER_H__
#define __BJK8_GAMBLELAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>
#include "BJK8WuFen.h"
#include "comm.h"
#include "Define.h"
#include "BJK8Kind.h"
#include "BJK8Cart.h"
#include "GameRule.h"
#include "LotteryGlobal.h"
#include "BaseLayer.h"

#define LAYER_HEIGHT 55

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class BJK8GambleLayer : public BaseLayer, public EditBoxDelegate,public TableViewDelegate,
	public TableViewDataSource
{
public:
	BJK8GambleLayer();
	~BJK8GambleLayer();
	CREATE_FUNC(BJK8GambleLayer);
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
	virtual void editBoxTextChanged(EditBox* editBox, const std::string& text) {};
	virtual void editBoxReturn(EditBox* editBox){};

	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	virtual cocos2d::Size cellSizeForTable(TableView *table);
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(TableView *table);
	virtual void scrollViewDidScroll(ScrollView* view);
	virtual void scrollViewDidZoom(ScrollView* view);

	//更新
	void refreshLayer();
private:
	void setWanFaKind(BeiJingKuai8Kind kind);
	void initData();
	void initView();	
	void touZhuRet(Object *obj);
	//top menu
	void back(Object *obj);
	void setting(Object* obj);
	void jumpResult(Object* obj);
	//middle menu
	void toMore(Object* obj);
	void toRandomGamble(Object* obj);
	void ZhuiHaoTouZhuJinENumber(Object *obj);
	void ShiYiXuanWoNumber(Object *obj);
	//bottom menu
	void toDelete(Object* obj);
	void toCart(Object* obj);
	void toGamble(Object* obj);
	//
	void changeKind(Object *obj);
	void deleteItem(Object *obj);
	//返回继续投注
	void addRandomItem(Object *obj); 
	void cartConfirm(Object *obj);
	void clearNumbers(Object *obj);
	void SetGameblenMoshi(Object *obj);
	void getSystemTimeObject(Object *obj);
	//kind layer show and hide
	void showKindView();
	void hideKindView();
	//update period
	void updatePeriod(float dt);
	void updateLotteryId();
	void updateCurrentTime();

	void calculateCQSSCQiShu();
	void XuanZeMonenyLayer(Object *obj);
	void updateTouZhuJine(Object *obj);
	void GetLastYueRet(Object *obj);
	
	void getSystemTime(float dt);
	void myUpdate(float dt);
	cocos2d::Size winSize;
	CGameRule *gameRule;
	//当前期数
	LONGLONG currentLotteryId;
	//封单时间
	string currentTime;

	BJK8BetArea *betArea;
	
	BeiJingKuai8Kind wanFaKind;
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
	LabelTTF *gambleTypeTitle;
	LabelTTF *lotteryIdLabel;
	LabelTTF *currentTimeLabel;
	LabelTTF* TouZhuJinELabel;
	//LabelTTF* ZhangHuYuELabel;
	BJK8Kind *kindLayer;
	MenuItemSprite *moreItem;
	MenuItemSprite *cartItem;
	bool kindLayerIsShow;
	BJK8Cart *cart;
	MenuItemSprite *YuanSelectedImageLayer;
	MenuItemSprite *JiaoSelectedImageLayer;
	MenuItemSprite *FenSelectedImageLayer;
	MenuItemSprite *LiSelectedImageLayer;  //新增厘模式
	Menu* MonenyMenuLayer;
	int   nLMoshi;
    WORD wYear;
    WORD wMonth;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;

	//新增
	EditBox *beilvInput;
	LabelTTF* Cardnumttf;

	DOUBLE fandian ;
	int fandianubt;

	void FanDianMinus(Object *obj);
	LayerColor* layerColor;
	ProgressTimer *pJinDuTiao;
	Sprite *henggangSprite;
	float percent;
	LabelTTF * numLingttf;
	LabelTTF * numttf;
	float BaiFenbiNumber;
	float PlusFanDianvalieMAX;
	float FandianBaifenbipColse;
	float FandianNumberpColse;
	float fandianNumber;
	float henggangPointX;
	float GlobalVariableFanDianvalie;
	float JinduPosX;
	float lenJindu;
	float FanDianbI;
	bool SlidePlusde;

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

	void PlusFianDian(Object *obj);
	void MinusFanDian(Object *obj);
	void pCoseGameResume(Object *obj);
	void SuccenGameResume(Object *obj);
	void KindMyFanDianRet(Object *obj);
	
	String * GetBonus(){ return f_t_bonus;};
	void pressMinus(Object *obj);
	void beiAdd(Object *obj);
	void beiMinus(Object *obj);
	void updateMoneyLabel();
	//void isCartShowLayer(Object *obj);
	void isFenDanHour(Object *obj);

	void kuaNianExpect(Object *obj);

	bool m_bIsFendan;

	Array* m_data;
	void transferString(string &str, int nStep);
	void queryGameMoreRet(Object* obj);
	bool m_bIsOpenList;

	bool isRun;
	LayerColor* lyTables;
	LayerColor* lyRefresh;

};

#endif // __HELLOWORLD_SCENE_H__