//  [4/8/2014 xieyi]
#ifndef __DIALOGLAYER__
#define __DIALOGLAYER__
#include "cocos2d.h"
#include "BJLRes.h"
#include <limits>
USING_NS_CC;

enum dialog_type
{
	dialog_hints = 1,//断线重连
	dialog_back,//返回
	dialog_poChan,//破产
	dialog_poChanSongCoin,//破产送金币
	dialog_dismissTable,//解散牌桌
	dialog_roomLimitMin,//房间限制
	dialog_roomLimitMax,//房间限制
	dialog_BankerList,//上庄列表<by hxh>
};


class DialogLayer : public Layer
{

public:
	DialogLayer(dialog_type type);
	~DialogLayer();
	virtual bool init();
	bool onTouchBegan(Touch *pTouch, Event *pEvent);;
	virtual void onTouchMoved(Touch *pTouch, Event *pEvent){};
	virtual void onTouchEnded(Touch *pTouch, Event *pEvent){};
	virtual void onTouchCancelled(Touch *pTouch, Event *pEvent){};

	static DialogLayer * create(dialog_type type);
	dialog_type m_type;
	virtual void onEnter();
	virtual void onExit();

private:
	void menuOkCallBack(Object * pSender);	//确定
	void menuCancleCallBack(Object * pSender);//取消
	void initTable(char* Name,int type);		//--1为只有确定，2为确定，取消
	LabelTTF * m_ttf;
};

#endif