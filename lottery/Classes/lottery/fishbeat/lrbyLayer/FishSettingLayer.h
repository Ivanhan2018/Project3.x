#ifndef _FISH_SETTING_LAYER_H_
#define _FISH_SETTING_LAYER_H_

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"

USING_NS_CC;

enum{
	TAG_SETTING = 1,		//设置
	TAG_LEAVE,				//离开
	TAG_TIP,				//提示
	TAG_DISCONNECT,			//网络断开
	TAG_IN_BACKGROUND		//长时间在后台
};

class FishSettingLayer : public Layer
{
public:
	CREATE_FUNC(FishSettingLayer);
	bool init();
	FishSettingLayer();
	virtual ~FishSettingLayer();

	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
	virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
	virtual void onTouchCancelled(Touch *pTouch, Event *pEvent);

	void setLayerType(int type);

private:
	ui::Button*					m_btnQuit;
	ui::Button*					m_btnSure;
	ui::Button*					m_btnCancle;
	ui::ImageView*				m_bg;

	ui::Button*					m_btnMusic;
	ui::Button*					m_btnMusicEffect;

	int m_type;

	bool m_music;
	bool m_musicEffect;

	void buttonQuit(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type);

	void buttonSure(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type);

	void buttonMusicBg(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type);

	void buttonMusicEffect(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type);

	void playBackgroudMusic(bool isPlay);

	void playMusicEffect(bool isPlay);

private:
	void initSettingLayer();

	void initTipLayer();

	void initLeaveLayer();

	void initDisconnectLayer();

	void initInBackgroundLayer();

	void leaveGame();
};


#endif // _INTRODUCELAYER_H_

