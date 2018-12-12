#pragma once
#ifndef __MY_LOGIN_LAYER_H__
#define __MY_LOGIN_LAYER_H__


#include "cocos2d.h"
#include "ui/UIWidget.h"

using namespace cocos2d;
//using namespace cocos2d::ui;

class MyLoginLayer : public cocos2d::Layer
{
public:
	ui::Widget* MyLoginLayerRoot;
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();
	static MyLoginLayer* LOGINSCENE;

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	void MyLoginLayerGame(Ref * pSender, cocos2d::ui::Widget::TouchEventType type);
	void OnCloseBulletinBoard(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);

	void OnGetBullet(const std::string& strBullet);
	// implement the "static create()" method manually
	CREATE_FUNC(MyLoginLayer);

private:
	void onAnsLoginFinish(Ref* obj);
};

#endif // __MY_LOGIN_LAYER_H__
