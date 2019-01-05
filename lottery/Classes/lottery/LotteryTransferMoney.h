#ifndef __LotteryTransferMoney__
#define __LotteryTransferMoney__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "Define.h"
#include <string>

USING_NS_CC_EXT;
USING_NS_CC;
using namespace std;

class LotteryTransferMoney: public Layer, public EditBoxDelegate
{

public:
	LotteryTransferMoney();
	~LotteryTransferMoney();
	CREATE_FUNC(LotteryTransferMoney);
	virtual bool init();
	static Scene* scene();
	virtual void onEnter();
	virtual void onExit();
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);	

	virtual void editBoxEditingDidBegin(EditBox* editBox);
	virtual void editBoxEditingDidEnd(EditBox* editBox);
	virtual void editBoxTextChanged(EditBox* editBox, const std::string& text) {};
	virtual void editBoxReturn(EditBox* editBox){};

	//typeTrans 0转彩票 1转游戏
	void setTransferType(int typeTrans);
private:
	void initData();
	void initView();	
	void contactKefu(Object* obj);	
	//top menu
	void back(Object *obj);
	void transferToLottery(Object* obj);
	void transferToQiPai(Object *obj);

	void updataTransferMoneyTitlelabel();
	void updataTransferMoneylabel();
	void updataTransferTypeLabel();

	void updateUI();

	void pressConfirm(Object *obj);
	void zhuanHuanRet(Object *obj);
	void getLastYueRet(Object *obj);

	void getKeFuUrl(Object *obj);
	void getNeedRefresh(Object* obj);
	String* kefuStr;

	cocos2d::Size winSize;
	string username;
	DOUBLE money;
	DOUBLE qiPaiMoney;
	LabelTTF *moneyLabel;
	LabelTTF *qiPaiMoneyLabel;
	int transferType;				//0转彩票 1转游戏
	float posX1;
	float posX2;
	float gapY;
	float startY;
	LabelTTF *transferMoneyTitlelabel;
	LabelTTF *transferMoneylabel;
	LabelTTF *transferTypelabel;
	EditBox *transferMoney;
	DOUBLE transferMoneyLast;
};

#endif