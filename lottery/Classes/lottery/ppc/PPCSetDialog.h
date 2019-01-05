#ifndef _PPC_SETTING_LAYER_H_
#define _PPC_SETTING_LAYER_H_

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"

USING_NS_CC;

class PPCSetDialog : public Layer
{
public:
	CREATE_FUNC(PPCSetDialog);
	bool init();
	PPCSetDialog();
	virtual ~PPCSetDialog();

	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
	virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
	virtual void onTouchCancelled(Touch *pTouch, Event *pEvent);

private:
	ui::Button*					m_btnQuit;
	ui::Button*					m_btnSure;
	ui::ImageView*				m_bg;

	ui::Button*					m_btnMusic;
	ui::Button*					m_btnMusicEffect;

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
};


#endif // _INTRODUCELAYER_H_

