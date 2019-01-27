//  [4/8/2014 xieyi]
#ifndef DDZ___DIALOGLAYER__
#define DDZ___DIALOGLAYER__
#include "cocos2d.h"
#include "DDZRes.h"
#include <limits>
#include "ui/CocosGUI.h"

USING_NS_CC;

enum DDZdialog_type
{
	DDZdialog_hints = 1,//断线重连
	DDZdialog_back,//返回
	DDZdialog_poChan,//破产
	DDZdialog_poChanSongCoin,//破产送金币
	DDZdialog_dismissTable,//解散牌桌
	DDZdialog_roomLimitMin,//房间限制
	DDZdialog_roomLimitMax,//房间限制
	DDZdialog_ChangeTableFailed,	//	换桌失败
	DDZdialog_Reject,	//	剔除玩家
	DDZdialog_Other_Login,	//	异地登录
};


class DDZDialogLayer : public Layer
{

public:
	DDZDialogLayer(DDZdialog_type type);
	~DDZDialogLayer();
	virtual bool init();
	virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void onTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

	static DDZDialogLayer * create(DDZdialog_type type);
	DDZdialog_type m_type;
	virtual void onEnter();
	virtual void onExit();
	//	设置提示语内容
	void setTipsString(const std::string sText);

private:
	//	按钮——确定
	ui::Button* m_pBtnOK;	

	//	按钮——取消
	ui::Button* m_pBtnCancel;

	//	提示语
	LabelTTF * m_pLabTips;

	//	点击确定按钮事件
	void onClickBtnOK(Ref* pSender);

	//	点击取消按钮事件
	void onClickBtnCancel(Ref* pSender);

	void initTable(const string sName,int type);		//--1为只有确定，2为确定，取消
};

#endif