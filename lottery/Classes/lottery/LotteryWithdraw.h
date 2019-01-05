#ifndef __LotteryWithdraw__
#define __LotteryWithdraw__
#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>
#include "Define.h"

USING_NS_CC_EXT;
USING_NS_CC;
using namespace std;

class LotteryWithdraw: public Layer, public EditBoxDelegate
{

public:
	LotteryWithdraw();
	~LotteryWithdraw();
	CREATE_FUNC(LotteryWithdraw);
	virtual bool init();
	static Scene* scene();
	virtual void onEnter();
	virtual void onExit();
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);	
	//±à¼­
	virtual void editBoxEditingDidBegin(EditBox* editBox);
	virtual void editBoxEditingDidEnd(EditBox* editBox);
	virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
	virtual void editBoxReturn(EditBox* editBox);

private:
	void initData();
	void initView();		
	void initWithdrawView();
	//top menu
	void back(Object *obj);

	void bangDing(Object *obj);
	void queryMyYinHangRet(Object *obj);
	void confirmWithdraw(Object *obj);
	void doQukuanRet(Object *obj);
	void getLastYueRet(Object *obj);
	void getQuKuanLimitRet(Object *obj);

	cocos2d::Size winSize;
	Node *m_node;
	string username;
	DOUBLE money;
	float posX1;
	float posX2;
	float posX3;
	float gapY;
	float startY;

	Integer *n_t_sxfpercent;
	Double *f_t_lowsxf;
	Double *f_t_highsxf; 
	Integer *n_t_freelastcount; 
	Double *f_t_lowjine;
	Double *f_t_highjine;

	void clear();
};

#endif