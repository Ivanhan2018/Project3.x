#ifndef __LiuHe_GAMBLELAYER_H__
#define __LiuHe_GAMBLELAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>
#include "LiuHeWuFen.h"
#include "comm.h"
#include "Define.h"
#include "GameRule.h"
#include "LotteryGlobal.h"
#include "BaseLayer.h"
#include "LiuHeKind.h"

#define LAYER_HEIGHT 55

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class LiuHeGambleLayer : public BaseLayer, public EditBoxDelegate,public TableViewDelegate,
	public TableViewDataSource
{
public:
	LiuHeGambleLayer();
	~LiuHeGambleLayer();
	CREATE_FUNC(LiuHeGambleLayer);
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

	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	virtual cocos2d::Size cellSizeForTable(TableView *table);
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(TableView *table);
	virtual void scrollViewDidScroll(ScrollView* view);
	virtual void scrollViewDidZoom(ScrollView* view);

	//更新
	void refreshLayer();
private:
	void setWanFaKind(LiuHeCaiKind kind);
	void initData();
	void initView();
	void touZhuRet(Object *obj);
	//top menu
	void back(Object *obj);
	void setting(Object* obj);
	void jumpResult(Object* obj);

	void toMore(Object* obj);	
	void toDelete(Object* obj);

	//kind layer show and hide
	void showKindView();
	void hideKindView();
	
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
	void updatePeiLv(Object *obj);
	void updateTouZhu(Object *obj);
	void updateFenDan(Object *obj);
	
	void getSystemTime(float dt);
	void myUpdate(float dt);
	cocos2d::Size winSize;
	CGameRule *gameRule;
	//当前期数
	LONGLONG currentLotteryId;
	//封单时间
	string currentTime;

	LiuHeBetArea *betArea;
	
	LiuHeCaiKind wanFaKind;
	int selectedItemTag;
	float TouZhuJinE;
	float ZhangHuYuE;
	//float TouZhuJinEJubu;
	CaiZhong gameKind;

	LiuHeKind *kindLayer;
	MenuItemSprite *moreItem;
	MenuItemSprite *cartItem;
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
	LabelTTF* TouZhuJinELabel; //投注金额
	LabelTTF *beilvBkTTf; //当前赔率

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

	void kuaNianExpect(Object *obj);

	Array* m_data;
	void transferString(string &str, int nStep);
	void queryGameMoreRet(Object* obj);
	bool m_bIsOpenList;

	bool isRun;
	LayerColor* lyTables;
	LayerColor* lyRefresh;
};

#endif // __HELLOWORLD_SCENE_H__