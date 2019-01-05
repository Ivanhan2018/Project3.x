#ifndef __DZPK_DIALOG_H__
#define __DZPK_DIALOG_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

//	对话框——基类
class DialogBase: public Layer
{
public:
	DialogBase();
	~DialogBase();
	virtual bool init();
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchCancelled(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);

	//	关闭对话框
	virtual void closed();
private:

protected:
	//	可视窗口尺寸
	 cocos2d::Size m_sVSize;

};

//	对话框——胜负结局
class DialogEnding: public DialogBase
{
public:
	DialogEnding();
	~DialogEnding();
	enum EndingType
	{
		EndingType_Win = 0,	//	胜
		EndingType_Lose,	//	负
		EndingType_Draw		//	和
	};

	static DialogEnding* create(EndingType eType);
	bool init(EndingType eType);

	void initUI(EndingType eType);
private:

};

//	对话框——退出游戏
class DialogQuit: public DialogBase
{
public:
	DialogQuit();
	~DialogQuit();
	CREATE_FUNC(DialogQuit);
	bool init();
	void initUI();
private:

};

//	对话框——超时退出
class DialogTimeOutQuit: public DialogBase
{
public:
	DialogTimeOutQuit();
	~DialogTimeOutQuit();
	CREATE_FUNC(DialogTimeOutQuit);
	bool init();
	void initUI();
private:
	//	背景
	Sprite* m_pBG;

	//	按钮——确定
	ui::Button* m_pBtnOK;

	//	点击确定按钮
	void onClickBtnOK(Ref* sender);
};




#endif	//	__DZPK_DIALOG_H__