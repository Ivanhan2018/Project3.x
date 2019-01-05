//
//  RegistScene.h
//  Game
//
//  Created by 兰 天乐 on 13-6-7.
//
//

#ifndef __Game__RoomOption__
#define __Game__RoomOption__

#include "cocos2d.h"
#include "GBEvent.h"
USING_NS_CC;

class RoomOption : public Layer
{
public:
    CREATE_FUNC(RoomOption);
    virtual bool init();
    
    RoomOption();
    ~RoomOption();

	void back(Object* obj);
	void changeAccount(Object* obj);
	void openSilentMode(Object* obj);
	void checkVesion(Object* obj);
	void onCheckVersion(Object* obj);
	void onEnter();
	void onExit();

	void toFreeLayer(Object* obj);

	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);					//手机返回按钮

	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchMoved(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);
	void onTouchCancelled(Touch *pTouch, Event *pEvent);
private:
    MenuItemSprite* m_pSilentItemSprite;
	bool m_bOpenSlientMode;

	Sprite* m_pMusicControlSprite;
	Sprite* m_pMusicControlBlackBack;
	ProgressTimer* m_pMusicControlWhiteTimer;

	Sprite* m_pSoundControlSprite;
	Sprite* m_pSoundControlBlackBack;
	ProgressTimer* m_pSoundControlWhiteTimer;

	int m_nChangeVoice;				//1为音乐，2为音效
};

#endif /* defined(__Game__RegistScene__) */
