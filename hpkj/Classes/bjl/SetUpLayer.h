#ifndef __SETUPLAYER__
#define __SETUPLAYER__
#include "cocos2d.h"
#include "BJLRes.h"
#include "SoundControl.h"
#include <limits>
USING_NS_CC;

class SetUpLayer : public Layer
{
public:
	SetUpLayer();
	~SetUpLayer();
	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchMoved(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);
	void onTouchCancelled(Touch *pTouch, Event *pEvent);
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	CREATE_FUNC(SetUpLayer);
private:
	bool m_voiceIsOpen;		//声音是否打开
	bool m_faceIsOpen;		//表情功能是否打开
	float m_musicVol;//背景音量
	float m_soundEffVol;//音效音量
	Sprite * m_voice;		//声音状态
	Sprite * m_voiceOnOff;	//声音开关按钮图片
	Sprite * m_face;		//表情状态
	Sprite * m_faceOnOff;	//表情开关按钮图片
	Sprite * m_bgSprite;	//背景
	Vec2 m_bgPoint;		//背景位置
	void setVoiceState(bool voiceState);	//设置声音状态
	void setFaceState(bool faceState);		//设置表情开关状态
	SpriteBatchNode * m_batchNode;
	ProgressTimer * m_soundEffTimer;	//音效条
	ProgressTimer * m_musicTimer;		//背景音量条
	Sprite * m_soundEffTimerBar;	//音效条上的按钮
	bool m_soundBarIsSelected;		//音效条按钮是否按下
	Sprite * m_musicTimerBar;		//背景声音条上的按钮
	bool m_musicBarIsSelected;		//背景音量条是否按下
};

#endif