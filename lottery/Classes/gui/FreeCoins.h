//
//  FreeCoins.h
//  Game
//
//  Created by wangjun on 15-1-7.
//
//

#ifndef __Game__FreeCoins__
#define __Game__FreeCoins__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GBEvent.h"
#include "PromptBox.h"
USING_NS_CC;
USING_NS_CC_EXT;

enum FreeCoinsTab
{
	FreeCoinsTabAlms,
	FreeCoinsTabRecommend,
	FreeCoinsTabRecommender
};

class FreeCoins : public Layer
{
public:
	CREATE_FUNC(FreeCoins);
	virtual bool init();

	FreeCoins();
	~FreeCoins();

	void getAlms( Object* obj);
	void recommend( Object* obj);
	void myRecommender( Object* obj);
	void pressGetCoins( Object* obj);
	void back(Object* obj);
	void onCheckVersion(Object* obj);
	void cancelHttpRequest(Object *obj);
	void onHttpRequestCompleted(Node *sender, void *data);
	void onEnter();
	void onExit();

	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);					//手机返回按钮

	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchMoved(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);
	void onTouchCancelled(Touch *pTouch, Event *pEvent);
private:
	FreeCoinsTab currentItem;
	MenuItemSprite* almsItem;
	MenuItemSprite* recommendItem;
	MenuItemSprite* recommenderItem;
	LabelTTF* almsText;
	LabelTTF* recommendText;
	LabelTTF* recommenderText;
	Menu *getCoinsMenu;
	EditBox *recommenderName;
	LabelTTF* recommenderLabel;
	PromptBox* promptBox;
	bool cancelOrNot;
};

#endif /* defined(__Game__RegistScene__) */
